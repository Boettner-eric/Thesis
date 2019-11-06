# Software
* This section of the repo contains all the necessary code to run Key Crypt
* Makefile compiles binaries for testing, screen simulation and QMK keymaps

## File Hierarchy
* Makefile
* Keymap.c  (a qmk keymap file)
* aes.c (an implementation of AES in c)
* aes.h
* sea.c (AES decryption)
* sea.h
* screens.c (a console that mimics the oled of the keyboard)
* tests.c (testing file for AES verification)
* Console applications / hid listener

## AES
* All encryption and decryption is handled in aes.c and sea.c.
* Code structure attempts to follow nist conventions and includes all the defined functions used for AES.
* Key expansion occurs inside of Cipher to streamline encryption for screens.c and keymap.c
* The header files aes.h and sea.h only contain references to the Cipher wrapper functions and not AES sub-functions for security.

```c
// AES' defined cipher code
void Cipher(state_t state, uint8_t* key) {
    word w;
    Key_expansion(key, w);
    Add_Round_Key(0, state, w);
    for (int i=1; i<Nr; i++) {
        Sub_bytes(state);
        ShiftRows(state);
        MixColumns(state);
        Add_Round_Key(i, state, w);
    }
    Sub_bytes(state);
    ShiftRows(state);
    Add_Round_Key(Nr, state, w);
};
```

## Screens
* A simple console application that simulates the function of the onboard oled
* Screens are tied to different logic with keyboard passing input straight to the os while Secure Entry doesn't pass input and instead saves keystrokes to a buffer to encrypt and send later.
* Key changes and secure entry buffers are handled on board and not on the host os/

```c
Menu
1 Secure Entry
2 Keyboard
3 Logo
4 Settings
```

## Testing
* A simple test file for verifying the accuracy of aes.c and sea.c
* Compiles into a testing binary that automates the effect of aes.c on given test arrays.
* Will be expaned with more testing and verification.

```c
// AES Publication Appendix B Cipher Example
// in   3243f6a8885a308d313198a2e0370734
// key  2b7e151628aed2a6abf7158809cf4f3c
void test_cipher() {
    state_t inputc = {
        {0x32, 0x88, 0x31, 0xe0},
        {0x43, 0x5a, 0x31, 0x37},
        {0xf6, 0x30, 0x98, 0x07},
        {0xa8, 0x8d, 0xa2, 0x34}
    };
    uint8_t ckey[Nk*Nb] = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c
    };
    Print_state(inputc,"Input");
    Cipher(inputc, ckey);
    Print_state(inputc,"Output");
};
```
* I implemented the examples used by nist to show correctness (testing will be expanded and automated soon)

## QMK (in progress)
* keymap.c will contain all the code necessary to compile a hex file for the Keyboard
* keymap.c cannot be compiled using the normal Makefile and instead needs to be placed inside the keyboard folder in qmk.
* Further compilation instructions will follow
