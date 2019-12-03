/*------------------------------------------------------------------*-

    port_lpc1769.h (Released 2019-04)

  ------------------------------------------------------------------

    This is the "Port Header" file: it documents usage of port pins
    in the project.

-*------------------------------------------------------------------*/

#ifndef _PORT_H
#define _PORT_H 1


// ------ Public constants -----------------------------------------
#define TIM_PRESCALE_USVAL	(100)


// Allows Active Low or Active High led to be used (or other wiring variations)
#define LED_ON	(true)
#define LED_OFF	(false)
#define OFF 0
#define ON 1

#define COLUM1 0
#define LINE1 0x00
#define LINE2 0x01
#define LINE3 0x02
#define LINE4 0x03


// Heartbeat LED
// Connected to "LED2" on LPC1769 board
// Port 0, Pin 22
#define HEARTBEAT_LED_PORT (0)
#define HEARTBEAT_LED_PIN (22)
#define HEARTBEAT_LED_PIN_MODE (IOCON_MODE_INACT)
#define HEARTBEAT_LED_PIN_FUNC (IOCON_FUNC0)

// defines puerto serie UART0
#define UART0_PORT      (0)
#define UART0_TX_PIN    (2)
#define UART0_RX_PIN    (3)

// defines puerto serie UART0
#define UART1_PORT      (0)
#define UART1_TX_PIN    (15)
#define UART1_RX_PIN    (16)

// I2C
#define GPIO_I2C_PORT   (0)
#define GPIO_I2C_SDA    (0)  //0
#define GPIO_I2C_SCL    (1)  //1

// LCD
#define GPIO_LCD_PORT2  (2) ///SEGUN UNIVERSAL
#define GPIO_LCD_D4     (13)

#define GPIO_LCD_PORT0  (0) ///SEGUN UNIVERSAL
#define GPIO_LCD_D5     (28)

#define GPIO_LCD_PORT1  (1) ///SEGUN UNIVERSAL
#define GPIO_LCD_D6     (31)
#define GPIO_LCD_D7     (30)

#define GPIO_LCD_CONTROL_PORT (2) ///SEGUN UNIVERSAL
#define GPIO_LCD_RS     (3)
#define GPIO_LCD_RW     (1)
#define GPIO_LCD_EN     (2)
//#define GPIO_LCD_BL ()

// DHT22
#define GPIO_DHT22_PORT (0) ///SEGUN UNIVERSAL
#define GPIO_DHT22_DATA (3)

//TECLADO

#define GPIO_COL_PORT   (2) ///SEGUN UNIVERSAL

#define GPIO_COL1_PIN   (12)
#define GPIO_COL2_PIN   (11)
#define GPIO_COL3_PIN   (10)
#define GPIO_COL4_PIN   (8)

#define GPIO_FILA_PORT  (2) ///SEGUN UNIVERSAL

#define GPIO_FILA1_PIN  (7)
#define GPIO_FILA2_PIN  (6)
#define GPIO_FILA3_PIN  (5)
#define GPIO_FILA4_PIN  (4)

#define FILA_OFF        (0)
#define FILA_ON         (1)


// Add jumper wire on baseboard to control WDT
// WDT is enabled *only* if jumper is in place.
// (Jumper is read at init phase only)
// Port 2, Pin 3 => Connected to "SW2 UP" on LPCXpresso baseboard
#define WDT_JUMPER_PORT (1)
#define WDT_JUMPER_PIN (18)
#define WDT_JUMPER_PIN_MODE (IOCON_MODE_PULLUP)
#define WDT_JUMPER_PIN_FUNC (IOCON_FUNC0)


// ------ Public data type declarations ----------------------------


// ------ Public function prototypes -------------------------------


#endif


/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
