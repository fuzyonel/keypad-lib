#include "keypad.h"

#define BUTTON_COUNT 4

Fuz_Key_t keys[BUTTON_COUNT];
Fuz_Keypad_t keypad;

/**
 * @brief Main function demonstrating keypad library usage.
 *
 * This example initializes the keypad with user-provided hardware configuration.
 * The user must implement the hardware interface functions (e.g., setting pins high/low).
 */
void main() {
    keys[0] = (Fuz_Key_t){
        .Type = KEY_1,
        .SetType = KEYPAD_TYPE_3x4KEYPAD,
        .KeypadOutputPort = COL1_PORT,
        .KeypadOutputPin = COL1_LAT,
        .KeypadInputPort = ROW1_PORT,
        .KeypadInputPin = ROW1_LAT
    };

    keys[1] = (Fuz_Key_t){
        .Type = KEY_2,
        .SetType = KEYPAD_TYPE_3x4KEYPAD,
        .KeypadOutputPort = COL2_PORT,
        .KeypadOutputPin = COL2_LAT,
        .KeypadInputPort = ROW1_PORT,
        .KeypadInputPin = ROW1_LAT
    };

    keys[2] = (Fuz_Key_t){
        .Type = KEY_3,
        .SetType = KEYPAD_TYPE_3x4KEYPAD,
        .KeypadOutputPort = COL3_PORT,
        .KeypadOutputPin = COL3_LAT,
        .KeypadInputPort = ROW1_PORT,
        .KeypadInputPin = ROW1_LAT
    };

    keys[3] = (Fuz_Key_t){
        .Type = KEY_ENTER,
        .SetType = KEYPAD_TYPE_BUTTONS,
        .KeypadInputPort = ROW4_PORT,
        .KeypadInputPin = ROW4_LAT
    };

    keypad = (Fuz_Keypad_t){
        .Type = KEYPAD_TYPE_MIXED,
        .Keys = keys,
        .KeyCount = BUTTON_COUNT
    };

    Keypad_Init(&keypad);

    uint32_t counter;

    for(;;) {
        counter++;
        Keypad_Scan(&keypad, counter);

        __delay_ms(1);
    }
}

/**
 * @brief Sets a specific keypad pin to 'high'.
 *
 * This function is part of the user-provided hardware abstraction layer and must
 * be implemented for the target hardware platform.
 *
 * For PIC platform:

 void __Fuzyonel_Keypad_Interface_Key_SetHigh(void* port, void* pin) {
     *(volatile uint8_t *)pin = 1;
 }

 * For ST platform:

 void __Fuzyonel_Keypad_Interface_Key_SetHigh(GPIO_TypeDef * port, uint16_t pin) {
     HAL_GPIO_WritePin(port, pin, 1);
 }

 * @param port The GPIO port for the pin (not used in this example).
 * @param pin Pointer to the specific pin to set high.
 */
// For PIC platform
void __Fuzyonel_Keypad_Interface_Key_SetHigh(void* port, void* pin) {
    *(volatile uint8_t *)pin = 1;
}

/**
 * @brief Sets a specific keypad pin to 'low'.
 *
 * This function is part of the user-provided hardware abstraction layer and must
 * be implemented for the target hardware platform.
 *
 * For PIC platform:

 void __Fuzyonel_Keypad_Interface_Key_SetLow(void* port, void* pin) {
     *(volatile uint8_t *)pin = 0;
 }

 * For ST platform:

 void __Fuzyonel_Keypad_Interface_Key_SetLow(GPIO_TypeDef * port, uint16_t pin) {
     HAL_GPIO_WritePin(port, pin, 0);
 }

 * @param port The GPIO port for the pin (not used in this example).
 * @param pin Pointer to the specific pin to set low.
 */
void __Fuzyonel_Keypad_Interface_Key_SetLow(void* port, void* pin) {
    *(volatile uint8_t *)pin = 0;
}

/**
 * @brief Reads the state of a specific keypad pin.
 *
 * This function is part of the user-provided hardware abstraction layer and must
 * be implemented for the target hardware platform.
 *
 * For PIC platform:

 uint8_t __Fuzyonel_Keypad_Interface_Key_Read(void* port, void* pin) {
     return (*(volatile uint8_t *)pin) & 0x01;
 }

 * For ST platform:

 uint8_t __Fuzyonel_Keypad_Interface_Key_Read(GPIO_TypeDef * port, uint16_t pin) {
     return HAL_GPIO_ReadPin(port, pin) ? 0 : 1;
 }

 * @param port The GPIO port for the pin (not used in this example).
 * @param pin Pointer to the specific pin to read.
 *
 * @return 1 if the pin is 'high', 0 if the pin is 'low'.
 */
uint8_t __Fuzyonel_Keypad_Interface_Key_Read(void* port, void* pin) {
    return (*(volatile uint8_t *)pin) & 0x01;
}

#ifdef __FUZYONEL_KEYPAD_DEBUG
/**
 * @brief Debug function for logging key events.
 *
 * This function is called by the library whenever a state transition occurs if
 * debug mode is enabled. The user can implement this function to log messages
 * to a console or other debug interface.
 *
 * @param message A string describing the debug event.
 * @param key The key structure associated with the event.
 */
void __Fuzyonel_Keypad_Interface_Debug(char* message, Fuz_Key_t key) {

}
#endif
