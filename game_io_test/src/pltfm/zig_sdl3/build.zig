// Using Zig 0.16.0

// static link SDL3
// zig fetch --save=SDL git+https://github.com/allyourcodebase/SDL3

// SDL3 Mixer binding not yet available 2026-07-08

const std = @import("std");

const root = "../../../..";
const app = root ++ "/game_io_test";
const res = app ++ "/res";
const bin = res ++ "/io_test_data.bin";

const app_name = "io_test";

const main_cpp = app ++ "/src/pltfm/sdl3/main_o.cpp";

const cpp_flags = &[_][]const u8{
    "-std=c++20",
    "-mavx",
    "-mavx2",
    "-mfma",
    "-O3",
    "-DNDEBUG",
    //"-DALLOC_COUNT",
    //"-DAPP_FULLSCREEN"
};

const targets: []const std.Target.Query = &.{
    //.{ .cpu_arch = .aarch64, .os_tag = .macos },
    .{ .cpu_arch = .aarch64, .os_tag = .linux },
    .{ .cpu_arch = .x86_64, .os_tag = .linux, .abi = .gnu },
    .{ .cpu_arch = .x86_64, .os_tag = .linux, .abi = .musl },
    .{ .cpu_arch = .x86_64, .os_tag = .windows },
};

pub fn build(b: *std.Build) void {
    const optimize = b.option(std.builtin.OptimizeMode, "optimize", "Optimization mode") orelse .ReleaseFast;

    for (targets) |t| {
        const exe = b.addExecutable(.{
            .name = app_name,
            .root_module = b.createModule(.{
                .root_source_file = null,
                .target = b.resolveTargetQuery(t),
                .optimize = optimize,
                .link_libc = true,
                .link_libcpp = true,
            }),
        });

        if (t.os_tag == .windows) {
            exe.subsystem = .Windows;
        }

        exe.root_module.addCSourceFiles(.{
            .files = &.{
                main_cpp,
            },
            .flags = cpp_flags,
        });

        const sdl_dep = b.dependency("SDL", .{
            .optimize = optimize,
            .target = t,
        });
        exe.root_module.linkLibrary(sdl_dep.artifact("SDL3"));

        b.installArtifact(exe);

        const copy_data = b.addInstallBinFile(
            b.path(bin),
            "punk_run.bin",
        );
        b.getInstallStep().dependOn(&copy_data.step);
    }
}
