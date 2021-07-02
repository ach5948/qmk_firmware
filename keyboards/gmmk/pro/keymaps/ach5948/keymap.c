/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12      Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)      BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             PgDn
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the RESET key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.

    // Base Layer
    [0] = LAYOUT(
     // ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12      Prt                Rotary(Mute)
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,            KC_MUTE,
     // ~        1        2        3        4        5        6        7        8        9        0         -       (=)      BackSpc            Del
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,           KC_HOME,
     // Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                  PgUp
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,           KC_PGUP,
     // Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter              PgDn
        MO(1),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,            KC_PGDN,
     // Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up        End
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,    KC_END,
     // Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down      Right
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN,  KC_RGHT
    ),

    // Base Mod Layer
    [1] = LAYOUT(
     // ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12      Prt                Rotary(Mute)
        _______, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_PSCR,           _______,
     // ~        1        2        3        4        5        6        7        8        9        0         -       (=)      BackSpc            Del
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_CALC,           _______,
     // Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                  PgUp
        _______, RGB_TOG, RGB_HUI, RGB_MOD, RESET,   RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______,           _______,
     // Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter              PgDn
        _______, RGB_SAD, RGB_HUD, RGB_SAI, _______, _______, _______, _______, _______, _______, _______, _______,          _______,           _______,
     // Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up        End
        _______,          _______, _______, _______, _______, _______, KC_MPRV, KC_MNXT, _______, _______, _______,          _______, RGB_VAI,  _______,
     // Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down      Right
        _______, _______, _______,                            KC_MPLY,                            _______, _______, _______, RGB_SPD, RGB_VAD, RGB_SPI
    ),
};


bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
      tap_code(KC_VOLU);
    } else {
      tap_code(KC_VOLD);
    }
    return true;
}

// TODO Move to config
//#define LED_LAYER_FUNCTION

static void change_layer_leds(uint8_t on_index, uint8_t on_modifier){
#ifdef LED_LAYER_FUNCTION
                             //  F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10, F11, F12
    const uint8_t keys[] = {   6,  12,  18,  23,  28,  34,  39,  44,  50,  56,  61,  66};
    const uint8_t max_index = 12;
#else

    //const int keys[] = { 72, 75, 86, 82 };
    const uint8_t keys[] = { 82, 86, 75, 72 };
    const uint8_t max_index = 4;
#endif
    uint8_t val = rgb_matrix_get_val();
    HSV red =   {   0, 255, val };
    HSV blue =  { 169, 255, val };
    HSV green = {  85, 255, val };
    HSV modifiers[] = {red, blue, green};
    const uint8_t max_mod = 3;
    if(on_modifier >= max_mod){
        on_modifier = max_mod - 1;
    }
    for(uint8_t i = 0; i < max_index; i++){
        if(i == (on_index - 1)){
            // TODO Use user settings for brightness, etc
            rgb_matrix_set_color(
                    keys[i],
                    hsv_to_rgb(modifiers[on_modifier]).r,
                    hsv_to_rgb(modifiers[on_modifier]).g,
                    hsv_to_rgb(modifiers[on_modifier]).b
            );
        } else {
            rgb_matrix_set_color(keys[i], 0, 0, 0);
        }
    }
    return;
}

void rgb_matrix_indicators_user(void) {
    switch (biton32(layer_state)) {
        case 0:
            // base layer
            change_layer_leds(1, 0);
            break;
        case 1:
            // mod layer for base layer
            change_layer_leds(1, 0);
            break;
        default:
            break;
    }
    return;
}
