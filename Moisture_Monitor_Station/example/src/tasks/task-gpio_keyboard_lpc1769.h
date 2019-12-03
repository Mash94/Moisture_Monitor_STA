/*--------------------------------------------------------------------*-

    task-gpio_led_lpc1769.h (Released 2019-04)

--------------------------------------------------------------------

    - See task-gpio_keyboard_lpc1769.c for details.

-*--------------------------------------------------------------------*/

#ifndef _GPIO_KEYBOARD_H
#define _GPIO_KEYBOARD_H 1

// ------ Public constants -----------------------------------------


// ------ Public data type -----------------------------------------


// ------ Public function prototypes -------------------------------
void GPIO_LED_Init(void);
void GPIO_KEYBOARD_Init(void);

void GPIO_LED_Update(void);
void GPIO_DEBOUNCE_KEYBOARD_Update(void);
void GPIO_MUX_KEYBOARD_update(void);

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
