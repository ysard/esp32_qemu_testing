# *****************************************************************************
# Copyright (C) 2025 Craig Setera
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/.
# *****************************************************************************
import os
import subprocess

Import("env")  # type: ignore

platform = env.PioPlatform()  # type: ignore
board_config = env.BoardConfig()  # type: ignore
build_mcu = board_config.get("build.mcu")
# Get the compiled firmware path
flash_image = env.File("$BUILD_DIR/flash_image.bin").abspath  # type: ignore


def build_emulator_image(source, target, env):
    """Build a flash image for use in the emulator"""

    esptool_dir = platform.get_package_dir("tool-esptoolpy")

    # Determine the bootloader offset based on the target device
    match build_mcu:
        case "esp32" | "esp32s2":
            bootloader_offset = "0x1000"
        case "esp32p4":
            bootloader_offset = "0x2000"
        case _:
            bootloader_offset = "0x0000"

    command = [
        os.path.join(esptool_dir, "esptool.py"),
        "--chip", build_mcu,
        "merge_bin",
        "-o", flash_image,
        "--fill-flash-size", board_config.get("upload.flash_size"),
        "--flash_size", "keep",
        "--flash_mode", env["BOARD_FLASH_MODE"],  # type: ignore
        "--flash_freq", "40m",
        bootloader_offset, env.File("$BUILD_DIR/bootloader.bin").abspath,  # type: ignore
        "0x8000", env.File("$BUILD_DIR/partitions.bin").abspath,  # type: ignore
        "0x10000", env.File("$BUILD_DIR/firmware.bin").abspath,  # type: ignore
    ]

    print(f"Building image with command:\n {' '.join(command)}")
    result = subprocess.run(command, capture_output=True, check=True)
    if result.returncode != 0:
        print("ERROR:")
        print(result.stdout)
        return 1
    print("Image built")
    return 0


def start_qemu_emulator(source, target, env):
    """Custom upload function that starts ESP32 QEMU instead of flashing"""

    # QEMU command and arguments
    qemu_cmd = [
        "qemu-system-xtensa",
        "-machine", build_mcu,
        "-drive", f"file={flash_image},if=mtd,format=raw",
        "-display", "gtk",
        "-serial", "stdio",
        "-d", "guest_errors",
    ]

    # print(env.Dump())
    if env.GetProjectOption("build_type") == "debug":
        qemu_cmd = qemu_cmd + ["-s", "-S"]

    print("Starting ESP32 QEMU emulator...")
    print(f"Emulator Command: {' '.join(qemu_cmd)}")

    try:
        # Start QEMU (this will block until QEMU exits)
        _ = subprocess.run(qemu_cmd, check=True)
        print("QEMU emulator started successfully")
        return 0
    except subprocess.CalledProcessError as e:
        print(f"Failed to start QEMU: {e}")
        return 1
    except FileNotFoundError:
        print("QEMU not found. Please install or build qemu-system-xtensa")
        return 1


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", build_emulator_image)  # type: ignore

# DO NOT interfere with Unity tests conducted by pio
if env["BUILD_TYPE"] != "debug, test":  # type: ignore
    # Replace the upload command with our custom function
    env.Depends(  # type: ignore
        "upload",
        [
            "$BUILD_DIR/bootloader.bin",
            "$BUILD_DIR/partitions.bin",
            "$BUILD_DIR/${PROGNAME}.bin",
        ],
    )
    env.Replace(UPLOADCMD=start_qemu_emulator)  # type: ignore
