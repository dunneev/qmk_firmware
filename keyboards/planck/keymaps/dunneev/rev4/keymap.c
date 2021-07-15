/* Copyright 2015-2017 Jack Humbert
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
#include "muse.h"


// Set dvorak first, so it's the default
enum planck_layers { _QWERTY, _COLEMAK, _DVORAK, _LOWER, _RAISE, _PLOVER, _ADJUST };

enum planck_keycodes { QWERTY = SAFE_RANGE, COLEMAK, DVORAK, PLOVER, BACKLIT, EXT_PLV };

// Tap Dance declarations
typedef struct {
    bool    is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP = 1,
    DOUBLE_TAP,
    TRIPLE_TAP,
    QUAD_TAP,
    HOLD,
};

enum {
    LEFT_SHIFT_BRACKET_DANCE,
    RIGHT_SHIFT_BRACKET_DANCE,
    EQUALS_SIGN_ENTER_DANCE,
    ESCAPE_BACKTICK_TILDE_DANCE,
};

uint8_t cur_dance(qk_tap_dance_state_t *state);

// For the SHIFT tap dance. Put it here so it can be used in any keymap
void left_shift_finished(qk_tap_dance_state_t *state, void *user_data);
void left_shift_reset(qk_tap_dance_state_t *state, void *user_data);
void right_shift_finished(qk_tap_dance_state_t *state, void *user_data);
void right_shift_reset(qk_tap_dance_state_t *state, void *user_data);

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // I HAVE MODIFIED LAYERS 0, 3, 4, and 6 USING ONLINE CONFIGURATOR.
    // Ran this command to generate keymap.c from the json keymap file:
    // qmk json2c -o output-keymap.c workhorse.json

    /* Qwerty
     * ,-----------------------------------------------------------------------------------.
     * | ESC  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Tab  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
     * `-----------------------------------------------------------------------------------'
     */
    [_QWERTY] = LAYOUT_planck_grid(TD(ESCAPE_BACKTICK_TILDE_DANCE),KC_Q,KC_W,KC_E,KC_R,KC_T,KC_Y,KC_U,KC_I,KC_O,KC_P,KC_BSPC,KC_TAB,KC_A,KC_S,KC_D,KC_F,KC_G,KC_H,KC_J,KC_K,KC_L,KC_SCLN,KC_QUOT,TD(LEFT_SHIFT_BRACKET_DANCE),KC_Z,KC_X,KC_C,KC_V,KC_B,KC_N,KC_M,KC_COMM,KC_DOT,KC_SLSH,TD(RIGHT_SHIFT_BRACKET_DANCE),KC_LCTL,KC_LGUI,KC_CAPS,KC_LALT,LOWER,KC_SPC,KC_SPC,RAISE,KC_UNDS,KC_BSLS,KC_DEL,RCTL_T(KC_ENT)),

    /* Colemak
     * ,-----------------------------------------------------------------------------------.
     * | Tab  |   Q  |   W  |   F  |   P  |   G  |   J  |   L  |   U  |   Y  |   ;  | Bksp |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Esc  |   A  |   R  |   S  |   T  |   D  |   H  |   N  |   E  |   I  |   O  |     |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Shift|   Z  |   X  |   C  |   V  |   B  |   K  |   M  |   ,  |   .  |   /  |Enter |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
     * `-----------------------------------------------------------------------------------'
     */
    [_COLEMAK] = LAYOUT_planck_grid(KC_TRNS,KC_Q,KC_W,KC_F,KC_P,KC_G,KC_J,KC_L,KC_U,KC_Y,KC_SCLN,KC_TRNS,KC_TRNS,KC_TRNS,KC_R,KC_S,KC_T,KC_D,KC_H,KC_N,KC_E,KC_I,KC_O,KC_QUOT,KC_TRNS,KC_Z,KC_X,KC_C,KC_V,KC_B,KC_K,KC_M,KC_COMM,KC_DOT,KC_SLSH,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS),

    [_DVORAK] = LAYOUT_planck_grid(KC_TRNS, KC_QUOT,KC_COMM,KC_DOT,KC_P,KC_Y,KC_F,KC_G,KC_C,KC_R,KC_L,KC_TRNS,KC_TRNS,KC_A,KC_O,KC_E,KC_U,KC_I,KC_D,KC_H,KC_T,KC_N,KC_S,KC_MINS,KC_TRNS,KC_SCLN,KC_Q,KC_J,KC_K,KC_X,KC_B,KC_M,KC_W,KC_V,KC_Z,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_SLSH,KC_TRNS,KC_TRNS,KC_TRNS),

    /* Lower
     * ,-----------------------------------------------------------------------------------.
     * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Bksp |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  |  |   |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO ~ |ISO | | Home | End  |      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
     * `-----------------------------------------------------------------------------------'
     */
    [_LOWER] = LAYOUT_planck_grid(KC_TRNS,KC_F1,KC_F2,KC_F3,KC_F4,KC_LPRN,KC_RPRN,KC_1,KC_2,KC_3,KC_PPLS,KC_BSPC,KC_TAB,KC_F5,KC_F6,KC_F7,KC_F8,KC_LPRN,KC_RPRN,KC_4,KC_5,KC_6,KC_PMNS,KC_PERC,KC_LSFT,KC_F9,KC_F10,KC_F11,KC_F12,KC_LPRN,KC_RPRN,KC_7,KC_8,KC_9,KC_PAST,KC_CIRC,KC_LCTL,LCTL(KC_Z),KC_CAPS,KC_TRNS,KC_TRNS,KC_SPC,KC_SPC,KC_TRNS,KC_0,KC_PDOT,KC_PSLS,TD(EQUALS_SIGN_ENTER_DANCE)),

    /* Raise
     * ,-----------------------------------------------------------------------------------.
     * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   -  |   =  |   [  |   ]  |  \   |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO # |ISO / |Pg Up |Pg Dn |      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
     * `-----------------------------------------------------------------------------------'
     */
    [_RAISE] = LAYOUT_planck_grid(KC_TRNS,KC_EXLM,KC_PIPE,KC_AMPR,KC_EQUAL,KC_HASH,KC_AMPR,KC_PMNS,KC_PPLS,KC_PAST,KC_DEL,KC_BSPC,KC_TAB,KC_LCBR,KC_LPRN,KC_RPRN,KC_RCBR,KC_DLR,KC_LEFT,KC_DOWN,KC_UP,KC_RGHT,KC_SCLN,RCS(KC_Z),KC_LSFT,KC_LBRC,KC_LT,KC_GT,KC_RBRC,KC_PDOT,KC_HOME,KC_PGDN,KC_PGUP,KC_END,KC_QUES,KC_RSFT,KC_LCTL,LCTL(KC_Z),LCTL(KC_X),LCTL(KC_C),KC_TRNS,KC_SPC,KC_SPC,KC_TRNS,KC_UNDS,KC_BSLS,LCTL(KC_V),KC_SFTENT),

    /* Plover layer (http://opensteno.org)
     * ,-----------------------------------------------------------------------------------.
     * |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |   S  |   T  |   P  |   H  |   *  |   *  |   F  |   P  |   L  |   T  |   D  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |   S  |   K  |   W  |   R  |   *  |   *  |   R  |   B  |   G  |   S  |   Z  |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Exit |      |      |   A  |   O  |             |   E  |   U  |      |      |      |
     * `-----------------------------------------------------------------------------------'
     */
    [_PLOVER] = LAYOUT_planck_grid(KC_1, KC_1, KC_1, KC_1, KC_1, KC_1, KC_1, KC_1, KC_1, KC_1, KC_1, KC_1, XXXXXXX, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, XXXXXXX, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, EXT_PLV, XXXXXXX, XXXXXXX, KC_C, KC_V, XXXXXXX, XXXXXXX, KC_N, KC_M, XXXXXXX, XXXXXXX, XXXXXXX),

    /* Adjust (Lower + Raise)
     *                      v------------------------RGB CONTROL--------------------v
     * ,-----------------------------------------------------------------------------------.
     * |      | Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|  Del |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |MUSmod|Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|TermOn|TermOf|      |      |      |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |             |      |      |      |      |      |
     * `-----------------------------------------------------------------------------------'
     */
    [_ADJUST] = LAYOUT_planck_grid(KC_PWR,KC_SLEP,KC_WAKE,KC_MYCM,CK_UP,KC_NO,KC_BTN1,KC_BTN2,KC_BTN3,KC_BTN4,KC_NO,KC_VOLU,DF(0),DF(1),DF(2),KC_NO,CK_DOWN,MI_OFF,KC_MS_L,KC_MS_D,KC_MS_U,KC_MS_R,KC_CALC,KC_VOLD,KC_NO,KC_NO,AU_TOG,MU_TOG,CK_TOGG,MI_ON,KC_ACL0,KC_ACL1,KC_ACL2,KC_NO,KC_MPLY,KC_MUTE,BL_STEP,BL_BRTG,AU_OFF,MU_MOD,KC_TRNS,KC_BRID,KC_BRIU,KC_TRNS,KC_NO,KC_MPRV,KC_MSTP,KC_MNXT)

};

#ifdef AUDIO_ENABLE
float plover_song[][2]    = SONG(PLOVER_SOUND);
float plover_gb_song[][2] = SONG(PLOVER_GOODBYE_SOUND);
#endif

layer_state_t layer_state_set_user(layer_state_t state) { return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST); }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                print("mode just switched to qwerty and this is a huge string\n");
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
            break;
        case COLEMAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAK);
            }
            return false;
            break;
        case DVORAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_DVORAK);
            }
            return false;
            break;
        case BACKLIT:
            if (record->event.pressed) {
// register_code(KC_RSFT);
#ifdef BACKLIGHT_ENABLE
                backlight_step();
#endif
#ifdef KEYBOARD_planck_rev5
                writePinLow(E6);
#endif
            } else {
                // unregister_code(KC_RSFT);
#ifdef KEYBOARD_planck_rev5
                writePinHigh(E6);
#endif
            }
            return false;
            break;
        case PLOVER:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                stop_all_notes();
                PLAY_SONG(plover_song);
#endif
                layer_off(_RAISE);
                layer_off(_LOWER);
                layer_off(_ADJUST);
                layer_on(_PLOVER);
                if (!eeconfig_is_enabled()) {
                    eeconfig_init();
                }
                keymap_config.raw  = eeconfig_read_keymap();
                keymap_config.nkro = 1;
                eeconfig_update_keymap(keymap_config.raw);
            }
            return false;
            break;
        case EXT_PLV:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                PLAY_SONG(plover_gb_song);
#endif
                layer_off(_PLOVER);
            }
            return false;
            break;
    }
    return true;
}

bool     muse_mode      = false;
uint8_t  last_muse_note = 0;
uint16_t muse_counter   = 0;
uint8_t  muse_offset    = 70;
uint16_t muse_tempo     = 50;

void encoder_update(bool clockwise) {
    if (muse_mode) {
        if (IS_LAYER_ON(_RAISE)) {
            if (clockwise) {
                muse_offset++;
            } else {
                muse_offset--;
            }
        } else {
            if (clockwise) {
                muse_tempo += 1;
            } else {
                muse_tempo -= 1;
            }
        }
    } else {
        if (clockwise) {
#ifdef MOUSEKEY_ENABLE
            tap_code(KC_MS_WH_DOWN);
#else
            tap_code(KC_PGDN);
#endif
        } else {
#ifdef MOUSEKEY_ENABLE
            tap_code(KC_MS_WH_UP);
#else
            tap_code(KC_PGUP);
#endif
        }
    }
}

void dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0: {
#ifdef AUDIO_ENABLE
            static bool play_sound = false;
#endif
            if (active) {
#ifdef AUDIO_ENABLE
                if (play_sound) {
                    PLAY_SONG(plover_song);
                }
#endif
                layer_on(_ADJUST);
            } else {
#ifdef AUDIO_ENABLE
                if (play_sound) {
                    PLAY_SONG(plover_gb_song);
                }
#endif
                layer_off(_ADJUST);
            }
#ifdef AUDIO_ENABLE
            play_sound = true;
#endif
            break;
        }
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
    }
}

void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
    switch (keycode) {
        case RAISE:
        case LOWER:
            return false;
        default:
            return true;
    }
}


/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustating to type.
 *
 * For the third point, there does exist the 'DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) {
            return SINGLE_TAP;
        }

        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return HOLD;
    } 
    else if (state->count == 2) {
        // DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        // if (state->interrupted) return DOUBLE_SINGLE_TAP;
        // else if (state->pressed) return DOUBLE_HOLD;

        if (!state->pressed) return DOUBLE_TAP;

        else return HOLD;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (!state->pressed) return TRIPLE_TAP;

        else return HOLD;
    }

    if (state->count == 4) {
        if (!state->pressed) return QUAD_TAP;

        else return HOLD;
    } 

    else return 8; // Magic number. At some point this method will expand to work for more presses
}

// Create an instance of 'tap' for the 'shift' tap dance.
static tap shift_tap_state = {
    .is_press_action = true,
    .state = 0
};

// Must use normal keycodes instead of quantum ones. Otherwise I run into a compilation error.
void left_shift_finished(qk_tap_dance_state_t *state, void *user_data) {
    shift_tap_state.state = cur_dance(state);
    switch (shift_tap_state.state) {
        // {
        case SINGLE_TAP: 
            register_code(KC_LSHIFT);
            register_code(KC_LBRC);
            break;

        // (
        case DOUBLE_TAP: 
            register_code(KC_LSHIFT);
            register_code(KC_9); 
            break;

        // [ 
        case TRIPLE_TAP: 
            register_code(KC_LBRC); 
            break;

        // < 
        case QUAD_TAP: 
            register_code(KC_LSHIFT);
            register_code(KC_COMMA);
            break;

        case HOLD: register_code(KC_LSHIFT); break;

        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        // case DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X);
    }
}

void left_shift_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (shift_tap_state.state) {
        case SINGLE_TAP: 
            unregister_code(KC_LSHIFT);
            unregister_code(KC_LBRC);
            break;

        // (
        case DOUBLE_TAP: 
            unregister_code(KC_LSHIFT);
            unregister_code(KC_9); 
            break;

        // [ 
        case TRIPLE_TAP: 
            unregister_code(KC_LBRC); 
            break;

        // < 
        case QUAD_TAP: 
            unregister_code(KC_LSHIFT);
            unregister_code(KC_COMMA);
            break;
            
        case HOLD: unregister_code(KC_LSHIFT); break;
    }
    shift_tap_state.state = 0;
}

void right_shift_finished(qk_tap_dance_state_t *state, void *user_data) {
    shift_tap_state.state = cur_dance(state);
    switch (shift_tap_state.state) {
        // }
        case SINGLE_TAP: 
            register_code(KC_LSHIFT);
            register_code(KC_RBRC);
            break;

        // )
        case DOUBLE_TAP: 
            register_code(KC_LSHIFT);
            register_code(KC_0); 
            break;

        // ]
        case TRIPLE_TAP: 
            register_code(KC_RBRC); 
            break;

        // >
        case QUAD_TAP: 
            register_code(KC_LSHIFT);
            register_code(KC_DOT);
            break;

        case HOLD: register_code(KC_RSHIFT); break;

        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.a
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        // case DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X);
    }
}

void right_shift_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (shift_tap_state.state) {
        case SINGLE_TAP: 
            unregister_code(KC_LSHIFT);
            unregister_code(KC_RBRC);
            break;

        // )
        case DOUBLE_TAP: 
            unregister_code(KC_LSHIFT);
            unregister_code(KC_0); 
            break;

        // ]
        case TRIPLE_TAP: 
            unregister_code(KC_RBRC); 
            break;

        // >
        case QUAD_TAP: 
            unregister_code(KC_LSHIFT);
            unregister_code(KC_DOT);
            break;

        case HOLD: unregister_code(KC_RSHIFT); break;
    }
    shift_tap_state.state = 0;
}


// Create an instance of 'tap' for the 'shift' tap dance.
static tap esc_tap_state = {
    .is_press_action = true,
    .state = 0
};


void esc_finished(qk_tap_dance_state_t *state, void *user_data) {
    esc_tap_state.state = cur_dance(state);
    switch (esc_tap_state.state) {
        // ESC
        case SINGLE_TAP: 
            register_code(KC_ESC);
            break;

        // ~
        case DOUBLE_TAP: 
            register_code(KC_GRV);
            break;

        // `
        case TRIPLE_TAP: 
            register_code(KC_LSFT); 
            register_code(KC_GRV);

            break;

    }
}

void esc_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (esc_tap_state.state) {

        // ESC
        case SINGLE_TAP: 
            unregister_code(KC_ESC);
            break;

        // ~
        case DOUBLE_TAP: 
            unregister_code(KC_GRV);
            break;

        // `
        case TRIPLE_TAP: 
            unregister_code(KC_LSFT); 
            unregister_code(KC_GRV);

            break;
    }
    esc_tap_state.state = 0;
}


qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap shifts for different brackets
    [LEFT_SHIFT_BRACKET_DANCE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, left_shift_finished, left_shift_reset),
    [RIGHT_SHIFT_BRACKET_DANCE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, right_shift_finished, right_shift_reset),

    // Tap numpad equals once for enter, twice for '='
    [EQUALS_SIGN_ENTER_DANCE] = ACTION_TAP_DANCE_DOUBLE(KC_KP_EQUAL, KC_EQUAL),
    [ESCAPE_BACKTICK_TILDE_DANCE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, esc_finished, esc_reset),

};