/*--------------------------------------------------------------------*-

    task-gpio_DHT22_lpc1769.h (Released 2019-04)

--------------------------------------------------------------------

    - See task-gpio_DHT22_lpc1769.c for details.

-*--------------------------------------------------------------------*/

// ------ Public constants -----------------------------------------
#define DATA 1
#define CMND 0

// ------ Public data type -----------------------------------------

// ------ Public function prototypes -------------------------------
void GPIO_DHT22_Update(void);
float readHumidity(void);
float readTemperature(void);
uint32_t expectPulse(uint32_t);
void GPIO_DHT22_Init(void);

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
