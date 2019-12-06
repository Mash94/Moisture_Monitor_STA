/*
===============================================================================
 Name        : SD_SpiExample.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================


/*==================[inclusions]=============================================*/

#include "board.h"
#include "ff.h"

/*==================[macros and definitions]=================================*/

//#define FILENAME "newfile.txt" //11 caracteres
#define FILENAME "asd.txt"

/*==================[internal data declaration]==============================*/

static uint32_t ms_ticks;  /**< 1ms timeticks counter */
static FATFS fs;           /**< FatFs work area needed for each volume */
static FIL fp;             /**< File object needed for each open file */

/*==================[internal functions declaration]=========================*/

/** @brief Hardware initialization function
 *  @return none
 */
static void initHardware(void);

/*==================[internal functions definition]==========================*/

static void initHardware(void)
{
    Board_Init();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);

    /* SPI configuration */
    Board_SSP_Init(LPC_SSP1);
    Chip_SSP_Init(LPC_SSP1);
    Chip_SSP_Enable(LPC_SSP1);
}

/*==================[external functions definition]==========================*/

void SysTick_Handler(void)
{
    ms_ticks++;

    if (ms_ticks >= 10) {
        ms_ticks = 0;
        disk_timerproc();   /* Disk timer process */
    }
}

int main(void)
{
    UINT nbytes;

    initHardware();

    /* Give a work area to the default drive */
    if (f_mount(&fs, "", 0) != FR_OK) {
        /* If this fails, it means that the function could
         * not register a file system object.
         * Check whether the SD card is correctly connected */
    }

    /* Create/open a file, then write a string and close it */
    if (f_open(&fp, FILENAME, FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {
        f_write(&fp, "PRUEBA\r\n", 8, &nbytes);
        f_write(&fp, "DATO 1\r\n", 8, &nbytes);
        f_write(&fp, "DATO 2\r\n", 8, &nbytes);
        f_write(&fp, "DATO 3\r\n", 8, &nbytes);
        f_write(&fp, "DATO 4\r\n", 8, &nbytes);
        f_write(&fp, "MASH 5\r\n", 8, &nbytes);

        f_close(&fp);

        if (nbytes == 8) {
            /* Toggle a LED if the write operation was successful */
            Board_LED_Toggle(0,22);
        }
    }

    while (1) {
        __WFI();
    }

    return 0;
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
