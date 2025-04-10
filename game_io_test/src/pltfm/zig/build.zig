const std = @import("std");

const root = "../../../../..";
const app = root ++ "/game_io_test";
const out = app ++ "/build/zig";

const name = "io_test";

const main_cpp = "io_test_zig_main.cpp";
//const main_cpp = "main.cpp";

const cpp_flags = &[_][]const u8 {
    "-std=c++20",
    "-mavx",
    "-mavx2",
    "-mfma",
    //"-O3",
    //"-DNDEBUG",
    //"-DALLOC_COUNT",
    //"-DAPP_FULLSCREEN"
};


pub fn build(b: *std.Build) void 
{
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = name,
        .root_source_file = null,
        .target = target,
        .optimize = optimize,
    });

    exe.addCSourceFiles(.{
        .files = &[_][]const u8{
            main_cpp,
            //"utils.cpp",
        },
        .flags = cpp_flags,
    });

    // Link SDL2 using pkg-config
    exe.linkSystemLibrary("SDL2"); // This uses pkg-config implicitly
    exe.linkSystemLibrary("SDL2_mixer");
    exe.linkSystemLibrary("pthread");

    exe.linkLibCpp();              // Link C++ standard library

    // Install the executable
    //b.installArtifact(exe);

    // Customize the install directory
    const install_step = b.addInstallArtifact(exe, .{
        .dest_dir = .{ .override = .{ .custom = out } }, // Relative to project root
    });
    b.getInstallStep().dependOn(&install_step.step);

    // Run step
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| 
    {
        run_cmd.addArgs(args);
    }
    b.step("run", "Run the app").dependOn(&run_cmd.step);
}