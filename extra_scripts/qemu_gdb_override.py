#**********************************************************************************
# Copyright (C) 2025 Craig Setera
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/.
#*********************************************************************************/
import os
Import("env") # type: ignore

# Determine if there is another version of the xtensa-esp32-elf-gdb
# that should be used instead of the default
initial_gdb = env['GDB'] # type: ignore
override_gdb = os.environ['XTENSA_ESP32_ELF_GBD_OVERRIDE']
if (initial_gdb.endswith('xtensa-esp32-elf-gdb') and (override_gdb is not None)):
    env.Replace(GDB = override_gdb)  # type: ignore
