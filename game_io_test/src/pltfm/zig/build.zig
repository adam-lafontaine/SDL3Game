const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "myapp",
        .root_source_file = null,
        .target = target,
        .optimize = optimize,
    });

    exe.addCSourceFiles(.{
        .files = &[_][]const u8{
            "main.cpp",
            //"utils.cpp",
        },
        .flags = &[_][]const u8{
            "-std=c++17",
        },
    });

    // Link SDL2 using pkg-config
    exe.linkSystemLibrary("SDL2"); // This uses pkg-config implicitly
    exe.linkLibCpp();              // Link C++ standard library

    // Install the executable
    //b.installArtifact(exe);

    // Customize the install directory
    const install_step = b.addInstallArtifact(exe, .{
        .dest_dir = .{ .override = .{ .custom = "../mybuild/bin" } }, // Relative to project root
    });
    b.getInstallStep().dependOn(&install_step.step);

    // Run step
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    b.step("run", "Run the app").dependOn(&run_cmd.step);
}