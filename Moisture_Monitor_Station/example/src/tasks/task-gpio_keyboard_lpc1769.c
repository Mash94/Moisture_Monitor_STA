/*--------------------------------------------------------------------*-
    task-gpio_led_lpc1769.c (Released 2019-04)

 	Simple GPIO_LED task for LPC1769.
	[Write Output on LPCXpresso baseboard.]

    Simple led interface code.
-*--------------------------------------------------------------------*/

// Project header
#include "task-gpio_keyboard_lpc1769.h"

#include "../main/main.h"

// Task header

#define NINGUNA (0xFF)

// ------ Public variable ------------------------------------------
extern int tecla;
int tecla_anterior = NINGUNA;
int key = NINGUNA;

// ------ Private constants ----------------------------------------
#define TEST_1 (1)	/* Test original task */
#define TEST_2 (2)	/* Test MEF blinking led */

#define TEST (TEST_2)

// ------ Private variable -----------------------------------------


/*------------------------------------------------------------------*-
    GPIO_LED_Init()
-*------------------------------------------------------------------*/
/*void GPIO_LED_Init(void)
{
	// Set up "GPIO" LED as an output pin
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_LED_PORT, GPIO_LED_PIN, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_LED_PORT, GPIO_LED_PIN);
}
*/
/*------------------------------------------------------------------*-
    GPIO_KEYBOARD_Init()
-*------------------------------------------------------------------*/
void GPIO_KEYBOARD_Init(void)
{
	// Columna 1
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_COL_PORT, GPIO_COL1_PIN, IOCON_MODE_PULLDOWN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_COL_PORT, GPIO_COL1_PIN);

	// Columna 2
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_COL_PORT, GPIO_COL2_PIN, IOCON_MODE_PULLDOWN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_COL_PORT, GPIO_COL2_PIN);

	// Columna 3
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_COL_PORT, GPIO_COL3_PIN, IOCON_MODE_PULLDOWN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_COL_PORT, GPIO_COL3_PIN);

	// Columna 4
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_COL_PORT, GPIO_COL4_PIN, IOCON_MODE_PULLDOWN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_COL_PORT, GPIO_COL4_PIN);

	// Fila 1
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_FILA_PORT, GPIO_FILA1_PIN, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA1_PIN);

	// Fila 2
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_FILA_PORT, GPIO_FILA2_PIN, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA2_PIN);

	// Fila 3
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_FILA_PORT, GPIO_FILA3_PIN, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA3_PIN);

	// Fila 4
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_FILA_PORT, GPIO_FILA4_PIN, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA4_PIN);
}

/*------------------------------------------------------------------*-
    GPIO_LED_Update()
-*------------------------------------------------------------------*/
/*void GPIO_LED_Update(void)
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_LED_PORT, GPIO_LED_PIN);
	Chip_GPIO_SetPinToggle(LPC_GPIO, GPIO_LED_PORT, GPIO_LED_PIN);
}*/

/*------------------------------------------------------------------*-
	GPIO_MUX_KEYBOARD_update()
-*------------------------------------------------------------------*/
void GPIO_MUX_KEYBOARD_update(void)
{
	uint8_t fila = 1;
	uint8_t columna_input = NINGUNA;

	key = NINGUNA;

	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA1_PIN, FILA_OFF);
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA2_PIN, FILA_OFF);
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA3_PIN, FILA_OFF);
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA4_PIN, FILA_OFF);

	while((fila < 5) && (key == NINGUNA)){
		switch(fila)
		{
			case 1:
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA2_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA3_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA4_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA1_PIN, FILA_ON);
				break;
			case 2:
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA1_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA3_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA4_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA2_PIN, FILA_ON);
				break;
			case 3:
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA1_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA2_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA4_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA3_PIN, FILA_ON);
				break;
			case 4:
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA1_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA2_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA3_PIN, FILA_OFF);
				Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_FILA_PORT, GPIO_FILA4_PIN, FILA_ON);
				break;
		}

		columna_input = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_COL_PORT, GPIO_COL1_PIN);

		if(columna_input != 0)
		{
			switch(fila){
				case 1:
					key = '1';
					break;
				case 2:
					key = '2';
					break;
				case 3:
					key = '3';
					break;
				case 4:
					key = 'A';
					break;
			}
		}else{
			columna_input = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_COL_PORT, GPIO_COL2_PIN);

			if(columna_input != 0)
			{
				switch(fila){
					case 1:
						key = '4';
						break;
					case 2:
						key = '5';
						break;
					case 3:
						key = '6';
						break;
					case 4:
						key = 'B';
						break;
				}
			}else{
				columna_input = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_COL_PORT, GPIO_COL3_PIN);

				if(columna_input != 0)
				{
					switch(fila){
						case 1:
							key = '7';
							break;
						case 2:
							key = '8';
							break;
						case 3:
							key = '9';
							break;
						case 4:
							key = 'C';
							break;
						}
				}else{
					columna_input = Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_COL_PORT, GPIO_COL4_PIN);

					if(columna_input != 0)
					{
						switch(fila){
							case 1:
								key = '*';
								break;
							case 2:
								key = '0';
								break;
							case 3:
								key = '#';
								break;
							case 4:
								key = 'D';
								break;
						}
					}else{
							key = NINGUNA;
					}
				}
			}
		}
		fila++;
    }
}


#if (TEST == TEST_2)
void GPIO_DEBOUNCE_KEYBOARD_Update(void){

	static uint8_t previa = NINGUNA;
	static uint32_t index = 0;
	uint32_t sw_touch = 0;

	sw_touch = key;


	if(sw_touch == previa){
		index++;
	}else index = 0;

	if((index >= 3) && (sw_touch != NINGUNA)){
		tecla = sw_touch;
		index = 0;
	}
	/*else{
		tecla = NINGUNA;
	}
	*/
	previa = sw_touch;
}
#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
