/*--------------------------------------------------------------------*-

    task-gpio_LCD_I2C_lpc1769.h (Released 2019-04)

--------------------------------------------------------------------

    - See task-gpio_LCD_I2C_lpc1769.c for details.

-*--------------------------------------------------------------------*/

// ------ Public constants -----------------------------------------


// ------ Public data type -----------------------------------------

// ------ Public function prototypes -------------------------------

void GPIO_I2C_Init(void);
void GPIO_I2C_Update(void);

/* -----------------------------------INI prototipos funciones para LCD --- */
void LCD_init(void);
void LCD_wr(unsigned char);
void LCD_send(unsigned char, unsigned char);
void printstr(char* s);
void delay(unsigned int);

/*   funciones I2C   */
void I2Cdelay(void);
void i2c_tx(unsigned char byte);
void i2c_addr(unsigned char addr, unsigned char rw);
unsigned char i2c_rx(unsigned char ultimo);
void i2c_stop(void);
void i2c_start(void);

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
