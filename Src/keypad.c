#include <stdint.h>
#include "keypad.h"

#ifdef __FUZYONEL_KEYPAD_DEBUG
#include <stdio.h>
#define DEBUG_MSG_BUFFER_SIZE 70
const char* __FUZYONEL_KEYPAD_STATE_STRINGS[] = {
    "KEY_IDLE",
    "KEY_PRESSED",
    "KEY_HOLD",
    "KEY_RELEASED"
};
#endif

/**
 * @brief Initializes the keypad structure and resets all keys to their default state.
 *
 * This function iterates through all keys in the keypad structure, setting their
 * state to KEY_IDLE and resetting their timing information.
 *
 * @param keypad Pointer to the keypad structure to initialize.
 */
void Keypad_Init(Fuz_Keypad_t *keypad) {
    if (!keypad || !keypad->Keys) {
        return;
    }

    for (uint8_t i = 0; i < keypad->KeyCount; i++) {
        keypad->Keys[i].State = KEY_IDLE;
        keypad->Keys[i].LastActionTime = 0;
        keypad->Keys[i].LastProcessedTime = 0;
    }
}

/**
 * @brief Scans the keypad for state changes and updates key states.
 *
 * This function reads the input state of each key, processes transitions between
 * states (e.g., KEY_PRESSED to KEY_HOLD), and updates the key's state and timing
 * information. Optionally, logs state transitions if debug mode is enabled.
 *
 * @param keypad Pointer to the keypad structure to scan.
 * @param currentTime Current system time (e.g., from a millisecond timer).
 */
void Keypad_Scan(Fuz_Keypad_t *keypad, uint32_t currentTime) {
#ifdef __FUZYONEL_KEYPAD_DEBUG
    char msg[DEBUG_MSG_BUFFER_SIZE];
#endif
    for (uint8_t i = 0; i < keypad->KeyCount; i++) {
        if (keypad->Keys[i].SetType == KEYPAD_TYPE_3x4KEYPAD) {
            __Fuzyonel_Keypad_Interface_Key_SetHigh(
                    keypad->Keys[i].KeypadOutputPort,
                    keypad->Keys[i].KeypadOutputPin
                    );
#ifdef __FUZYONEL_KEYPAD_DEBUG
#ifdef __FUZYONEL_KEYPAD_VERBOSE
            __Fuzyonel_Keypad_Interface_Debug("Changing keypad matrix line to 'high'", keypad->Keys[i]);
#endif
#endif
        }

        Fuz_KeyState_t oldState = keypad->Keys[i].State;

        uint8_t currentState = __Fuzyonel_Keypad_Interface_Key_Read(
                keypad->Keys[i].KeypadInputPort,
                keypad->Keys[i].KeypadInputPin
                ) ? 1 : 0;
        // newState encodes the current key state and the immediate input in two bits:
        // Bits: [Old State | Current Input]
        Fuz_KeyState_t newState = ((keypad->Keys[i].State << 1) | currentState) & 0b11;

        if (currentState) {
            if (oldState == KEY_IDLE) {
                keypad->Keys[i].State = KEY_PRESSED;
                keypad->Keys[i].LastActionTime = currentTime;
#ifdef __FUZYONEL_KEYPAD_DEBUG
                snprintf(msg, sizeof (msg), "Key %d: Transition change: %s -> %s", i, __FUZYONEL_KEYPAD_STATE_STRINGS[oldState], __FUZYONEL_KEYPAD_STATE_STRINGS[newState]);
                __Fuzyonel_Keypad_Interface_Debug(msg, keypad->Keys[i]);
#endif
            } else if (oldState == KEY_PRESSED && currentTime - keypad->Keys[i].LastActionTime > __FUZYONEL_KEYPAD_HOLD_TIME) {
                keypad->Keys[i].State = KEY_HOLD;
                keypad->Keys[i].LastActionTime = currentTime;
#ifdef __FUZYONEL_KEYPAD_DEBUG
                snprintf(msg, sizeof (msg), "Key %d: Transition change: %s -> %s", i, __FUZYONEL_KEYPAD_STATE_STRINGS[oldState], __FUZYONEL_KEYPAD_STATE_STRINGS[newState]);
                __Fuzyonel_Keypad_Interface_Debug(msg, keypad->Keys[i]);
#endif
            }
        } else if (oldState != KEY_IDLE) {
            if (currentTime - keypad->Keys[i].LastActionTime > __FUZYONEL_KEYPAD_IDLE_TIME) {
                keypad->Keys[i].State = KEY_IDLE;
            } else {
                keypad->Keys[i].State = KEY_RELEASED;
                keypad->Keys[i].LastActionTime = currentTime;
#ifdef __FUZYONEL_KEYPAD_DEBUG
                snprintf(msg, sizeof (msg), "Key %d: Transition change: %s -> %s", i, __FUZYONEL_KEYPAD_STATE_STRINGS[oldState], __FUZYONEL_KEYPAD_STATE_STRINGS[newState]);
                __Fuzyonel_Keypad_Interface_Debug(msg, keypad->Keys[i]);
#endif
            }
        }

        if (oldState != keypad->Keys[i].State) {
            keypad->Keys[i].LastActionTime = currentTime;
        }

        if (keypad->Keys[i].SetType == KEYPAD_TYPE_3x4KEYPAD) {
            __Fuzyonel_Keypad_Interface_Key_SetLow(
                    keypad->Keys[i].KeypadOutputPort,
                    keypad->Keys[i].KeypadOutputPin
                    );
#ifdef __FUZYONEL_KEYPAD_DEBUG
#ifdef __FUZYONEL_KEYPAD_VERBOSE
            __Fuzyonel_Keypad_Interface_Debug("Changing keypad matrix line to 'low'", keypad->Keys[i]);
#endif
#endif
        }
    }
}
