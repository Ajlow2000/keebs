/* Copyright 2015-2021 Jack Humbert
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
    _SYMBOL,
    _NAV,
    _GAME,
};

enum tap_dances {
    COPY_PASTE = 0,
    SHIFTED_COPY_PASTE
};
tap_dance_action_t tap_dance_actions[] = {
    [COPY_PASTE] = ACTION_TAP_DANCE_DOUBLE(C(KC_V), C(KC_C)),
    [SHIFTED_COPY_PASTE] = ACTION_TAP_DANCE_DOUBLE(C(S(KC_V)),C(S(KC_C))),
};

/* Tap Dance */
#define TD_CV TD(COPY_PASTE)
#define TD_SCV TD(SHIFTED_COPY_PASTE)

/* Mod Taps */
#define MT_ESC MT(MOD_LCTL, KC_ESC)
#define MT_ENT MT(MOD_RSFT, KC_ENT)

/* Shortened Keycodes */
#define SKC_VUP KC_KB_VOLUME_UP
#define SKC_VDN KC_KB_VOLUME_DOWN

/* Layer Switching */
#define L_QWERTY TO(_QWERTY)
#define L_SYMBOL TT(_SYMBOL)
#define L_NAV TG(_NAV)
#define L_GAME TG(_GAME)

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);
const key_override_t copy_paste_override = ko_make_basic(MOD_MASK_SHIFT, TD_CV, TD_SCV);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &delete_key_override,
    &copy_paste_override,
    NULL // Null terminate the array of overrides!
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QWERTY] = LAYOUT_preonic_1x2uC(
        TD_CV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,   KC_6,  KC_7,     KC_8,    KC_9,    KC_0,    G(KC_L),
        KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,   KC_Y,  KC_U,     KC_I,    KC_O,    KC_P,    KC_MINS,
        MT_ESC,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,   KC_H,  KC_J,     KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   KC_N,  KC_M,     KC_COMM, KC_DOT,  KC_SLSH, MT_ENT,
        XXXXXXX,  XXXXXXX, KC_LGUI, KC_LALT, KC_SPC,    L_SYMBOL,    KC_SPC,   KC_BSPC, KC_DEL,  XXXXXXX, XXXXXXX
    ),

    [_SYMBOL] = LAYOUT_preonic_1x2uC(
        QK_BOOT, DB_TOGG, XXXXXXX, XXXXXXX, L_GAME, L_NAV,     SKC_VDN, SKC_VUP,  KC_BRIU, KC_BRID, RGB_TOG, KC_PSCR,
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,    KC_F7,   KC_F8,    KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, KC_GRV,  KC_LBRC, KC_RBRC, KC_MINS, KC_EQUAL, KC_LEFT, KC_DOWN,  KC_UP,   KC_RGHT, KC_BSLS, XXXXXXX,
        _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,     KC_8,    KC_9,    KC_0,    _______,
        _______, _______, _______, _______, L_NAV,     L_QWERTY,       _______,  _______, _______, _______, _______
    ),

    [_NAV] = LAYOUT_preonic_1x2uC(
        _______, _______, _______, _______, _______, _______, _______, KC_NUM,   KC_PSLS, KC_PAST, KC_PMNS, _______,
        _______, _______, KC_WH_L, KC_MS_U, KC_WH_R, KC_WH_U, _______, KC_KP_7,  KC_KP_8, KC_KP_9, KC_PPLS, _______,
        _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D, _______, KC_KP_4,  KC_KP_5, KC_KP_6, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, KC_KP_1,  KC_KP_2, KC_KP_3, _______, _______,
        _______, _______, _______, KC_BTN2, KC_BTN1,    L_QWERTY,      KC_KP_0,  _______, _______, _______,  _______
    ),

    [_GAME] = LAYOUT_preonic_1x2uC(
        KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,   KC_6,   KC_7,     KC_8,    KC_9,    KC_0,    G(KC_L),
        KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,   KC_Y,   KC_U,     KC_I,    KC_O,    KC_P,    KC_MINS,
        KC_LCTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,   KC_H,   KC_J,     KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   KC_N,   KC_M,     KC_COMM, KC_DOT,  KC_SLSH, MT_ENT,
        XXXXXXX,  XXXXXXX, XXXXXXX, KC_LCTL, KC_SPC,    L_QWERTY,     KC_SPC,   KC_BSPC, KC_DEL,  XXXXXXX, XXXXXXX
    ),
};
