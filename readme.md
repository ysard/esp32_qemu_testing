## Summary

Example project for testing [ESP32 QEMU](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/tools/qemu.html) emulator functionality.  Uses the built-in [RGB panel](https://components.espressif.com/components/espressif/esp_lcd_qemu_rgb/versions/1.0.2/examples/lcd_qemu_rgb_panel?language=en) to simulate a [Neomatrix panel](https://github.com/adafruit/Adafruit_NeoMatrix).  The primary goal is to use this for further development on my [Pixelli](https://github.com/csetera/pixelli) project.

**NOTE: The variety of references linked in this documentation have conflicting information in them.  Keep that in mind.**

## Compiling QEMU

The emulator must be built from the [Espressif repository fork](https://github.com/espressif/qemu).  On Ubuntu 25.04, a combination of documentation was used in order to get the emulator built.  There were likely other libraries not noted that needed to be installed as well, but this gives a starting point.

* https://github.com/espressif/esp-toolchain-docs/tree/main/qemu/esp32
* https://alexconesa.wordpress.com/2025/01/17/speeding-up-esp32-embedded-system-testing-with-qemu-on-ubuntu/
* https://wiki.qemu.org/Hosts/Linux#Building_QEMU_for_Linux


```
sudo apt-get install libgtk-3-dev libpixman-1-dev libsdl2-dev

./configure --target-list=xtensa-softmmu \
    --enable-gcrypt \
    --enable-slirp \
    --enable-debug \
    --enable-sdl \
    --disable-strip --disable-user \
    --disable-capstone --enable-vnc \
    --enable-gtk
ninja -C build
```

## Building for Emulator

In this example project, the upload target has been overriden via [qemu_upload.py](extra-scripts/qemu_upload.py) to build a flash image and launch the emulator.

More info:
* https://github.com/espressif/esp-toolchain-docs/blob/main/gcc/build-and-run-native-app.md
* https://github.com/espressif/esp-toolchain-docs/tree/main/qemu/esp32#compiling-the-esp-idf-program-to-emulate


## Debugging

The standard Espressif Arduino support has ended.  The [pioarduino fork](https://github.com/pioarduino/platform-espressif32) has continued the process of building the Arduino framework around more recent espressif versions.  To get the proper emulator, you will need to set up the project to look similar to the following:

```
[esp32]
  framework = arduino
  platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
```

As it stands, "PIO Debug (without uploading)" works as long as the emulator has already been launched.

## Networking
TBD

## Miscellaneous

* https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html
* [IDF Monitor](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/tools/idf-monitor.html)
