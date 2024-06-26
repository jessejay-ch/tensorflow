# Description:
#   Computationally expensive, exhaustive tests for XLA

load("@local_tsl//tsl/platform:rules_cc.bzl", "cc_library")
load("//xla/tests:build_defs.bzl", "xla_test")

package(
    # copybara:uncomment default_applicable_licenses = ["//tensorflow:license"],
    default_visibility = [":friends"],
    licenses = ["notice"],
)

package_group(
    name = "friends",
    includes = [
        "//xla:friends",
    ],
    packages = [
        "//platforms/testing/tests/...",
    ],
)

cc_library(
    name = "exhaustive_op_test_utils",
    testonly = True,
    srcs = ["exhaustive_op_test_utils.cc"],
    hdrs = ["exhaustive_op_test_utils.h"],
    tags = ["no_pip"],
    deps = [
        "//xla:bit_cast",
        "//xla:shape_util",
        "//xla:xla_data_proto_cc",
        "//xla/client:xla_builder",
        "//xla/client/lib:constants",
        "//xla/client/lib:math",
        "//xla/service:shaped_buffer",
        "//xla/tests:client_library_test_base",
        "//xla/tests:literal_test_util",
        "//xla/tests:test_macros_header",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "exhaustive_unary_test_f32_or_smaller_lib",
    testonly = True,
    srcs = ["exhaustive_unary_test_f32_or_smaller.cc"],
    tags = ["no_pip"],
    deps = [
        ":exhaustive_op_test_utils",
        "//xla:util",
        "//xla/client:xla_builder",
        "//xla/client/lib:math",
        "//xla/tests:client_library_test_base",
        "@com_google_absl//absl/flags:flag",
    ],
)

xla_test(
    name = "exhaustive_unary_test_f32_or_smaller",
    srcs = ["exhaustive_test_main.cc"],
    backends = [
        "gpu",
        "cpu",
    ],
    real_hardware_only = True,  # Very slow on the interpreter.
    shard_count = 50,
    tags = [
        "optonly",
        "test_xla_cpu_thunks",
        # This is a big test that we skip for capacity reasons in OSS testing.
        "no_oss",
    ],
    deps = [
        ":exhaustive_unary_test_f32_or_smaller_lib",
        "@local_tsl//tsl/platform",
        "@local_tsl//tsl/platform:test",
    ],
)

xla_test(
    name = "exhaustive_unary_test_f64",
    srcs = ["exhaustive_unary_test_f64.cc"],
    backends = [
        "gpu",
        "cpu",
    ],
    shard_count = 50,
    tags = [
        "optonly",
        "test_xla_cpu_thunks",
        # This is a big test that we skip for capacity reasons in OSS testing.
        "no_oss",
        # TODO(b/151340488): Timed out on 2020-03-18.
        "nozapfhahn",
    ],
    deps = [
        ":exhaustive_op_test_utils",
        "//xla:util",
        "//xla/client:xla_builder",
        "//xla/tests:client_library_test_base",
        "//xla/tests:xla_internal_test_main",
    ],
)

xla_test(
    name = "exhaustive_unary_test_complex",
    srcs = ["exhaustive_unary_test_complex.cc"],
    backends = [
        "gpu",
        "cpu",
    ],
    shard_count = 50,
    tags = [
        "optonly",
        "test_xla_cpu_thunks",
        # This is a big test that we skip for capacity reasons in OSS testing.
        "no_oss",
    ],
    deps = [
        ":exhaustive_op_test_utils",
        "//xla:types",
        "//xla/client:xla_builder",
        "//xla/tests:client_library_test_base",
        "//xla/tests:literal_test_util",
        "//xla/tests:test_macros_header",
        "//xla/tests:xla_internal_test_main",
        "@com_google_absl//absl/types:span",
    ],
)

xla_test(
    name = "exhaustive_binary_16_bit_test",
    srcs = ["exhaustive_binary_16_bit_test.cc"],
    backends = [
        "gpu",
        "cpu",
    ],
    shard_count = 50,
    tags = [
        "optonly",
        "test_xla_cpu_thunks",
        # This is a big test that we skip for capacity reasons in OSS testing.
        "no_oss",
    ],
    deps = [
        ":exhaustive_op_test_utils",
        "//xla/tests:xla_internal_test_main",
    ],
)

xla_test(
    name = "exhaustive_binary_test_f32_f64",
    srcs = ["exhaustive_binary_test_f32_f64.cc"],
    backends = [
        "gpu",
        "cpu",
    ],
    shard_count = 50,
    tags = [
        "optonly",
        "test_xla_cpu_thunks",
        # This is a big test that we skip for capacity reasons in OSS testing.
        "no_oss",
    ],
    deps = [
        ":exhaustive_op_test_utils",
        "//xla/tests:xla_internal_test_main",
    ],
)
