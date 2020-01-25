#include "screens.h"

// This file contains a simulation of the onboard screen to visualize and debug
// screen features and display formatting.

// Helper functions

// prints a state in hex
static void print_state(state_t state){
    char str[3];
    for(int l=0; l < Nb; l++) {
      for(int i=0; i< Nk; i++){
          sprintf(str, "%02X", state[l][i]);
          printf("%c%c ", str[0], str[1]);
      }
    }
    printf("\n");
};

// gets rid of offset on charactors (returns them as hex)
uint8_t c_h(char c) {
    if (c >= 'a' && c <= 'f') // 10-16
        return (c - 'a' + 10);
    else if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    else if (c >= '0' && c <= '9')
        return (c - '0');
    else
        return '0'; // don't accept non hex
};

// takes a string and encodes it into a state in bytes
void String_to_bytes(state_t state, char buffer[17]) {
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            state[i][j] = (uint8_t)buffer[i*4+j];
        }
    }
};

/*
    - f_* functions display the current screen
    - other print functions alter logic then call their respective f_* screen
*/
typedef void screen_function(char c);

// main menu
void f_0(char c) { // in keymap.c these will be redundant as OLED write handles screen prints
    screen = 0;
    printf("Menu\n1 Secure Entry\n2 Keyboard\n3 Logo\n4 Settings\n\n");
};

// secure entry
void f_1(char c) {
    screen = 1;
    if (hexb == 'S') {
        printf("Secure Entry (%c)\n [%s]\n%s\n\n(press send)\n", hexb, buffer, error_msg);
    } else {
        printf("Secure Entry (%c)\n [%.16s]\n [%.16s]\n%s\n\n(press send)\n", hexb, buffer, \
            &buffer[16], error_msg);
    }
};

// enter case for secure entry
void snt(char c) {
    if (hexb == 'H') {
        for(int i=0;i<32;i+=2) {
            inputc[i/2%4][i/8] = (c_h(buffer[i]))*16 + (c_h(buffer[i+1]));
            // just turns char to it's hex repr i.e. 'A' -> 11
        }
    } else {
        String_to_bytes(inputc, buffer); // encodes string as bytes
    }
    print_state(inputc); // initial state
    Cipher(inputc, ckey);
    print_state(inputc); // post encryption state
    // this will be sent as presses or a console message instead of being displayed like this
    f_1(c);
};

// buffer case for secure entry
void sbf(char c) {
    if (c == 127) { // backspace
        buffer[strlen(buffer)-1] = '\0';
    } else { // all other keys
        if ((strlen(buffer) < 16 && hexb == 'S') || (strlen(buffer) < 32 && hexb == 'H')){
            char d = (char) c;
            strncat(buffer, &d, 1);
        } else {
            if (hexb == 'S') {
                error_msg = RED "Plaintext is 16 chars" RESET;
            } else {
                error_msg = RED "Plaintext is 32 chars" RESET;
            }
        }
    }
    f_1(c);
};

// keyboard passthrough
void f_2(char c) {
    screen = 2;
    if (c != 13) { // don't pass enter for formatting errors
        printf("Keyboard\n [%c : %d]\n\n\n\n(press menu)\n", c, c); // show keycode
    } else {
        printf("Keyboard\n [\\n : 13]\n\n\n\n(press menu)\n"); // show keycode
    }
};

// logo passthrough
void f_3(char c) {
    screen = 3;
    printf(BLU "+---------------------+\n");
    printf("|            .,       |\n");
    printf("|  .      _,'f----.._ |\n");
    printf("|  |\\ ,-\'\"/  |     ,\' |\n");
    printf("|  |,_  ,--.      /   |\n");
    printf("|  /,-. ,\'`.     (_   |\n");
    printf("|  (  o|  o|__     \"`-|\n");
    printf("|  ,-._.,--\'_ `.   _.,|\n");
    printf("|  `\"\' ___.,\'` j,-\'   |\n");
    printf("|  `-.__.,--\'         |\n");
    printf("+---------------------+\n" RESET);
};

// settings menu
void f_4(char c){
    screen = 4;
    printf("Settings\n1 Hex/String (%c)\n2 Keypress/Console (%c) \n3 Change Key\n4 Test AES\n_ Reset EPROM\n"\
        , hexb, outb);
};

// toggle for Hex/String
void h_s(char c) {
    if (hexb == 'H') {
        hexb = 'S';
    } else {
        hexb = 'H';
    }
    f_4(c);
};

// key change entry
void f_5(char c) {
    screen = 5;
    printf("Change Key\n [%.16s]\n [%.16s]\n%s\n\n(press send)\n", buffer, &buffer[16], error_msg);
};

void tst(char c) {
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
    print_state(inputc);
    Cipher(inputc, ckey);
    print_state(inputc);
    f_4(c);
};
// reset EPROM
void res(char c) {
    exit(0);
};

// toggle for output Keypress/Console
void k_c(char c) {
    if (outb == 'K') {
        outb = 'C';
    } else {
        outb = 'K';
    }
    f_4(c);
};

// buffer case for key entry
void kbf(char c) {
    if (c == 127) {
        buffer[strlen(buffer)-1] = '\0';
    } else {
        if (strlen(buffer) < 32) {
            char d = (char) c;
            strncat(buffer, &d, 1);
        } else {
            error_msg = RED "Key is 32 chars" RESET;
        }
    }
    f_5(c);
};

// enter case for key entry
void knt(char c) {
    for(int i=0;i<32;i+=2) {
        ckey[i/2] = (c_h(buffer[i]))*16 + (c_h(buffer[i+1]));
    }
    error_msg = BLU "New key set" RESET;
    f_5(c);
};

screen_function *dispatch[6][6] = {
//   any   1    2    3    4   ent
    {f_0, f_1, f_2, f_3, f_4, f_0}, // main menu
    {sbf, sbf, sbf, sbf, sbf, snt}, // secure input
    {f_2, f_2, f_2, f_2, f_2, f_2}, // key passthrough
    {f_3, f_3, f_3, f_3, f_3, f_3}, // logo
    {f_4, h_s, k_c, f_5, tst, res}, // settings
    {kbf, kbf, kbf, kbf, kbf, knt}  // change key
};

int main() {
    int c = 0;
    while (1) {
        error_msg = "";
        system("clear");
        if (screen != 1 && screen != 5)
            strcpy(buffer, ""); // clear buffer when screen changes
        if (c == 68) // left key / menu key
            dispatch[0][0](c);
        else if (c >= 49 && c <= 52) // 1-4
            dispatch[screen][c-48](c);
        else if (c == 13) // enter
            dispatch[screen][5](c);
        else // any other key
            dispatch[screen][0](c);
        // For the final product this will be called as a callback on new keypress instead of in a loop
        system("/bin/stty raw");
        // https://stackoverflow.com/questions/32781937/getting-a-single-character-without-pressing-enter
        c = getchar();
        system("/bin/stty cooked");
    }
};
