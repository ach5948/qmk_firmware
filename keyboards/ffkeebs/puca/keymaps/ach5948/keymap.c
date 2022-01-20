/* Copyright 2021 Sleepdealer, James Young for QMK (@noroadsleft)
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

#define OLED_VAL_STEP 16

static bool _scr_state = true;

enum custom_keycodes {
    SCR_TOG = SAFE_RANGE,
    SCR_BU,
    SCR_BD
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho(
        KC_MUTE,             XXXXXXX, TG(1),
        KC_PPLS,    KC_PSLS, KC_PAST, KC_PMNS,
        KC_P7,      KC_P8,   KC_P9,   TG(1),
        KC_P4,      KC_P5,   KC_P6,   KC_PIPE,
        KC_P1,      KC_P2,   KC_P3,   KC_ENTER,
        LSFT(KC_3), KC_P0,   KC_PDOT, MO(1)
    ),
    [1] = LAYOUT_ortho(
        _______,          TG(1),   TG(2),
        MO(2),   SCR_TOG, SCR_BD,  SCR_BU,
        KC_PGUP, KC_UP,   KC_PGDN, TG(1),
        KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX,
        KC_INS,  KC_HOME, KC_END,  _______,
        KC_ESC,  XXXXXXX, KC_DEL,  XXXXXXX
    ),
    [2] = LAYOUT_ortho(
        XXXXXXX,          TG(2),   XXXXXXX,
        _______, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, RESET,   XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    )
};

bool oled_task_user(void) {
    if (!_scr_state) {
        oled_off();
        return false;
    }

    // Draw logo
    if (!draw_puca_logo()) { return false; }

    oled_write_P(PSTR("L"), false);
    // Support two digits
    char num[3];

    uint8_t layer = get_highest_layer(layer_state);

    // Show layer number without padding.
    if (layer < 10) {
        // layer is only one character.
        num[0] = (char)(layer + '0');
        num[1] = '\0';
        num[2] = '\0';
    } else if (layer < 100) {
        // layer is two digits
        // first character
        num[0] = (char)((layer / 10) + '0');
        // second character
        num[1] = (char)((layer % 10) + '0');
        num[2] = '\0';
    } else {
        // layer is huge
        num[0] = '-';
        num[1] = '-';
        num[2] = '\0';
    }
    oled_write_ln(num, false);

    // Named layers
    switch (layer) {
        case 0:
            oled_write_ln_P(PSTR("BASE"), false);
            break;
        case 1:
            oled_write_ln_P(PSTR("FUNC"), false);
            break;
        default:
            // Clear layer name
            oled_write_ln_P(PSTR("    "), false);
    }
    // Don't do keyboard oled stuff
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SCR_TOG:
            if (record->event.pressed) {
                _scr_state = !_scr_state;
            }
            return false;
        case SCR_BU:
            if (record->event.pressed) {
                uint8_t bright = oled_get_brightness();
                if (bright < (255 - OLED_VAL_STEP)) {
                    oled_set_brightness(bright + OLED_VAL_STEP);
                } else {
                    oled_set_brightness(255);
                }
            }
            return false;
        case SCR_BD:
            if (record->event.pressed) {
                uint8_t bright = oled_get_brightness();
                if (bright >= (OLED_VAL_STEP)) {
                    oled_set_brightness(bright - OLED_VAL_STEP);
                } else {
                    oled_set_brightness(0);
                }
            }
            return false;
    }
    return true;
}
