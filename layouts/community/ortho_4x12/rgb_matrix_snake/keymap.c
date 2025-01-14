#include QMK_KEYBOARD_H
#include "action_layer.h"
#include "eeconfig.h"

#include "raw_hid.h"

// SNAKE: include the header.
// (and remember to add snake.c in rules.mk).
#include "snake.h"

extern keymap_config_t keymap_config;

// SNAKE: add a layer for snake.
// (here, first layer is used to demonstrate the game).
enum layers {
  _SNAKE,
  _QWERTY,
  _WORKMAN,
  _COLEMAK,
  _DVORAK,
  _LOWER,
  _RAISE,
  _ADJUST
};

// SNAKE: some custom keycodes which may be useful, handled in process_record_user.
//  - QUARTER: reset keyboard when four corners tapped
//  - HALF: switch to QWERTY when the two homing keys tapped
//  - SNAKE: switch to SNAKE layer (and configure some things)
//  - DIRx: change snake direction
enum custom_keycodes {
  QUARTER = SAFE_RANGE,
  HALF,
  SNAKE,
  DIRRGHT,
  DIRUP,
  DIRLEFT,
  DIRDOWN
};

#define QWERTY     DF(_QWERTY)
#define WORKMAN    DF(_WORKMAN)
#define COLEMAK    DF(_COLEMAK)
#define DVORAK     DF(_DVORAK)

#define LWR_ESC LT(_LOWER, KC_ESC)
#define RSE_ENT LT(_RAISE, KC_ENT)

// SNAKE: keep track of number of the QUARTER/HALF pressed down
char quarter_count = 0;
char half_count = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// SNAKE: the snake 'game' keymap
[_SNAKE] = LAYOUT_ortho_4x12( \
  QUARTER, QK_RBT,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QUARTER, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, HALF,    XXXXXXX, XXXXXXX, HALF,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DIRUP,   XXXXXXX, \
  QUARTER, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DIRLEFT, DIRDOWN, DIRRGHT \
),

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | GUI  | Alt  | Tab  |Lower |Space |      |Raise | Del  | Alt  | GUI  | Ctrl |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_ortho_4x12(
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SC_SENT,
    KC_LCTL, KC_LGUI, KC_LALT, KC_TAB,  LWR_ESC, KC_SPC,  KC_BSPC, RSE_ENT, KC_DEL,  KC_RALT, KC_RGUI, KC_RCTL
),

/* Workman
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   D  |   R  |   W  |   B  |   J  |   F  |   U  |   P  |   ;  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   H  |   T  |   G  |   Y  |   N  |   E  |   O  |   I  |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   M  |   C  |   V  |   K  |   L  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | GUI  | Alt  | Tab  |Lower |Space |      |Raise | Del  | Alt  | GUI  | Ctrl |
 * `-----------------------------------------------------------------------------------'
 */
[_WORKMAN] = LAYOUT_ortho_4x12(
    KC_TAB,  KC_Q,    KC_D,    KC_R,    KC_W,    KC_B,    KC_J,    KC_F,    KC_U,    KC_P,    KC_SCLN, KC_BSPC,
    KC_ESC,  KC_A,    KC_S,    KC_H,    KC_T,    KC_G,    KC_Y,    KC_N,    KC_E,    KC_O,    KC_I,    KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_M,    KC_C,    KC_V,    KC_K,    KC_L,    KC_COMM, KC_DOT,  KC_SLSH, SC_SENT,
    KC_LCTL, KC_LGUI, KC_LALT, KC_TAB,  LWR_ESC, KC_SPC,  KC_BSPC, RSE_ENT, KC_DEL,  KC_RALT, KC_RGUI, KC_RCTL
),

/* Colemak
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   F  |   P  |   G  |   J  |   L  |   U  |   Y  |   ;  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   R  |   S  |   T  |   D  |   H  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   K  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |  FN  | Ctrl | Alt  | GUI  |Lower |Space |      |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_COLEMAK] = LAYOUT_ortho_4x12(
    KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC,
    KC_ESC,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SC_SENT,
    KC_LCTL, KC_LGUI, KC_LALT, KC_TAB,  LWR_ESC, KC_SPC,  KC_BSPC, RSE_ENT, KC_DEL,  KC_RALT, KC_RGUI, KC_RCTL
),

/* Dvorak
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   '  |   ,  |   .  |   P  |   Y  |   F  |   G  |   C  |   R  |   L  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   O  |   E  |   U  |   I  |   D  |   H  |   T  |   N  |   S  |  /   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   ;  |   Q  |   J  |   K  |   X  |   B  |   M  |   W  |   V  |   Z  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |  FN  | Ctrl | Alt  | GUI  |Lower |Space |      |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_DVORAK] = LAYOUT_ortho_4x12(
    KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_BSPC,
    KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH,
    KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    SC_SENT,
    KC_LCTL, KC_LGUI, KC_LALT, KC_TAB,  LWR_ESC, KC_SPC,  KC_BSPC, RSE_ENT, KC_DEL,  KC_RALT, KC_RGUI, KC_RCTL
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  |  |   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ins  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  |  ?   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      | Home |Pg Dn |Pg Up | End  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_ortho_4x12(
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
    KC_INS,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_QUES,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,
    _______, _______, _______, _______, XXXXXXX, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |  \   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   -  |   =  |   [  |   ]  |  /   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_ortho_4x12(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSLS,
    KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_SLSH,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, XXXXXXX, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY
),

/* Adjust (Lower + Raise)
 *                      v------------------------RGB CONTROL--------------------v
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |CapsLk|      |Pause |ScrLck|PrtScr|      |      |QWERTY|WORKMN|COLEMK|DVORAK|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_ortho_4x12(
    _______, QK_BOOT, DB_TOGG, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, _______,
    KC_CAPS, _______, KC_PAUS, KC_SCRL, KC_PSCR, _______, _______, QWERTY,  WORKMAN, COLEMAK, DVORAK,  _______,
    _______, _______, _______, _______, _______, _______, _______, SNAKE,   _______, _______, _______, _______,
    _______, _______, _______, _______, XXXXXXX, _______, _______, XXXXXXX, _______, _______, _______, _______
)

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // SNAKE: impl. key handling logic here
    case DIRUP:
      if (snake_status.last_moved_direction != DIRECTION_DOWN) {
        snake_status.direction = DIRECTION_UP;
      }
      return false;
      break;
    case DIRDOWN:
      if (snake_status.last_moved_direction != DIRECTION_UP) {
        snake_status.direction = DIRECTION_DOWN;
      }
      return false;
      break;
    case DIRLEFT:
      if (snake_status.last_moved_direction != DIRECTION_RIGHT) {
        snake_status.direction = DIRECTION_LEFT;
      }
      return false;
      break;
    case DIRRGHT:
      if (snake_status.last_moved_direction != DIRECTION_LEFT) {
         snake_status.direction = DIRECTION_RIGHT;
      }

      // corner
      if (record->event.pressed) {
        quarter_count += 1;
      } else {
        quarter_count -= 1;
      }
      if (quarter_count == 4) {
        reset_keyboard();
      }
      return false;
      break;
    case QUARTER:
      // corner
      if (record->event.pressed) {
        quarter_count += 1;
      } else {
        quarter_count -= 1;
      }
      if (quarter_count == 4) {
        reset_keyboard();
      }
      return false;
      break;
    case HALF:
      if (record->event.pressed) {
        half_count += 1;
      } else {
        half_count -= 1;
      }
      if (half_count == 2) {
        layer_move(_QWERTY);
        rgb_matrix_mode_noeeprom(RGB_MATRIX_CYCLE_PINWHEEL);
      }
      return false;
      break;
    case SNAKE:
      layer_move(_SNAKE);
      rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_SNAKE);
      return false;
      break;
  }
  return true;
}

void keyboard_post_init_user(void) {
  eeconfig_init();
  // SNAKE: for demo purposes, activate snake layer immediately
  rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_SNAKE);

  snake_init();
}

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
