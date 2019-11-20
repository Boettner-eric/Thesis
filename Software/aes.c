#include "aes.h"

// Adapted from the AES standard documentation and algorithm specification

// substitutes a four byte sequence with each byte's sbox counterpart
static void Sub_word(uint8_t* temp){
    for (int i = 0; i < 4; ++i) {
        temp[i] = sbox[temp[i]];
    }
};

// shifts all byte indices by one (wraps 3-0)
static void Rot_word(uint8_t* temp){
    uint8_t tmp = temp[0];
    for (int i = 0; i < 3; ++i) {
        temp[i] = temp[i+1];
    }
    temp[3] = tmp;
};

// turns a key into a word following AES key expansion
void Key_expansion(uint8_t* key, word w) {
    uint8_t temp[4];
    int i, j, k;
    for (i = 0; i < Nk; ++i) {
        // w[i] = word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3])
        j = i * 4;
        w[j] = key[j];
        w[j+1] = key[j+1];
        w[j+2] = key[j+2];
        w[j+3] = key[j+3];
    };
    for (i = Nk; i < Nb*(Nr+1); i++) {
        // temp = w[i-1]
        k = (i - 1) * 4; // 4 offset bc this sets k through (k+3)
        temp[0] = w[k];
        temp[1] = w[k+1];
        temp[2] = w[k+2];
        temp[3] = w[k+3];
        if (i % Nk == 0) {
            Rot_word(temp);
            Sub_word(temp);
            temp[0] ^= rcon[i / Nk];
        };
        // w[i] = w[i-Nk] xor temp
        k = (i - Nk) * 4;
        w[i*4] = w[k] ^ temp[0];
        w[i*4+1] = w[k+1] ^ temp[1];
        w[i*4+2] = w[k+2] ^ temp[2];
        w[i*4+3] = w[k+3] ^ temp[3];
    };
};

// substitutes each byte for its corresponding sbox byte
static void Sub_bytes(state_t state) {
  for (int i=0; i<Nk; i++) {
    for (int j=0; j<Nb; j++) {
        state[i][j] = sbox[state[i][j]];
    };
  };
};

// shifts each row as according to the algorithm
static void ShiftRows(state_t state) {
  uint8_t tmp = state[1][0];
  state[1][0] = state[1][1];
  state[1][1] = state[1][2];
  state[1][2] = state[1][3];
  state[1][3] = tmp;

  tmp = state[2][0]; //two in place swaps
  uint8_t tmpt = state[2][1];
  state[2][0] = state[2][2];
  state[2][1] = state[2][3];
  state[2][2] = tmp;
  state[2][3] = tmpt;

  tmp = state[3][0];
  state[3][0] = state[3][3];
  state[3][3] = state[3][2];
  state[3][2] = state[3][1];
  state[3][1] = tmp;
};

// uses xtime macro to do GF(2^8) field multiplication
static void MixColumns(state_t state) {
    uint8_t zero, base;
    for (int i=0; i<Nk; i++) {
        zero = state[0][i];
        base = zero ^ state[1][i] ^ state[2][i] ^ state[3][i];
        state[0][i] ^= (xtime(zero ^ state[1][i])) ^ base;
        state[1][i] ^= (xtime(state[1][i] ^ state[2][i])) ^ base;
        state[2][i] ^= (xtime(state[2][i] ^ state[3][i])) ^ base;
        state[3][i] ^= (xtime(state[3][i] ^ zero)) ^ base;
    }; // see nist (5.6) for definitions
};

// XORs each element of the state with a corresponding word
static void Add_Round_Key(uint8_t round, state_t state, word w) {
    for (int j=0; j<Nk; j++){
        for (int i=0; i<Nb; i++){
              state[i][j] ^= w[(round*16)+(j*Nb)+i];
        }
    }
};

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

// Helper functions that are shared between test.c, screens.c and keymap.c

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
