# Key Crypt

* A secure input device built in QMK using AES encryption.
* This keyboard is an attempt to close a large vulnerability in secure text input by encrypting keystrokes before they enter a host operating system.

![Keyboard](hardware/layout.png)

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
    * keymap.c  (a qmk keymap file)
    * aes.c (an implementation of AES in c)
    * aes.h
    * sea.c (in progress)
    * sea.h
    * screens.c (a console that mimics the oled of the keyboard)
    * tests.c (testing file for AES verification)
    * Console applications / hid listeners (in progress)

## Schedule
* This project is still in active development and is not going to be completed till may.
*  I will be posting incremental updates as I make progress, but large portions of code and design might change in the interim.
* My goal is to make this project easily replicable with both software and hardware open sourced with clear instructions.

## Credits
See bibliography for more comprehensive listings and citations
* Eric Roberts - for advising me, providing feedback and guidance
* Jay Ewing - for help with hardware design, planning and assembly
* NIST - for clear and helpful documentation for AES encryption and decryption
* QMK - for creating a strong open source foundation for keyboard programming
* PDXKBC - for providing feedback on design and usability

## License
[MIT](https://choosealicense.com/licenses/mit/)
