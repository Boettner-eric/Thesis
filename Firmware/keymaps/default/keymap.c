/* Copyright 2019 Eric-Boettner
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "key_crypt.h"

char output_method = 'D';
int screen = 0;
int curr = 0;
int k_curr = 0;
char buffer[17];
char k_buffer[33];

// takes a string and encodes it into a state in bytes
void String_to_bytes(state_t state, char buffer[17]) {
    //uprintf("buffer: %s\n", buffer);
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            state[j][i] = (int) (buffer[i*4+j]);
        }
    }
}

// takes a state in bytes and encodes it into a string to print
void Print_state(state_t state) {
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            if (output_method == 'K') {
                char hex_b[3];
                sprintf(hex_b,"%02X", state[j][i]);
                send_string(hex_b);
            } else {
                uprintf("%02X", state[j][i]);
            }
        }
    }
    uprintf("\n");
}

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
}

// From process_terminal.c
__attribute__((weak)) const char keycode_to_ascii_lut[58] = {0, 0, 0, 0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',\
 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7',\
 '8', '9', '0', 0, 0, 0, '\t', ' ', '-', '=', '[', ']', '\\', 0, ';', '\'', '`', ',', '.', '/'};
// 0x0000 0x00FF = keycodes
__attribute__((weak)) const char shifted_keycode_to_ascii_lut[58] = {0, 0, 0, 0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',\
 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '@', '#', '$', '%', '^', '&', '*', \
 '(', ')', 0, 0, 0, '\t', ' ', '_', '+', '{', '}', '|', 0, ':', '\'', '~', '<', '>', '?'};

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    MENU = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_MINUS,   KC_1,     KC_2,    KC_3,    KC_4,    KC_5,                 KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_PLUS,
        KC_GESC,     KC_Q,     KC_W,    KC_E,    KC_R,    KC_T,                 KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_TAB,     KC_A,     KC_S,    KC_D,    KC_F,    KC_G,                 KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT,    KC_Z,     KC_X,    KC_C,    KC_V,    KC_B,                 KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
        RESET,  KC_LCTRL, KC_LALT, KC_RGUI,  KC_RGUI,  KC_SPC,  KC_ENT, MENU,  KC_SPC, KC_BSPC,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT // two extra keys in middle
    ),
};

static void render_logo(void) {
  static const char PROGMEM qmk_logo[] = {
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

  oled_write(qmk_logo, false);
};

// default key for secure entry
uint8_t ckey[Nk*Nb] = {
    0x2b, 0x7e, 0x15, 0x16,
    0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88,
    0x09, 0xcf, 0x4f, 0x3c
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    state_t inputc;
    if (keycode == MENU && record->event.pressed) {
        screen = 0;
        return false;
    }
    // handle keystrokes based on the current screen
    switch (screen) {
        case 0: // main menu
            // "Menu\n1 Secure Entry\n2 Keyboard\n3 Logo\n4 Settings\n\n\n"
            if (record->event.pressed){
                switch (keycode) {
                    case KC_1:
                        screen = 1;
                        return false;
                    case KC_2:
                        screen = 2;
                        return false;
                    case KC_3:
                        screen = 3;
                        return false;
                    case KC_4:
                        screen = 4;
                        return false;
                }
            }
            break;
        case 1:// secure entry screen
            if (keycode == KC_BSPC && record->event.pressed) { // backspace
                buffer[curr-1] = '\0';
                curr -= 1;
            } else if (keycode == KC_ENT && record->event.pressed) { // send buffer through encryption
                String_to_bytes(inputc, buffer);
                Cipher(inputc, ckey);
                Print_state(inputc);
                memset(buffer, 0, 16);
                curr = 0;
                screen = 1;
            } else if (keycode <= 58 && record->event.pressed && curr <= 17) {
                char char_to_add = 0;
                if (get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) {
                    char_to_add = shifted_keycode_to_ascii_lut[keycode];
                } else if (get_mods() == 0) {
                    char_to_add = keycode_to_ascii_lut[keycode];
                }
                if (char_to_add != 0) {
                    strncat(buffer, &char_to_add, 1);
                    curr += 1;
                }
            }
            return false;
            break;
        // 2 and 3 are pass through modes
        case 4: // settings
            // Settings\n1Keypress/Debug (%c)\n2 Change Key\n3 Test AES\n4 Reset EPROM
            if (keycode == KC_1 && record->event.pressed) {
                output_method == 'D' ? (output_method = 'K') : (output_method = 'D');
                return false;
            } else if (keycode == KC_2 && record->event.pressed) {
                screen = 5;
                return false;
            } else if (keycode == KC_3 && record->event.pressed) {
                char test[17] = "this is a test !";
                String_to_bytes(inputc, test);
                Cipher(inputc, ckey);
                Print_state(inputc);
                return false;
            } else if (keycode == KC_4 && record->event.pressed) {
                screen = 6;
                reset_keyboard();
            }
            break;
        case 5: // key entry
            if (keycode == KC_BSPC && record->event.pressed) {
                k_buffer[k_curr-1] = '\0'; // erase last char
                k_curr -= 1;
                return false;
            } else if (keycode == KC_ENT && record->event.pressed) {
                for(int i=0;i<32;i+=2) {
                    ckey[i/2] = (c_h(k_buffer[i]))*16 + (c_h(k_buffer[i+1]));
                    uprintf("%02X ", ckey[i/2]);
                }
                uprintf("\n");
                memset(k_buffer, 0, 32);
                k_curr = 0;
                screen = 0;
                return false;
            } else if (keycode <= 58 && record->event.pressed) {
                char char_to_add = 0;
                if (get_mods() & (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))) {
                    char_to_add = shifted_keycode_to_ascii_lut[keycode];
                } else if (get_mods() == 0) {
                    char_to_add = keycode_to_ascii_lut[keycode];
                }
                if (char_to_add != 0 && strlen(k_buffer) < 32) {
                    strncat(k_buffer, &char_to_add, 1);
                    k_curr += 1;
                }
                return false; // Skip all further processing of this key
            }
            break;
    }
    return true;
}

void encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        tap_code(KC_1);
    } else {
        tap_code(KC_2);
    }
}

#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) {
    char screen_buffer[200];
    switch (screen) {
        case 0:
            snprintf(screen_buffer, 200, "Menu\n \n1. Secure Entry\n2. Keyboard\n3. Logo\n4. Settings\n\n\n");
            break;
        case 1:
            snprintf(screen_buffer, 200, "Secure Entry\n\n [%s]\n\n\n(press enter)\n", buffer);
            break;
        case 2:
            snprintf(screen_buffer, 200, "\nKeyboard Mode\n\n\n\n\n");
            break;
        case 3:
            snprintf(screen_buffer, 200, "\nKey_Crypt Secure\nBy Eric Boettner\n");
            render_logo();
            break;
        case 4:
            snprintf(screen_buffer, 200, "Settings\n \n1. Keypress/Debug (%c)2. Change Key\n3. Test AES\n4. Reset EPROM\n", output_method);
            break;
        case 5:
            snprintf(screen_buffer, 200, "Change Key\n [%.16s]\n [%.16s]\n\n\n(press send)\n", k_buffer, &k_buffer[16]);
            break;
        case 6:
            snprintf(screen_buffer, 200, "DFU MODE\nReflash Firmware\n\n\n");
            break;
        default:
            render_logo();
            break;
    }
    const char *dirty = screen_buffer;
    oled_write(dirty, false);
}
#endif
