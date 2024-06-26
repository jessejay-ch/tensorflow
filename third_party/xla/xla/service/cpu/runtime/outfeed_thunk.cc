/* Copyright 2024 The OpenXLA Authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "xla/service/cpu/runtime/outfeed_thunk.h"

#include <cstdint>
#include <cstring>

#include "absl/status/status.h"
#include "absl/strings/str_format.h"
#include "absl/types/span.h"
#include "xla/service/buffer_assignment.h"
#include "xla/service/cpu/runtime/thunk.h"
#include "xla/service/cpu/xfeed_manager.h"
#include "xla/stream_executor/device_memory.h"
#include "xla/util.h"
#include "tsl/platform/logging.h"
#include "tsl/platform/statusor.h"
#include "tsl/profiler/lib/traceme.h"

namespace xla::cpu {

OutfeedThunk::OutfeedThunk(Info info,
                           absl::Span<const OutfeedBuffer> outfeed_buffers)
    : Thunk(Kind::kOutfeed, info),
      outfeed_buffers_(outfeed_buffers.begin(), outfeed_buffers.end()) {}

absl::Status OutfeedThunk::Execute(const ExecuteParams& params) {
  tsl::profiler::TraceMe trace([&] { return TraceMeEncode(); });

  VLOG(3) << absl::StreamFormat("Outfeed %d buffers", outfeed_buffers_.size());

  runtime::XfeedManager* xfeed = params.xfeed;
  if (xfeed == nullptr) {
    return InvalidArgument("Xfeed must be not null to execute outfeed thunk");
  }

  int64_t outfeed_num = 0;

  for (OutfeedBuffer& outfeed_buffer : outfeed_buffers_) {
    TF_ASSIGN_OR_RETURN(
        se::DeviceMemoryBase outfeed_data,
        params.buffer_allocations->GetDeviceAddress(outfeed_buffer.slice));

    VLOG(3) << absl::StreamFormat(
        "  outfeed #%d: %s into slice %s (%p)", outfeed_num++,
        outfeed_buffer.shape.ToString(), outfeed_buffer.slice.ToString(),
        outfeed_data.opaque());

    // Acquire outfeed buffer from the runtime.
    runtime::XfeedBuffer* buffer = xfeed->outfeed()->BlockingDequeueBuffer();
    if (buffer->length() != outfeed_buffer.slice.size()) {
      return Internal(
          "XLA runtime-managed outfeed buffer size %d did not match the "
          "outfeed operation parameter buffer size %d",
          buffer->length(), outfeed_buffer.slice.size());
    }

    // TODO(ezhulenev): Benchmark multi threaded memcpy and consider add a
    // parallel memcpy that can be reused in Copy thunk.

    // Copy data from outfeed buffer to the destination.
    std::memcpy(buffer->data(), outfeed_data.opaque(), buffer->length());

    // Release outfeed buffer back to the runtime.
    xfeed->outfeed()->ReleaseCurrentBuffer(buffer->length(), buffer->data(),
                                           outfeed_buffer.shape);
  }

  return absl::OkStatus();
}

}  // namespace xla::cpu
