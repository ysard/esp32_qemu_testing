#**********************************************************************************
# Copyright (C) 2025 Craig Setera
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/.
#*********************************************************************************/
Import("env") # type: ignore
import subprocess

def build_emulator_image(output_path):
    """Build a flash image for use in the emulator"""
    # print(env.Dump())

    command = [ 
        env.File('$PROJECT_PACKAGES_DIR//tool-esptoolpy/esptool.py').abspath,  # type: ignore
        '--chip', env['BOARD_MCU'], # type: ignore
        'merge_bin',
        '-o', output_path,
        '--fill-flash-size', '4MB',
        '--flash_size', 'keep',
        '--flash_mode', env['BOARD_FLASH_MODE'], # type: ignore
        '--flash_freq', '40m',
        '0x1000', env.File('$BUILD_DIR/bootloader.bin').abspath, # type: ignore
        '0x8000', env.File('$BUILD_DIR/partitions.bin').abspath, # type: ignore
        '0x10000', env.File('$BUILD_DIR/firmware.bin').abspath, # type: ignore
    ]
 
    result = subprocess.run(command, capture_output=True)
    if result.returncode != 0:
        print(result.stdout)
        return False
    else:
        print("Image built")
        return True


def start_qemu_emulator(source, target, env):
    """Custom upload function that starts ESP32 QEMU instead of flashing"""
    
    # Get the compiled firmware path
    flash_image = str(source[0])
    if not build_emulator_image(flash_image):
        return
    
    # QEMU command and arguments
    qemu_cmd = [
        'qemu-system-xtensa',
        '-machine', 'esp32',
        "-drive", f"file={flash_image},if=mtd,format=raw",
        '-display', 'gtk',
        '-serial', 'stdio',
        '-d', 'guest_errors',
        '-s', '-S'
   ]
    
    print(f"Starting ESP32 QEMU emulator...")
    print(f"Command: {' '.join(qemu_cmd)}")
    
    try:
        # Start QEMU (this will block until QEMU exits)
        result = subprocess.run(qemu_cmd, check=True)
        print("QEMU emulator started successfully")
        return 0
    except subprocess.CalledProcessError as e:
        print(f"Failed to start QEMU: {e}")
        return 1
    except FileNotFoundError:
        print("QEMU not found. Please install or build qemu-system-xtensa")
        return 1

# Replace the upload command with our custom function
env.Depends("upload", ["$BUILD_DIR/bootloader.bin", "$BUILD_DIR/partitions.bin", "$BUILD_DIR/${PROGNAME}.bin"])   # type: ignore
env.Replace(UPLOADCMD=start_qemu_emulator)   # type: ignore
