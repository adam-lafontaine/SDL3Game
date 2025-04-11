const std = @import("std");

// scp game_io_test/src/res/io_test_data.bin adam@192.168.50.14:~/Downloads
// scp zig-out/bin/io_test_rpi adam@192.168.50.14:~/Downloads

const root = "../../../../../.."; // ~/
const repo = root ++ "/Repos/SDL3Game";
const app = repo ++ "/game_io_test";
const out = app ++ "/build/rpi";

const name = "io_test_rpi";

const main_cpp = "../sdl2/io_test_sdl2_main.cpp";

const sysroot = root ++ "/raspberrypi/4b/sysroot";

const sdl_include = sysroot ++ "/usr/include";
const sdl_include_arch = sdl_include ++ "/aarch64-linux-gnu";
const sdl_lib = sysroot ++ "/usr/lib/aarch64-linux-gnu";


const cpp_flags = &[_][]const u8 {
    "-std=c++20",
    "-march=armv8-a",       // ARMv8-A architecture
    "-DPLATFORM_RPI",
    //"-O2",
    //"-DNDEBUG",
    //"-DAPP_FULLSCREEN",
    
    //"-DALLOC_COUNT",
};


pub fn build(b: *std.Build) void 
{
    const target = b.resolveTargetQuery(.{
        .cpu_arch = .aarch64,       // 64-bit ARM
        .os_tag = .linux,
        .abi = .gnu,                // GNU ABI (64-bit)

        .cpu_features_add = std.Target.aarch64.featureSet(&[_]std.Target.aarch64.Feature{
            .v8a,                   // ARMv8-A architecture
            .neon,                  // Enable NEON for SIMD (optional)
            .fp_armv8,              // Floating-point support
        }),
    });


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

    // Sysroot paths for 64-bit Pi
    exe.addSystemIncludePath(b.path(sdl_include));
    exe.addSystemIncludePath(b.path(sdl_include_arch));
    exe.addLibraryPath(b.path(sdl_lib));
    exe.root_module.addRPath(b.path(sdl_lib));

    // Link SDL2 using pkg-config
    exe.linkSystemLibrary("SDL2"); // This uses pkg-config implicitlylibs
    exe.linkSystemLibrary("SDL2_mixer");

    exe.linkLibCpp();              // Link C++ standard library

    // Install the executable
    b.installArtifact(exe);


    // Customize the install directory
    // DOES NOT WORK FOR RPI
    //const install_step = b.addInstallArtifact(exe, .{
    //    .dest_dir = .{ .override = .{ .custom = out } }, // Relative to project root
    //});
    //b.getInstallStep().dependOn(&install_step.step);

    // Run step
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| 
    {
        run_cmd.addArgs(args);
    }
    b.step("run", "Run the app").dependOn(&run_cmd.step);
}