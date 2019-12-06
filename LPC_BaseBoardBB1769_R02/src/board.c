/*
 * @brief BaseBoar1769_R02 Board File
 *
 * @note
 * Ing Ruben Lozano
 * Ing Fabio Marano
 *
 */

#include "board.h"
#include "string.h"

#include "retarget.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/


/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* System oscillator rate and RTC oscillator rate */
const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes board LED(s) */
static void Board_LED_Init(void)
{
	/* Pins are configured as GPIO pin during SystemInit */
	/* Set the pins as output */
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED0_GPIO_PORT_NUM, LED0_GPIO_BIT_NUM, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED1_GPIO_PORT_NUM, LED1_GPIO_BIT_NUM, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED2_GPIO_PORT_NUM, LED2_GPIO_BIT_NUM, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED3_GPIO_PORT_NUM, LED3_GPIO_BIT_NUM, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED4_GPIO_PORT_NUM, LED4_GPIO_BIT_NUM, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED5_GPIO_PORT_NUM, LED5_GPIO_BIT_NUM, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED6_GPIO_PORT_NUM, LED6_GPIO_BIT_NUM, true);
	Chip_GPIO_WriteDirBit(LPC_GPIO, LED7_GPIO_PORT_NUM, LED7_GPIO_BIT_NUM, true);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/




/* Initialize debug output via UART for board */
void Board_UART_Init(uint32_t pUART,uint32_t baud)
{

	Chip_UART_Init(pUART);
	Chip_UART_SetBaud(pUART, baud);
	Chip_UART_ConfigData(pUART, UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS);
	//Chip_UART_SetupFIFOS(pUART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	/* Enable UART Transmit */
	Chip_UART_TXEnable(pUART);

}

/* Sends a character on the UART */
void Board_UARTPutChar(uint32_t pUART,char ch)
{

	while ((Chip_UART_ReadLineStatus(pUART) & UART_LSR_THRE) == 0) {}
	Chip_UART_SendByte(pUART, (uint8_t) ch);

}

/* Gets a character from the UART, returns EOF if no character is ready */
int Board_UARTGetChar(uint32_t pUART)
{


		return (int) Chip_UART_ReadByte(pUART);


	return EOF;
}

/* Outputs a string on the debug UART */
void Board_UARTPutSTR(uint32_t pUART,char *str)
{

	while (*str != '\0') {
		Board_UARTPutChar(pUART,*str++);
	}

}

/* Sets the state of a board LED to on or off */
void Board_LED_Set(uint8_t port, uint8_t bitnum, bool On)
{

	Chip_GPIO_WritePortBit(LPC_GPIO, port, bitnum, On);

}

/* Returns the current state of a board LED */
bool Board_LED_Test(uint8_t port, uint8_t bitnum)
{
	bool state = false;

	state = Chip_GPIO_ReadPortBit(LPC_GPIO, port, bitnum);

	return state;
}

void Board_LED_Toggle(uint8_t port, uint8_t bitnum)
{
	Board_LED_Set(port,bitnum, !Board_LED_Test( port, bitnum));

}

void Board_RGB_Set(uint8_t port, uint8_t bitnum, bool On){

	Chip_GPIO_WritePortBit(LPC_GPIO, port, bitnum, On);
}

void Board_Buttons_Init(void)
{
	Chip_GPIO_WriteDirBit(LPC_GPIO, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM, false);
	Chip_GPIO_WriteDirBit(LPC_GPIO, BUTTONS_BUTTON2_GPIO_PORT_NUM, BUTTONS_BUTTON2_GPIO_BIT_NUM, false);
}

uint32_t Buttons_GetStatus(uint8_t port, uint8_t bitnum)
{
	uint8_t ret = NO_BUTTON_PRESSED;
	if (Chip_GPIO_ReadPortBit(LPC_GPIO, port, bitnum) == 0x00) {
		if(bitnum==BUTTONS_BUTTON1)
			ret |= BUTTONS_BUTTON1;
		else
			ret |= BUTTONS_BUTTON2;
	}
	return ret;
}

/* Set up and initialize all required blocks and functions related to the
   board hardware */
void Board_Init(void)
{
	/* Sets up DEBUG UART */
	DEBUGINIT();

	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO);
	Chip_IOCON_Init(LPC_IOCON);

	/* Initialize LEDs */
	Board_LED_Init();
	/* Initialize Tact Switch */
	Board_Buttons_Init();
}

/* Returns the MAC address assigned to this board */
void Board_ENET_GetMacADDR(uint8_t *mcaddr)
{
	const uint8_t boardmac[] = {0x00, 0x60, 0x37, 0x12, 0x34, 0x56};

	memcpy(mcaddr, boardmac, 6);
}

/* Initialize pin muxing for SSP interface */
void Board_SSP_Init(LPC_SSP_T *pSSP)
{
	if (pSSP == LPC_SSP1) {
		/* Set up clock and muxing for SSP1 interface */
		/*
		 * Initialize SSP0 pins connect
		 * P0.7: SCK
		 * P0.6: SSEL
		 * P0.8: MISO
		 * P0.9: MOSI
		 */
		Chip_IOCON_PinMux(LPC_IOCON, 0, 7, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 6, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 8, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 9, IOCON_MODE_INACT, IOCON_FUNC2);
	}
	else {
		/* Set up clock and muxing for SSP0 interface */
		/*
		 * Initialize SSP0 pins connect
		 * P0.15: SCK
		 * P0.16: SSEL
		 * P0.17: MISO
		 * P0.18: MOSI
		 */
		Chip_IOCON_PinMux(LPC_IOCON, 0, 15, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 16, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 17, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 18, IOCON_MODE_INACT, IOCON_FUNC2);
	}
}

/* Initialize pin muxing for SPI interface */
void Board_SPI_Init(bool isMaster)
{
	/* Set up clock and muxing for SSP0 interface */
	/*
	 * Initialize SSP0 pins connect
	 * P0.15: SCK
	 * P0.16: SSEL
	 * P0.17: MISO
	 * P0.18: MOSI
	 */
	Chip_IOCON_PinMux(LPC_IOCON, 0, 15, IOCON_MODE_PULLDOWN, IOCON_FUNC3);
	if (isMaster) {
		Chip_IOCON_PinMux(LPC_IOCON, 0, 16, IOCON_MODE_PULLUP, IOCON_FUNC0);
		Chip_GPIO_WriteDirBit(LPC_GPIO, 0, 16, true);
		Board_SPI_DeassertSSEL();

	}
	else {
		Chip_IOCON_PinMux(LPC_IOCON, 0, 16, IOCON_MODE_PULLUP, IOCON_FUNC3);
	}
	Chip_IOCON_PinMux(LPC_IOCON, 0, 17, IOCON_MODE_INACT, IOCON_FUNC3);
	Chip_IOCON_PinMux(LPC_IOCON, 0, 18, IOCON_MODE_INACT, IOCON_FUNC3);
}

/* Assert SSEL pin */
void Board_SPI_AssertSSEL(void)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, 0, 16, false);
}

/* De-Assert SSEL pin */
void Board_SPI_DeassertSSEL(void)
{
	Chip_GPIO_WritePortBit(LPC_GPIO, 0, 16, true);
}



/* Sets up board specific I2C interface */
void Board_I2C_Init(I2C_ID_T id)
{
	switch (id) {
	case I2C0:
		Chip_IOCON_PinMux(LPC_IOCON, 0, 27, IOCON_MODE_INACT, IOCON_FUNC1);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 28, IOCON_MODE_INACT, IOCON_FUNC1);
		Chip_IOCON_SetI2CPad(LPC_IOCON, I2CPADCFG_STD_MODE);
		break;

	case I2C1:
		Chip_IOCON_PinMux(LPC_IOCON, 0, 19, IOCON_MODE_INACT, IOCON_FUNC3);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 20, IOCON_MODE_INACT, IOCON_FUNC3);
		Chip_IOCON_EnableOD(LPC_IOCON, 0, 19);
		Chip_IOCON_EnableOD(LPC_IOCON, 0, 20);
		break;

	case I2C2:
		Chip_IOCON_PinMux(LPC_IOCON, 0, 10, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_PinMux(LPC_IOCON, 0, 11, IOCON_MODE_INACT, IOCON_FUNC2);
		Chip_IOCON_EnableOD(LPC_IOCON, 0, 10);
		Chip_IOCON_EnableOD(LPC_IOCON, 0, 11);
		break;
	}
}


void Serial_CreateStream(void *Stream)
{}

void Board_USBD_Init(uint32_t port)
{
	/* VBUS is not connected on the NXP LPCXpresso LPC1769, so leave the pin at default setting. */
	/*Chip_IOCON_PinMux(LPC_IOCON, 1, 30, IOCON_MODE_INACT, IOCON_FUNC2);*/ /* USB VBUS */
	
	Chip_IOCON_PinMux(LPC_IOCON, 0, 29, IOCON_MODE_INACT, IOCON_FUNC1);	/* P0.29 D1+, P0.30 D1- */
	Chip_IOCON_PinMux(LPC_IOCON, 0, 30, IOCON_MODE_INACT, IOCON_FUNC1);

	LPC_USB->USBClkCtrl = 0x12;                /* Dev, AHB clock enable */
	while ((LPC_USB->USBClkSt & 0x12) != 0x12); 
}

