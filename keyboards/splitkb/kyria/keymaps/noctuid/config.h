/* Copyright 2022 Thomas Baart <thomas@splitkb.com>
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

#pragma once

// * RGB Settings
#ifdef RGBLIGHT_ENABLE
// decrease steps from 10 to 5
#define RGBLIGHT_HUE_STEP 5
#define RGBLIGHT_SAT_STEP 5
#define RGBLIGHT_VAL_STEP 5
#endif

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_EFFECT_BREATHING
#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#    define RGBLIGHT_EFFECT_SNAKE
#    define RGBLIGHT_EFFECT_KNIGHT
#    define RGBLIGHT_EFFECT_CHRISTMAS
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#    define RGBLIGHT_EFFECT_RGB_TEST
#    define RGBLIGHT_EFFECT_ALTERNATING
#    define RGBLIGHT_EFFECT_TWINKLE
#    ifndef RGBLIGHT_LIMIT_VAL
#    	define RGBLIGHT_LIMIT_VAL 150
#    endif
#endif

// * Tap-Hold Settings
// default
// #define TAPPING_TERM 200

// dumb mode; always act as hold when both pressed at once
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

// don't treat mod tap keys specially; honor PERMISSIVE_HOLD
#define IGNORE_MOD_TAP_INTERRUPT

// have tap-hold key act as hold (even if tapping term duration has not passed)
// when another key overlaps completely, i.e. the other key is pressed and
// released while the tap-hold key is still held; only applies for keys not
// handled by HOLD_ON_OTHER_KEY_PRESS (i.e. keys get_hold_on_other_key_press
// returns false for)
#define PERMISSIVE_HOLD

// disable tapping then holding tap-hold keys to repeat tap action
// occasionally using for backspace, so commented
// #define QUICK_TAP_TERM 0
