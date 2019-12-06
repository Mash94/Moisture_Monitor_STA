/*--------------------------------------------------------------------*-
    task-SD_lpc1769.c (Released 2019-04)

 	Simple SD handler task for LPC1769.
-*--------------------------------------------------------------------*/

// Project header
#include "../main/main.h"

// Task header
#include "task-SD_lpc1769.h"
#include "ff.h"
#include "board.h"

// -------- Public variable ------------------------------------------
#define FILENAME "mash.txt"

// -------- Global  variable -----------------------------------------
extern int temperatura;
extern int humedad;
extern int tecla;

// -------- Private constants ----------------------------------------

// -------- Private variable -----------------------------------------
static FATFS fs;           /**< FatFs work area needed for each volume */
static FIL fp;             /**< File object needed for each open file */

// -------- Functions ------------------------------------------------
/****************************************************************************************
 * Function Name : void GPIO_DHT22_update(void);
 * Description : Blocking read
 * Input :
 * Output :
 * Void Note :
****************************************************************************************/
void GPIO_SD_Update(void) {
	UINT nbytes;
	char hum[8];
	char tem[8];
	char nuevoMed[11];
	static char tecla_anterior = 0xFF;

	static int i = 0;
	static int j = 1;

	if(i < 10000 && tecla == '1'){
		/* Give a work area to the default drive */
		if (f_mount(&fs, "", 0) != FR_OK) {
			/* If this fails, it means that the function could
			 * not register a file system object.
			 * Check whether the SD card is correctly connected */
		}
		CargarSD (tem , temperatura, 1);
		CargarSD (hum , humedad, 0);

		/* Create/open a file, then write a string and close it */
		if (f_open(&fp, FILENAME, FA_OPEN_APPEND | FA_OPEN_ALWAYS | FA_WRITE) == FR_OK) {
			f_write(&fp, tem, 8, &nbytes);
			f_write(&fp, hum, 8, &nbytes);

			f_close(&fp);
		}
		i++;
	}
	if(tecla_anterior == '1' && tecla == 'A'){
		CargarSDM(nuevoMed , j);
		if (f_open(&fp, FILENAME, FA_OPEN_APPEND | FA_OPEN_ALWAYS | FA_WRITE) == FR_OK) {
			f_write(&fp, nuevoMed, 11, &nbytes);
			f_close(&fp);
		}
		j++;
	}
	tecla_anterior = tecla;
}
/****************************************************************************************
 * Function Name : GPIO_DHT22_Init();
 * Description :
 * Input :
 * Output : Void
 * Note :
****************************************************************************************/
void GPIO_SD_Init(void)
{
    /* SPI configuration */
    Board_SSP_Init(LPC_SSP1);
    Chip_SSP_Init(LPC_SSP1);
    Chip_SSP_Enable(LPC_SSP1);
}
/****************************************************************************************
 * Function Name : void CargarSD (char* vector , int valor, int tipo);
 * Description :
 * Input :
 * Output : Void
 * Note :
****************************************************************************************/
void CargarSD (char* vector , int valor, int tipo){
	vector[0] = (valor / 100 + 0x30 );
	vector[1] = (((valor / 10) % 10) + 0x30 );
	vector[2] = '.';
	vector[3] = ((valor %10) + 0x30 );
	vector[4] = '0';
	if (tipo)
		vector[5] = 'C';
	else
		vector[5] = 'H';
	vector[6] = 'F';
	vector[7] = '\n';
}
/****************************************************************************************
 * Function Name : void CargarSDM (char* vector , int valor);
 * Description :
 * Input :
 * Output : Void
 * Note :
****************************************************************************************/
void CargarSDM (char* vector , int valor){
	vector[0] = 'M';
	vector[1] = 'e';
	vector[2] = 'd';
	vector[3] = 'i';
	vector[4] = 'c';
	vector[5] = 'i';
	vector[6] = 'o';
	vector[7] = 'n';
	vector[8] = ' ';
	vector[9] = valor + 0x30;
	vector[10] = '\n';
}
