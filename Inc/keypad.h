#ifndef KEYPAD_H
#define	KEYPAD_H

#ifdef	__cplusplus
extern "C" {
#endif

#define __FUZYONEL_KEYPAD_HOLD_TIME 500 // Duration (in milliseconds) for a button to be considered "held"
#define __FUZYONEL_KEYPAD_IDLE_TIME 100 // Duration (in milliseconds) for a button to transition to "idle"
#define __FUZYONEL_KEYPAD_DEBUG // Enables debug mode to log key state transitions
//#define __FUZYONEL_KEYPAD_VERBOSE // Enables detailed debug logs for key transitions and actions. Requires __FUZYONEL_KEYPAD_DEBUG enabled

typedef enum {
    KEYPAD_TYPE_BUTTONS = 0,
    KEYPAD_TYPE_3x4KEYPAD,
    KEYPAD_TYPE_MIXED
} Fuz_KeypadTypes_t;

typedef enum {
    KEY_0 = 0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_STAR,
    KEY_SHARP,
    KEY_ENTER,
    KEY_ESC,
    KEY_BACK,
    KEY_PLUS,
    KEY_MINUS,
    KEY_MULTIPLY,
    KEY_DIVIDE,
    KEY_PLAY,
    KEY_STOP,
    KEY_NEXT,
    KEY_PREV
} Fuz_Keys_t;

typedef enum {
    KEY_IDLE = 0,
    KEY_PRESSED,
    KEY_HOLD,
    KEY_RELEASED
} Fuz_KeyState_t;

typedef struct {
    Fuz_Keys_t Type;
    Fuz_KeypadTypes_t SetType;
    Fuz_KeyState_t State;
    uint32_t LastActionTime;
    uint32_t LastProcessedTime;
    void* KeypadOutputPort;
    void* KeypadOutputPin;
    void* KeypadInputPort;
    void* KeypadInputPin;
} Fuz_Key_t;

typedef struct {
    Fuz_KeypadTypes_t Type;
    Fuz_Key_t *Keys;
    uint8_t KeyCount;
} Fuz_Keypad_t;


void Keypad_Init(Fuz_Keypad_t*);
void Keypad_Scan(Fuz_Keypad_t*, uint32_t);

#ifdef	__cplusplus
}
#endif

#endif	/* KEYPAD_H */

