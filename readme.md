# Key Crypt

* A demonstration of Onboard Keystroke Encryption
* This keyboard is an attempt to close a large vulnerability in secure text input by encrypting keystrokes before they enter a host operating system.

![Keyboard](https://github.com/Boettner-eric/Thesis/blob/master/Hardware/layout.png)

## Features
* Secure input and local encryption on the keyboard's processor.
* No driver software needed, all work is done locally in QMK.
* Clear visual menus and settings onboard.
* Sample console software for live decryption.

## Repository organization
* Hardware
    * Design files for top plate and case
    * Wiring diagram
    * Key layout editor data
    * Hardware assembly instructions and photos
* Software
    * Makefile
    * aes.c (an implementation of AES in c)
    * aes.h
    * screens.c (a console that mimics the oled of the keyboard)
    * screens.h
    * tests.py (testing file for AES verification)
    * translate.py
    * demo.py (simple decryption app)
    * Console applications / hid listeners (in progress)
* Firmware
    * keymaps/default
        * config.h
        * keymap.c
        * readme.md
    * aes.c
    * aes.h
    * config.h
    * key_crypt.c
    * key_crypt.h
    * info.json
    * readme.md
    * rules.mk

## Credits
See bibliography for more comprehensive listings and citations
* Eric Roberts - for advising me, providing feedback and guidance
* Jay Ewing - for help with hardware design, planning and assembly
* NIST - for clear and helpful documentation for AES encryption and decryption
* QMK - for creating a strong open source foundation for keyboard programming
* PDXKBC - for providing feedback on design and usability

## License
[MIT](https://choosealicense.com/licenses/mit/)
