/*--------------------------------------------------------------------*-
    task-gpio_lcd_lpc1769.c (Released 2019-04)

 	Simple GPIO_LCD task for LPC1769.
-*--------------------------------------------------------------------*/

// Project header
#include "../main/main.h"

// Task header
#include "task-gpio_LCD_I2C_lpc1769.h"


// -------- Public variable ------------------------------------------
I2C_ID_T Display_I2C0;

// -------- Private constants ----------------------------------------
#define SPEED_100KHZ            (100000)
#define ACTIVITY_MAaaSK           (0x003FFFFF)

#define I2C_ADDR_7BIT                  (0x40)
#define I2C_REG_ADDR_7BIT              (0x03)

//---lcd----
/*#define SDA PTAD_PTAD2
#define SCL PTAD_PTAD3
#define DDR_SDA PTADD_PTADD2 // bit config. E/S pin SDA
#define DDR_SCL PTADD_PTADD3 // bit config. E/S pin SDA
*/


#define SDA_OFF 0
#define SDA_ON 1
#define SCL_OFF 0
#define SCL_ON 1

/* ----------------------------------------------- INI defines para LCD --- */
#define DATO 1
#define CMND 0
#define DELY 50

#define SLAVEADD 0x27
/* ----------------------------------------------- FIN defines para LCD --- */

/*   variable necesaria para I2C   */
unsigned char i2c_error; // bit error

/*   funciones para lectura / escritura de un bytes (random read / write) */
void escribir_byte(unsigned char dato);
unsigned int i;

I2C_ID_T DisplayI2C;

void GPIO_I2C_Init(void)
{
	Chip_I2C_Init(DisplayI2C);
	Chip_I2C_SetClockRate(DisplayI2C, SPEED_100KHZ);
}


/*
 Env�a byte por I2C al PCF8574
 Orden de escritura de bits a placa 8574

 D7 D6 D5 D4 BL E RW RS

 BL = Backlight (en este ejemplo siempre ON)
 */

/*
void escribir_byte(unsigned char dato) {
	unsigned char byte_dire = 0;
	// start
	i2c_start();
	i2c_tx(0b01001110); // device id 0100111+Write
						// (0x27 + Write)
	I2Cdelay();
	// enviar dato
	i2c_tx(dato);
	I2Cdelay();
	// stop
	i2c_stop();
}

// --- funciones generales para IIC ---
void I2Cdelay(void) {
	unsigned char i;
	for (i = 0; i < 10; i++);
}

void i2c_tx(unsigned char byte) {
	unsigned char i;

	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT,GPIO_I2C_SDA);    // SDA salida
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT,GPIO_I2C_SCL);    // SCL salida

	for (i = 0; i < 8; i++) {
		if (byte & 0x80)
			Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, SDA_ON);// SDA = 1; //envia cada bit, comenzando por el MSB
		else
			Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, SDA_OFF);// SDA = 0;
		Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, SCL_ON);  // SCL = 1;
		I2Cdelay();
		Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, SCL_OFF); //SCL = 0;
		I2Cdelay();
		byte = byte << 1;
	}
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT,GPIO_I2C_SDA);    // Esera ACK (config. pin como lectura)
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, SCL_ON);  // SCL = 1;
	I2Cdelay();

	if (Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA))
		i2c_error = 0x01; // error si el ACK no es correcto
	else
		i2c_error = 0;
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, SCL_OFF);  // SCL = 0;
	I2Cdelay();
}

void i2c_start() {
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_I2C_PORT, GPIO_I2C_SDA, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_I2C_PORT, GPIO_I2C_SCL, IOCON_MODE_INACT, IOCON_FUNC0);

	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT,GPIO_I2C_SDA);    // SDA salida
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT,GPIO_I2C_SCL);    // SCL salida

	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, SDA_ON);// SDA = 1; // garantizamos el estado inicial de SDA y SCL (reposo)
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, SCL_ON);// SCL = 1;
	I2Cdelay();
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, SDA_OFF);//SDA = 0; // condicion START
	I2Cdelay();
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, SCL_OFF);// SCL = 0;
	I2Cdelay();
}

void i2c_addr(unsigned char addr, unsigned char rw) {
	// addr, direcci�n (7 bits)
	// rw=1, lectura
	// rw=0, escritura
	i2c_tx(addr << 1 + rw); // envia byte de direccion
}

unsigned char i2c_rx(unsigned char ultimo)
// ultimo = NACK = 1 para el ultimo byte
// ultimo = ACK = 0 para cualquier byte que no sea el �ltimo.
{
	unsigned char i, byte;
	byte = 0;
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_I2C_PORT,GPIO_I2C_SDA);    //SDA entrada
	for (i = 0; i < 8; i++) // lee un bit comenzando por el MSB
			{
		//SCL=1;
		do {
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT,GPIO_I2C_SCL);    // SCL salida
			Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, SCL_ON);// SCL = 1;
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_I2C_PORT,GPIO_I2C_SCL);    // SCL = entrada;
		} while (Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL) == 0); // SCL ck stretching?
		Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT,GPIO_I2C_SCL);    // SCL salida
		I2Cdelay();
		byte <<= 1;
		if (Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA))
			byte++;
		Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, SCL_OFF);// SCL = 0;
		I2Cdelay();
	}
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT,GPIO_I2C_SDA); //DDR_SDA = salida; // SDA salida
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, ultimo);  //SDA = ultimo; // ACK es funcion de ultimo
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, SCL_ON);// SCL = 1 // ACK=0, si no es �ltimo byte le�do
	I2Cdelay(); // ACK=1, despu�s del �ltimo byte le�do (NACK)
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, SCL_OFF);// SCL = 0;
	I2Cdelay();
	return (byte);
}

void i2c_stop(void) {
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT,GPIO_I2C_SDA); //DDR_SDA = salida; // SDA salida
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, SDA_OFF); //SDA = 0;
	I2Cdelay();
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, SCL_ON); //SCL = 1;
	I2Cdelay();
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, SDA_ON); //SDA = 1;
	I2Cdelay();
}
*/








//static I2CM_XFER_T  i2cmXferRec;
// -------- Private variable -----------------------------------------

// -------- Functions ------------------------------------------------
/*
void GPIO_I2C_Init(void)
{
	Chip_I2C_Init(Display); // Initializes the LPC_I2C peripheral with specified parameter
	Chip_I2C_SetClockRate(Display_I2C0, SPEED_100KHZ);

}
void GPIO_I2C_Update(void){
	int a = Chip_I2C_MasterSend(Display_I2C0, I2C_ADDR_7BIT, *BuffTx , len); // Master tx only
}


// Function to setup and execute I2C transfer request
static void SetupXferRecAndExecute(uint8_t devAddr,
								   uint8_t *txBuffPtr,
								   uint16_t txSize,
								   uint8_t *rxBuffPtr,
								   uint16_t rxSize)
{
	// Setup I2C transfer record
	i2cmXferRec.slaveAddr = devAddr;
	i2cmXferRec.options = 0;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize;
	i2cmXferRec.rxSz = rxSize;
	i2cmXferRec.txBuff = txBuffPtr;
	i2cmXferRec.rxBuff = rxBuffPtr;
	Chip_I2CM_XferBlocking(LPC_I2C0, &i2cmXferRec);
}

// Perform I2CM write on target board
static void WriteBoard_I2CM(int writeVal)
{
	uint8_t tx_buffer[3];

#if (defined(BOARD_HITEX_EVA_1850) || defined(BOARD_HITEX_EVA_4350))
	// Test for non-zero write pattern
	if (writeVal != 0) {
		// Setup all-bit write pattern
		writeVal = 0xFF;
	}

	// Setup output buffer
	tx_buffer[0] = writeVal;
	tx_buffer[1] = writeVal;

	// Write 0000 or FFFF via I2CM to PCA9673PW CHIP on 1850/4350 board
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 2, NULL, 0);
#elif (defined(BOARD_KEIL_MCB_1857) || defined(BOARD_KEIL_MCB_4357))
	// Write value via I2CM to STMPE811 CHIP ID REG on 1857/4357 board
	tx_buffer[0] = I2C_REG_ADDR_7BIT;
	tx_buffer[1] = writeVal & 1;
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 2, NULL, 0);
#elif (defined(BOARD_NGX_XPLORER_1830) || defined(BOARD_NGX_XPLORER_4330))
	// Write audio power value via I2CM to UDA1380 part on 1830/4330 board
	tx_buffer[0] = I2C_REG_ADDR_7BIT;

	// Test for non-zero write pattern
	if (writeVal != 0) {
		// Turn all audio power on
		tx_buffer[1] = 0xA5;
		tx_buffer[2] = 0xDF;
	}
	else {
		// Turn all audio power off
		tx_buffer[1] = 0;
		tx_buffer[2] = 0;
	}

	// Write audio power value
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 3, NULL, 0);
#elif defined(BOARD_NXP_LPCLINK2_4370)
	// This transfer will NAK since no on-board I2C devices exist
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 3, NULL, 0);
#elif defined(BOARD_NXP_LPCXPRESSO_4337)
	// set configuration to default value
	tx_buffer[0] = 0x00; // Write to Config register
	tx_buffer[1] = 0x39;
	tx_buffer[2] = 0x9F;
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 3, NULL, 0);
#else
#warning "WARNING: Unknown board for I2CM write operation!"
#endif
}

// Perform I2CM read on target board
static void ReadBoard_I2CM()
{
	uint8_t tx_buffer[3];
	uint8_t rx_buffer[3];

#if (defined(BOARD_HITEX_EVA_1850) || defined(BOARD_HITEX_EVA_4350))
	// Read register via I2CM from STMPE811 CHIP ID REG on 1850/4350 board
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 0, rx_buffer, 2);
#elif (defined(BOARD_KEIL_MCB_1857) || defined(BOARD_KEIL_MCB_4357))
	// Read value via I2CM from STMPE811 CHIP ID REG on 1857/4357 board
	tx_buffer[0] = I2C_REG_ADDR_7BIT;
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 1, rx_buffer, 2);
#elif (defined(BOARD_NGX_XPLORER_1830) || defined(BOARD_NGX_XPLORER_4330))
	// Read value via I2CM from UDA1380 part on 1830/4330 board
	tx_buffer[0] = I2C_REG_ADDR_7BIT;
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 1, rx_buffer, 2);
#elif defined(BOARD_NXP_LPCLINK2_4370)
	// This transfer will NAK since no on-board I2C devices exist
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 0, rx_buffer, 0);
#elif defined(BOARD_NXP_LPCXPRESSO_4337)
	tx_buffer[0] = I2C_REG_ADDR_7BIT; // Read the Voltage across the shunt
	rx_buffer[0] = 0;
	rx_buffer[1] = 0;
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 1, rx_buffer, 2);
	DEBUGOUT("Voltage Reading across shunt: 0x%02X%02X\r\n", rx_buffer[0], rx_buffer[1]);
#else
#warning "WARNING: Unknown board for I2CM read operation!"
#endif
}*/
/****************************************************************************************
 * Function Name : GPIO_LCD_Update();
 * Description :
 * Input :
 * Output : Void
 * Note :
****************************************************************************************/
