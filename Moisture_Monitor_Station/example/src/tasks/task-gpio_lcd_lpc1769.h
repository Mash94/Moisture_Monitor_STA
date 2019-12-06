/*--------------------------------------------------------------------*-

    task-gpio_lcd_lpc1769.h (Released 2019-04)

--------------------------------------------------------------------

    - See task-gpio_lcd_lpc1769.c for details.

-*--------------------------------------------------------------------*/

#ifndef _GPIO_LCD_H
#define _GPIO_LCD_H 1

// ------ Public constants -----------------------------------------
#define DATA 1
#define CMND 0

// ------ Public data type -----------------------------------------

// ------ Public function prototypes -------------------------------
void GPIO_LCD_Update(void);
void LCD_Init(void);
void LCD4_EscribirHW(uint8_t, uint8_t);

void DemoramS(uint32_t);
void DemorauS(uint32_t);

void lcd_gotoxy(uint32_t , uint32_t);
void lcd_backlight_on(void);
void lcd_backlight_off(void);
void lcd_clear(void);

void LCD_wr(unsigned char);

void CargarString(char* ,char* );
void Maquina_Estados(void);

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
