# Software
* This section of the repo contains all the necessary code to run Key Crypt
* Makefile compiles binaries for testing, screen simulation and QMK keymaps

## File Hierarchy
* Makefile
* Keymap.c  (a qmk keymap file)
* aes.c (an implementation of AES in c)
* aes.h
* screens.c (a console that mimics the oled of the keyboard)
* tests.py (testing file for AES verification)
* Console applications / hid listener

## AES
* All encryption and decryption is handled in aes.c
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
* Will be expanded with more testing and verification.

```python
# AES Publication Appendix B Cipher Example
# in   3243f6a8885a308d313198a2e0370734
# key  2b7e151628aed2a6abf7158809cf4f3c
def test_sample(self):
    known = bytearray(b'9%\x84\x1d\x02\xdc\t\xfb\xdc\x11\x85\x97\x19j\x0b2')
    c, py = compare(self.txt, self.key)
    assert c == py == known, "Comparison failed for sample"
```
* I implemented the examples used by nist to show correctness

## QMK (in progress)
* Keyboard will be ported to QMK and compilation will follow standard syntax
* Compilation via:
```bash
    make key_crypt:default:dfu-util
````
* Further compilation instructions will follow
