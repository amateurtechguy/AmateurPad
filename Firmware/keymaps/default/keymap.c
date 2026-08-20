// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    GITHUB = SAFE_RANGE,
    NOTEPAD
};

enum encoder_mode {
    VOLUME_MODE,
    BRIGHTNESS_MODE
};

static enum encoder_mode current_mode = VOLUME_MODE;

#define ENCODER_BUTTON_PIN GP1

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_3x3(
        KC_CALC, GITHUB,  NOTEPAD,
        KC_ESC,  KC_DEL,  KC_LALT,
        KC_LCTL, KC_SPC,  KC_ENT
    )
};

void keyboard_post_init_user(void) {
    setPinInputHigh(ENCODER_BUTTON_PIN);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }

    switch (keycode) {
        case GITHUB:
            tap_code16(LGUI(KC_R));
            wait_ms(100);
            send_string("https://github.com/amateurtechguy/AmateurPad");
            tap_code(KC_ENT);
            return false;

        case NOTEPAD:
            tap_code16(LGUI(KC_R));
            wait_ms(100);
            send_string("notepad");
            tap_code(KC_ENT);
            return false;
    }

    return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index != 0) {
        return true;
    }

    if (current_mode == VOLUME_MODE) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else {
        if (clockwise) {
            tap_code(KC_BRIU);
        } else {
            tap_code(KC_BRID);
        }
    }

    return true;
}

void matrix_scan_user(void) {
    static bool last_state = true;
    bool current_state = readPin(ENCODER_BUTTON_PIN);

    if (last_state && !current_state) {
        current_mode = (current_mode == VOLUME_MODE)
            ? BRIGHTNESS_MODE
            : VOLUME_MODE;
    }

    last_state = current_state;
}
