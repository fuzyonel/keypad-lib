# Keypad Library

## Overview
The Keypad Library provides a flexible and portable solution for managing keypads and individual buttons in embedded systems. It supports:
- 3x4 matrix keypads
- Simple button inputs
- Mixed setups combining both types

The library handles key state transitions (e.g., IDLE, PRESSED, HOLD, RELEASED) and offers debugging capabilities for easy development and troubleshooting.

## Features
- **Keypad Types:** Supports button-only, matrix keypads, and mixed configurations.
- **Configurable Timing:** Easily adjust button hold and idle times.
- **Debugging Support:** Log key transitions with optional verbose mode.
- **Portability:** Hardware abstraction for use with different platforms (e.g., PIC, STM32).

---

## Requirements

### Software
- **PIC Microcontrollers:** XC8 compiler.
- **STM32 Microcontrollers:** STM32 HAL.
- Any other controller with custom user implementation in step 2 below.

---

## Integration

### 1. Add the Library to Your Project
Include `keypad.c` and `keypad.h` in your project.

### 2. Implement the Hardware Interface
You must define the following hardware interface functions in your project:

```c
void __Fuzyonel_Keypad_Interface_Key_SetHigh(void* port, void* pin);
void __Fuzyonel_Keypad_Interface_Key_SetLow(void* port, void* pin);
uint8_t __Fuzyonel_Keypad_Interface_Key_Read(void* port, void* pin);
```

#### Example Implementation
For **PIC (XC8):**
```c
void __Fuzyonel_Keypad_Interface_Key_SetHigh(void* port, void* pin) {
    *(volatile uint8_t*)pin = 1;
}

void __Fuzyonel_Keypad_Interface_Key_SetLow(void* port, void* pin) {
    *(volatile uint8_t*)pin = 0;
}

uint8_t __Fuzyonel_Keypad_Interface_Key_Read(void* port, void* pin) {
    return (*(volatile uint8_t*)pin) & 0x01;
}
```

For **STM32 (HAL):**
```c
void __Fuzyonel_Keypad_Interface_Key_SetHigh(GPIO_TypeDef* port, uint16_t pin) {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
}

void __Fuzyonel_Keypad_Interface_Key_SetLow(GPIO_TypeDef* port, uint16_t pin) {
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

uint8_t __Fuzyonel_Keypad_Interface_Key_Read(GPIO_TypeDef* port, uint16_t pin) {
    return HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET ? 1 : 0;
}
```

### 3. Configure the Keypad
Define your keys and initialize the keypad structure in `main.c`:

```c
#define BUTTON_COUNT 4

Fuz_Key_t keys[BUTTON_COUNT];
Fuz_Keypad_t keypad;

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

    uint32_t counter = 0;

    for (;;) {
        counter++;
        Keypad_Scan(&keypad, counter);
        __delay_ms(1);
    }
}
```

---

## Example
### Debugging Transitions
Enable debugging by defining `__FUZYONEL_KEYPAD_DEBUG` in `keypad.h`. Optionally, enable verbose logging with `__FUZYONEL_KEYPAD_VERBOSE`.

Debug transitions will log messages like:
```
Key 1: Transition change: KEY_IDLE -> KEY_PRESSED
Key 2: Transition change: KEY_PRESSED -> KEY_HOLD
```

---

## Configuration
- `__FUZYONEL_KEYPAD_HOLD_TIME`: Time (ms) to consider a button held.
- `__FUZYONEL_KEYPAD_IDLE_TIME`: Time (ms) before a button transitions to idle.
- `__FUZYONEL_KEYPAD_DEBUG`: Enable debug logging.
- `__FUZYONEL_KEYPAD_VERBOSE`: Enable detailed debug logging.

---

## License
The MIT License is a permissive open-source license. Here's a short summary:

* **You can** => Use, copy, modify, merge, publish, distribute, sublicense, and sell the software.
* **You must** => Include the original license notice in any copies or substantial portions of the software.
* **No Warranty** => The software is provided "as is," without any warranty, and the author is not liable for any damages.

---

Feel free to submit issues or feature requests via GitHub!

