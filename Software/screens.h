#include "aes.h"

// color printing won't work on an oled screen but is here for clarity
#define RESET "\x1B[0m"
#define RED   "\x1B[31m"
#define BLU   "\x1B[34m"

// AES variables
uint8_t ckey[Nk*Nb] = {
    0x2b, 0x7e, 0x15, 0x16,
    0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88,
    0x09, 0xcf, 0x4f, 0x3c
};
state_t inputc;

// Global settings
int screen = 5;  // start on change key screen
char hexb = 'S'; // default to string inputs
char outb = 'C'; // default to console printing
char buffer[33]; // string buffer for secure/key entry
char* error_msg; // string buffer for console messages
