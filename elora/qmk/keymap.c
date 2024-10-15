/* Copyright 2023 splitkb.com <support@splitkb.com>
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

enum layers {
    _QWERTY,
    _SYM,
    _NAV,
    _NUM,
    _GAME_OVERLAY,
    _GRAPHITE,
};

enum tap_dances {
    COPY_PASTE = 0,
};

tap_dance_action_t tap_dance_actions[] = {
    [COPY_PASTE] = ACTION_TAP_DANCE_DOUBLE(C(KC_V), C(KC_C)),
};

/* Tap Dance */
#define TD_CV TD(COPY_PASTE)

/* Mod Taps */
#define CTL_ESC MT(MOD_LCTL, KC_ESC)
#define CTL_ENT MT(MOD_RCTL, KC_ENT)
#define ALT_ENT  MT(MOD_LALT, KC_ENT)
#define SFT_SLH  MT(MOD_RSFT, KC_SLSH)

/* Layer stuff */
#define DF_Q DF(_QWERTY)
#define DF_G DF(_GRAPHITE)
#define L_SYM TT(_SYM)
#define L_NAV TT(_NAV)
#define NUM_SPC LT(_NUM, KC_SPC)
#define GAME TG(_GAME_OVERLAY)

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QWERTY] = LAYOUT_myr(
        TD_CV,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                XXXXXXX, XXXXXXX,          KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    G(KC_L),
        KC_TAB , KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                XXXXXXX, XXXXXXX,          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_TAB,
        CTL_ESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                XXXXXXX, XXXXXXX,          KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, CTL_ENT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    C(KC_LSFT), KC_F5,   KC_F11,  KC_DEL,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SFT_SLH,
                                   XXXXXXX, ALT_ENT, KC_SPC,  L_SYM,      KC_LGUI, KC_TAB,  L_NAV,   NUM_SPC, KC_BSPC, XXXXXXX,

        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX
    ),

    [_GRAPHITE] = LAYOUT_myr(
        TD_CV,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                XXXXXXX, XXXXXXX,          KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    G(KC_L),
        KC_TAB , KC_B,    KC_L,    KC_D,    KC_W,    KC_Z,                XXXXXXX, XXXXXXX,          KC_QUOT, KC_F,    KC_O,    KC_U,    KC_J,    KC_TAB,
        CTL_ESC, KC_N,    KC_R,    KC_T,    KC_S,    KC_G,                XXXXXXX, XXXXXXX,          KC_Y,    KC_H,    KC_A,    KC_E,    KC_I,    CTL_ENT,
        KC_LSFT, KC_Q,    KC_X,    KC_M,    KC_C,    KC_V,    C(KC_LSFT), KC_F5,   KC_F11,  KC_DEL,  KC_K,    KC_P,    KC_DOT,  KC_MINS, KC_COMM, SFT_SLH,
                                   XXXXXXX, ALT_ENT, KC_SPC,  L_SYM,      KC_LGUI, KC_TAB,  L_NAV,   NUM_SPC, KC_BSPC, XXXXXXX,

        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,                            XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX
    ),

    [_SYM] = LAYOUT_myr(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            XXXXXXX, XXXXXXX,          KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, KC_BSLS, KC_QUOT, KC_DQUO, KC_PERC, KC_QUES,          XXXXXXX, XXXXXXX,          KC_EXLM, KC_RBRC, KC_LBRC, KC_GRV,  KC_COLN, _______,
        _______, KC_CIRC, KC_EQL,  KC_UNDS, KC_DLR,  KC_ASTR,          XXXXXXX, XXXXXXX,          KC_AT,   KC_RCBR, KC_LCBR, KC_TILD, KC_SCLN, _______,
        _______, KC_LT,   KC_PIPE, KC_MINS, KC_GT,   KC_SLSH, _______, _______, _______, _______, KC_PLUS, KC_RPRN, KC_LPRN, KC_HASH, KC_DOT,  _______,
                                   XXXXXXX, _______, _______, TG(_SYM),_______, KC_COMM, KC_DOT,  _______, KC_DEL,  XXXXXXX,

        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,                             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX
    ),

    [_NAV] = LAYOUT_myr(
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
        _______, QK_BOOT, XXXXXXX, KC_MS_U, XXXXXXX, DF_G,             XXXXXXX, XXXXXXX,           KC_MUTE, KC_VOLD, KC_VOLU, XXXXXXX, QK_BOOT, _______,
        _______, DF_Q,    KC_MS_L, KC_MS_D, KC_MS_R, GAME,             XXXXXXX, XXXXXXX,           KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, G(KC_L), _______,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______, _______, _______,  XXXXXXX, KC_BRID, KC_BRIU, XXXXXXX, XXXXXXX, _______,
                                   XXXXXXX, _______, KC_BTN1, KC_BTN2, _______, _______, TG(_NAV), _______, KC_DEL,  XXXXXXX,

        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,                             XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX
    ),

    [_NUM] = LAYOUT_myr(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        _______, KC_F1,   KC_F4,   KC_F7,   KC_F10,  KC_F13,           XXXXXXX, XXXXXXX,           KC_SLSH, KC_7,    KC_8,    KC_9,    KC_MINS, _______,
        _______, KC_F2,   KC_F5,   KC_F8,   KC_F11,  KC_F14,           XXXXXXX, XXXXXXX,           KC_COLN, KC_4,    KC_5,    KC_6,    KC_PLUS, _______,
        _______, KC_F3,   KC_F6,   KC_F9,   KC_F12,  KC_F15,  _______, _______, _______, _______,  KC_DOT,  KC_1,    KC_2,    KC_3,    KC_0,    _______,
                                   XXXXXXX, _______, _______, TG(_NUM),_______, _______, TG(_NUM), _______, _______, XXXXXXX,

        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,                              XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX
    ),

    [_GAME_OVERLAY] = LAYOUT_myr(
        KC_ESC,  _______, _______, _______, _______, _______,          XXXXXXX, XXXXXXX,          _______, _______, _______, _______, _______, KC_BSPC,
        KC_TAB,  _______, _______, _______, _______, _______,          XXXXXXX, XXXXXXX,          _______, _______, _______, _______, _______, KC_ESC,
        KC_LCTL, _______, _______, _______, _______, _______,          XXXXXXX, XXXXXXX,          _______, _______, _______, _______, _______, KC_ENT,
        KC_LSFT, _______, _______, _______, _______, _______, KC_LALT, KC_F1,   KC_F2,   KC_F3,   _______, _______, _______, _______, _______, KC_ENT,
                                   XXXXXXX, KC_LCTL, KC_SPC,  L_SYM,   KC_LGUI, KC_F4,   L_NAV,   NUM_SPC, KC_BSPC, XXXXXXX,

        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,                              XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX
    ),

    // /*
    //  * Layer template - LAYOUT_myr
    //  *
    //  * ,-------------------------------------------.      ,------.  ,------.      ,-------------------------------------------.
    //  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
    //  * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
    //  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
    //  * |--------+------+------+------+------+------|      |------|  |------|      |------+------+------+------+------+--------|
    //  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
    //  * |--------+------+------+------+------+------+------+------|  |------|------+------+------+------+------+------+--------|
    //  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
    //  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
    //  *                        |      |      |      |      |      |  |      |      |      |      |      |
    //  *                        |      |      |      |      |      |  |      |      |      |      |      |
    //  *                        `----------------------------------'  `----------------------------------'
    //  *
    //  * ,-----------------------------.      ,------.                ,---------------------------.      ,------.
    //  * |        |      |      |      |      |      |                |      |      |      |      |      |      |
    //  * `-----------------------------'      `------'                `---------------------------'      '------'
    //  */
    //     [_LAYERINDEX] = LAYOUT_myr(
    //       _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______,
    //       _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______,
    //       _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______,
    //       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //
    //       _______, _______, _______, _______,          _______,                   _______, _______, _______, _______,          _______
    //     ),
};

/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/elora/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */

/*
#ifdef OLED_ENABLE
bool oled_task_user(void) {
  // Your code goes here
}
#endif
*/


#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    // Left side
    if (index == 0 || index == 1 || index == 2) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
        // Right side
    } else if (index == 4 || index == 5 || index == 6) {
        if (clockwise) {
            tap_code(KC_BRIU);
        } else {
            tap_code(KC_BRID);
        }
        // Myriad
    } else if (index == 3 || index == 7) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }

    // if (IS_LAYER_ON(_GAME_OVERLAY)) {
    // } else if (IS_LAYER_ON(_SYM)) {
    // } else if (IS_LAYER_ON(_NAV)) {
    // } else if (IS_LAYER_ON(_NUM)) {
    // } else { // ALPHA Layers
    //     // Left side
    //     if (index == 0 || index == 1 || index == 2) {
    //         if (clockwise) {
    //             tap_code(KC_VOLU);
    //         } else {
    //             tap_code(KC_VOLD);
    //         }
    //     // Right side
    //     } else if (index == 4 || index == 5 || index == 6) {
    //         if (clockwise) {
    //             tap_code(KC_PGUP);
    //         } else {
    //             tap_code(KC_PGDN);
    //         }
    //     // Myriad
    //     } else if (index == 3 || index == 7) {
    //         if (clockwise) {
    //             tap_code(KC_VOLU);
    //         } else {
    //             tap_code(KC_VOLD);
    //         }
    //     }
    //}

    return true;
}
#endif
