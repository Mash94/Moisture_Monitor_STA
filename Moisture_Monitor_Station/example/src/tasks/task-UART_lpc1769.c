/*--------------------------------------------------------------------*-
    task-UART_lpc1769.c (Released 2019-04)

 	Simple UART task for LPC1769.
-*--------------------------------------------------------------------*/

// Project header
#include "../main/main.h"

// Task header
#include "task-UART_lpc1769.h"


// ------ Private constants ----------------------------------------

//#define LPC_UART0 (LPC_UART0)	/* Defined uart */

/* Transmit  ring buffer size */
#define UART_SRB_SIZE (128)	/* Send */
//#define UART_RRB_SIZE (128)	/* Receive */

#define MAX_DURATION (1000)

#define CHARBUFF (7)

extern int temperatura;
extern int humedad;
extern int tecla;


/* Transmit buffers */
static uint8_t txbuff[UART_SRB_SIZE];
//static uint8_t rxbuff[UART_RRB_SIZE];

// ------ Private data type ----------------------------------------

// ------ Private variable -----------------------------------------
//static uint32_t Time_in_state_G = MAX_DURATION;

// ------ Public variable ------------------------------------------
/* Transmit and receive ring buffers */
RINGBUFF_T txring, rxring;
bool blockUART = 1;

uint8_t VecTEM[CHARBUFF]="25.50CF";
uint8_t VecHUM[CHARBUFF]="40.00HF";


/****************************************************************************************
 * Function Name : uint32_t expectPulse(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
void UART0_Init(void)
{
	Chip_IOCON_PinMux(LPC_IOCON, UART0_PORT, UART0_TX_PIN, IOCON_MODE_INACT, IOCON_FUNC1);	// P0.2 as Txd0
    //Chip_IOCON_PinMux(LPC_IOCON, UART0_PORT, UART0_RX_PIN, IOCON_MODE_INACT, IOCON_FUNC1); //  P0.3 as Rxd0

    /* Setup UART for 115.2K8N1 */
	Chip_UART_Init(LPC_UART0);
	Chip_UART_SetBaud(LPC_UART0, 9600);
	Chip_UART_ConfigData(LPC_UART0, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT |UART_LCR_PARITY_DIS));
	Chip_UART_TXEnable(LPC_UART0);

	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);
	//RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	Chip_UART_SetupFIFOS(LPC_UART0, (UART_FCR_FIFO_EN | UART_FCR_RX_RS |	UART_FCR_TX_RS | UART_FCR_TRG_LEV2));
}
/****************************************************************************************
 * Function Name : void UART0_Update(void);
 * Description :
 * Input :
 * Output :
 * Void Note :
****************************************************************************************/
void UART0_Update(void)
{
	static bool cont = FALSE;

	if(tecla == '2'){
		CargarUART(VecTEM,temperatura,1);
		CargarUART(VecHUM,humedad,0);

		if(cont && blockUART == 1){
			Chip_UART_SendBlocking(LPC_UART0, VecTEM,CHARBUFF);
			cont = !cont;
		}else if(!cont && blockUART == 1){
			Chip_UART_SendBlocking(LPC_UART0, VecHUM,CHARBUFF);
			cont = !cont;
		}
	}
}

void CargarUART (char* vector , int valor, int tipo){
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
}
/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
