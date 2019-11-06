#include "aes.h"

// I will include more testing and automated verification soon
// This will also test sea.c when that is completed

// prints boxed state arrays similar to nist diagrams for easy visual comparison
static void Print_state(state_t state, char* name){
    printf("%s\n+-------------+\n", name);
    for(int l=0; l < Nb; l++) {
        printf("| ");
        for(int i=0; i< Nk; i++){
            printf("%02X ", state[l][i]);
        }
        printf("|\n");
    }
    printf("+-------------+\n");
};

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

// AES Publication Appendix A.1 EXPANSION OF A 128-BIT CIPHER KEY
static void keyexp(){
    uint8_t key[Nk*Nb] = {
        0x2b, 0x7e, 0x15, 0x16,\
        0x28, 0xae, 0xd2, 0xa6,\
        0xab, 0xf7, 0x15, 0x88,\
        0x09, 0xcf, 0x4f, 0x3c\
    };
    word w;
    Key_expansion(key, w);
    printf("Key Expansion\n");
    printf(" 0 : ");
    for(int l = 0; l < sizeof(w); l++) {
        printf("0x%02x ", w[l]);
        if ((l+1) % 4 == 0 && l != sizeof(w)-1){
            printf("\n%2i : ", (l)/4+1);
        };
    };
    printf("\n");
};

int main() {
    keyexp();
    test_cipher();
    state_t inputc;
    char str[32];
    printf("Enter Hex Plaintext: ");
    scanf("%[^\n]", str); // scan till enter is pressed
    for(int i=0;i<32;i+=2) {
        inputc[i/2%4][i/8] = (c_h(str[i]))*16 + (c_h(str[i+1]));
    }
    Print_state(inputc,"Input");
    printf("Enter Hex Key: ");
    scanf("%s",str);
    uint8_t key[Nk*Nb];
    for(int i=0;i<32;i+=2) {
        key[i/2] = (c_h(str[i]))*16 + (c_h(str[i+1]));
    }
    Cipher(inputc, key);
    Print_state(inputc,"Output");

    printf("Enter Plaintext: ");
    scanf("%s", str);
    String_to_bytes(inputc, str);

    Print_state(inputc,"Input");
    Cipher(inputc, key);
    Print_state(inputc,"Output");
    return 1;
};
