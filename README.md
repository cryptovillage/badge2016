# cpvdcbadge

## Flashing Instructions

    lsusb
Find the Atmega device in the list. If it's already been flashed, it will show up as "dc24:1337". To get it into a mode to reflash it, follow the directions in the "Reflashing" section.

    sudo chown 666 /dev/bus/usb/02/020
    dfu-programmer atmega32u2 erase --force
    dfu-programmer atmega32u2 flash --suppress-bootloader-mem blinkybadge-test.hex

## Reflashing
Run the "reset_badge.py" script to temporarily enable flashing.

    python reset_badge.py
