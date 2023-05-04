/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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

// * Layers
enum layers {
    _COLEMAK_DHK = 0,
    _QWERTY,
    _NAV,
    _SYM,
    _FUNCTION,
    _ADJUST,
};

#define QWERTY DF(_QWERTY)
#define COLEMAK DF(_COLEMAK_DHK)

#define ADJUST MO(_ADJUST)
#define FKEYS MO(_FUNCTION)

// * Key Aliases
#define C_BSPC C(KC_BACKSPACE)
#define C_TAB C(KC_TAB)
#define S_TAB S(KC_TAB)

#define KC_C_A C(KC_LALT)

// * Tap Dance
enum {
    TD_DEBUG_FLASH,
};

void quantum_tap_dance(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            debug_enable ^= 1;
            break;
        case 2:
            // bootloader for flashing
            reset_keyboard();
            break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    // tap once to toggle debug mode, twice for bootloader
    [TD_DEBUG_FLASH] = ACTION_TAP_DANCE_FN(quantum_tap_dance),
};

#define DB_FLASH TD(TD_DEBUG_FLASH)

// * Dual-Role/Tap-Hold Keys
// tap-hold/dual-role aliases
#define DR_CTL MT(MOD_LCTL, KC_TAB)
#define DR_SFT MT(MOD_LSFT, KC_ESC)
// sxhkd hotkeys
#define DR_HOT MT(MOD_LGUI | MOD_LCTL, KC_LEFT_BRACKET)

// win/super/window management
#define DR_WIN MT(MOD_LGUI, KC_LEFT_PAREN)
#define DR_SYM LT(_SYM, KC_ENTER)
#define DR_ALT MT(MOD_LALT, KC_LEFT_CURLY_BRACE)

#define DR_NAV LT(_NAV, KC_BACKSPACE)
#define DR_RAT LT(_NAV, KC_DELETE)

#define DR_Z LT(_NAV, KC_Z)

// to be able to access media previous on nav layer
#define DR_SCAB MT(MOD_LSFT, S(C(KC_TAB)))

// necessary since QMK's dual-role implementation is garbage
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case DR_WIN:
        // necessary since mod-tap only supports basic keycodes by default
        if (record->tap.count && record->event.pressed) {
            tap_code16(KC_LEFT_PAREN);
            // Return false to ignore further processing of key
            return false;
        }
        break;
    case DR_ALT:
        if (record->tap.count && record->event.pressed) {
            tap_code16(KC_LEFT_CURLY_BRACE);
            // Return false to ignore further processing of key
            return false;
        }
        break;
    }
    return true;
}

// dumb mode for every key except z (which is potentially typed in rapid sucession with other keys)
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DR_Z:
            // do not immideatly select the hold action when another key is pressed
            return false;
        default:
            // select the hold action when another key is pressed
            return true;
    }
}

// * Key Overrides
// shift + next for previous
const key_override_t media_next_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_MEDIA_NEXT_TRACK, KC_MEDIA_PREV_TRACK);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &media_next_key_override,
    NULL // Null terminate the array of overrides!
};

// * Default LED Color/Mode
#ifdef RGBLIGHT_ENABLE
void keyboard_post_init_user(void) {
  // set default dark orange breathing (does not look like how this would look in a color picker)
  rgblight_sethsv_noeeprom(3, 255, 177);
  rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING);
  // on or off depending on last state
}
#endif

// * Keymaps
// full key list:
// https://docs.qmk.fm/#/keycodes

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: Colemak Dhk
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |RGB Next|   Q  |   W  |   F  |   P  |   B  |                              |   J  |   L  |   U  |   Y  | ;  : |  KP +  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | RGB Tog|   A  |   R  |   S  |   T  |   G  |                              |   K  |   N  |   E  |   I  |   O  |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | Quantum|      |   X  |   C  |   D  |   V  | Nav/Z|CapsWd|  |F-keys|Rat/Dl|   M  |   H  | ,  < | . >  | /  ? |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LCTL | LSFT | Space|Hotkey|  | Super| Nav  | Sym  | Alt  | Menu |
 *                        |      | Tab  | Esc  |      |  [   |  |   (  | Bkspc| Enter|   {  |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_COLEMAK_DHK] = LAYOUT(
     RGB_MOD , KC_Q   , KC_W   , KC_F   , KC_P   , KC_B   ,                                        KC_J  , KC_L  , KC_U  , KC_Y   ,KC_SCLN, KC_PPLS,
     RGB_TOG , KC_A   , KC_R   , KC_S   , KC_T   , KC_G   ,                                        KC_K  , KC_N  , KC_E  , KC_I   , KC_O  ,KC_QUOTE,
     DB_FLASH, _______, KC_X   , KC_C   , KC_D   , KC_V   , DR_Z  ,CW_TOGG,     FKEYS   , DR_RAT , KC_M  , KC_H  ,KC_COMM, KC_DOT ,KC_SLSH, _______,
                                 ADJUST , DR_CTL , DR_SFT , KC_SPC, DR_HOT,     DR_WIN  , DR_NAV , DR_SYM, DR_ALT, KC_APP

    ),

/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |RGB Next|   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  KP +  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | RGB Tog|   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  |  ; : |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | Quantum|      |   X  |   C  |   V  |   B  | Nav/Z|CapsWd|  |F-keys|Rat/Dl|   N  |   M  | ,  < | . >  | /  ? |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Adjust| LCTL | LSFT | Space|Hotkey|  | Super| Space| Sym  | Alt  | Menu |
 *                        |      | Tab  | Esc  |      |  [   |  |   (  |      | Enter|  {   |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
     RGB_MOD , KC_Q   , KC_W    , KC_E   , KC_R   , KC_T   ,                                        KC_Y  , KC_U  , KC_I  , KC_O   , KC_P  , KC_PPLS,
     RGB_TOG , KC_A   , KC_S    , KC_D   , KC_F   , KC_G   ,                                        KC_H  , KC_J  , KC_K  , KC_L   ,KC_SCLN,KC_QUOTE,
     DB_FLASH, _______, KC_X    , KC_C   , KC_V   , KC_B   , DR_NAV,CW_TOGG,     FKEYS   , DR_RAT , KC_N  , KC_M  ,KC_COMM, KC_DOT ,KC_SLSH, _______,
                                  ADJUST , DR_CTL , DR_SFT , KC_SPC, DR_HOT,     DR_WIN  , KC_SPC , DR_SYM, DR_ALT, KC_APP

    ),

/*
 * Nav Layer: Navigation, media
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |C+bspc| Rat↑ | PrtSc|      |                              |Brght↓|PgDown| Home | End  | PgUp | Brght↑ |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | Rat← | Rat↓ | Rat→ |      |                              |M Next|   ↓  |   →  |   ↑  | Play |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      | Rat1 | Rat2 | Rat3 |      |Wheel↓|Wheel↑|  |      |      |VolMut|   ←  | VolDn| VolUp|      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      | S-Tab| LSFT | C-Tab|      |  |      |      |      |      |      |
 *                        |      |      |CS-Tab|      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      _______, _______, C_BSPC , KC_MS_U, KC_PSCR, _______,                                     KC_BRID, KC_PGDN, KC_HOME, KC_END  , KC_PGUP, KC_BRIU,
      _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______,                                     KC_MNXT, KC_DOWN, KC_UP  , KC_RIGHT, KC_MPLY, _______,
      _______, _______, KC_BTN1, KC_BTN2, KC_BTN3, _______, KC_WH_D, KC_WH_U, _______, _______, KC_MUTE, KC_LEFT, KC_VOLD, KC_VOLU , _______, _______,
                                 _______, S_TAB  , DR_SCAB, C_TAB  , _______, _______, _______, _______, _______, _______
    ),

/*
 * Sym Layer: Numbers and symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  `   |  =   |  ]   |  _   |  \   |                              |   @  |  -   |  )   |  }   |  !   |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  1   |  2   |  3   |  4   |  |   |                              |   &  |  7   |  8   |  9   |  0   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |  ~   |  #   |  5   |  ^   |      |      |  |      |      |   $  |  6   |  +   |  *   |  %   |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_SYM] = LAYOUT(
     _______ , KC_GRV , KC_EQL , KC_RBRC, KC_UNDS, KC_BSLS,                                     KC_AT  , KC_MINS, KC_RPRN, KC_RCBR, KC_EXLM, _______,
     _______ , KC_1   , KC_2   , KC_3   , KC_4   , KC_PIPE,                                     KC_AMPR, KC_7   , KC_8   , KC_9   , KC_0   , _______,
     _______ , _______, KC_TILD, KC_HASH, KC_5   , KC_CIRC, _______, _______, _______, _______, KC_DLR , KC_6   , KC_ASTR, KC_PLUS, KC_PERC, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Function Layer: Function keys, other keys I don't really need
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      | F11  | F12  |      |      |                              |      |      | Power| Sleep| Wake |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  F1  |  F2  |  F3  |  F4  |      |                              |      |  F7  |  F8  |  F9  |  F10 |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |  F5  |      |      |      |  |      |      |      |  F6  |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      | C-Alt|      |      |  |      |      |      |      |      |
 *                        |      |      | (VT) |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_FUNCTION] = LAYOUT(
     _______ , KC_F11 , KC_F12 , _______, _______, _______,                                     _______, KC_PWR , KC_SLEP, KC_WAKE, _______, _______,
     _______ , KC_F1  , KC_F2  , KC_F3  , KC_F4  , _______,                                     _______, KC_F7  , KC_F8  , KC_F9  , KC_F10  , _______,
     _______ , _______, _______, _______, KC_F5  , _______, _______, _______, _______, _______, _______, KC_F6  , _______, _______, _______, _______,
                                 _______, _______, KC_C_A , _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * Adjust Layer: Default layer settings, RGB, and other keyboard commands
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |QWERTY|      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              | TOG  | HUI  | SAI  | VAI  | SPD+ |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |Colmak| Debug|      |      |      |  |      |      |      | HUD  | SAD  | VAD  | SPD- |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, QWERTY , _______, _______, _______, _______,                                    _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                                    RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, _______,
      _______, _______, _______, COLEMAK, DB_TOGG, _______,_______, _______, _______, _______, _______, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, _______,
                                 _______, _______, _______,_______, _______, _______, _______, _______, _______, _______
    ),

// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};


// * RGB
/* #ifdef RGBLIGHT_ENABLE */
/* void keyboard_post_init_user(void) { */
/*   rgblight_enable_noeeprom(); // Enables RGB, without saving settings */
/*   rgblight_sethsv_noeeprom(HSV_PURPLE); */
/*   rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT); */
/* } */
/* #endif */

// * OLED/Rotarary Encoder (Default)

/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/kyria/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * For your convenience, here's a copy of those settings so that you can uncomment them if you wish to apply your own modifications.
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */

/* DELETE THIS LINE TO UNCOMMENT (1/2)
#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // QMK Logo and version information
        // clang-format off
        static const char PROGMEM qmk_logo[] = {
            0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
            0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
            0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
        // clang-format on

        oled_write_P(qmk_logo, false);
        oled_write_P(PSTR("Kyria rev1.0\n\n"), false);

        // Host Keyboard Layer Status
        oled_write_P(PSTR("Layer: "), false);
        switch (get_highest_layer(layer_state|default_layer_state)) {
            case _QWERTY:
                oled_write_P(PSTR("QWERTY\n"), false);
                break;
            case _DVORAK:
                oled_write_P(PSTR("Dvorak\n"), false);
                break;
            case _COLEMAK_DHK:
                oled_write_P(PSTR("Colemak-DHK\n"), false);
                break;
            case _NAV:
                oled_write_P(PSTR("Nav\n"), false);
                break;
            case _SYM:
                oled_write_P(PSTR("Sym\n"), false);
                break;
            case _FUNCTION:
                oled_write_P(PSTR("Function\n"), false);
                break;
            case _ADJUST:
                oled_write_P(PSTR("Adjust\n"), false);
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }

        // Write host Keyboard LED Status to OLEDs
        led_t led_usb_state = host_keyboard_led_state();
        oled_write_P(led_usb_state.num_lock    ? PSTR("NUMLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
    } else {
        // clang-format off
        static const char PROGMEM kyria_logo[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
            0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };
        // clang-format on
        oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
    }
    return false;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {

    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return false;
}
#endif
DELETE THIS LINE TO UNCOMMENT (2/2) */


// Local Variables:
// fill-column: 200
// End:
