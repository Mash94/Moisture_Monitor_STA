/*--------------------------------------------------------------------*-
    task-gpio_lcd_lpc1769.c (Released 2019-04)

 	Simple GPIO_LCD task for LPC1769.
-*--------------------------------------------------------------------*/

// Project header
#include "../main/main.h"

// Task header
#include "task-gpio_lcd_lpc1769.h"


// -------- Public variable ------------------------------------------
//Variables LCD-I2C
unsigned char RS = 0, E, DATABUS, DATOI2C;

unsigned char i2c_error; // bit error

#define PCF_DEVICE_ID   0x4E  // (0x27 + write)

// -------- Private constants ----------------------------------------
#define ESCRIBIR true
#define SLAVEADD 0x27

#define DATO 1
#define CMND 0
#define DELY 50
#define NINGUNA (0xFF)

char Linea1[22]="Moisture Monitor STA";
char Linea2[22]="Medicion:";
char Linea3[22]="1-Guardar";
char Linea4[22]="2-Enviar Muestras";

// -------- Private variable -----------------------------------------
extern I2C_ID_T DisplayI2C;
// -------- Functions ------------------------------------------------

extern int temperatura;
extern int humedad;
extern int tecla;

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

/****************************************************************************************
 * Function Name : GPIO_LCD_Update();
 * Description :
 * Input :
 * Output : Void
 * Note :
****************************************************************************************/
void GPIO_LCD_Update(void)
{
	static int linea = 0;

    Maquina_Estados();

	if(linea == 0){
		LCD_send(0x80 , CMND); //0x80 escribe la primera linea
		printstr(Linea1);
	}else if(linea == 1){
		LCD_send(0x80+0x40 , CMND); //0xC0 escribe la segunda linea
		printstr(Linea2);
		LCD_send(0x80+0x40 + 9, CMND); //0x94 escribe la tercera linea
				LCD_send((temperatura / 100 + 0x30 ), DATO);
				LCD_send((((temperatura / 10)%10) + 0x30 ), DATO);
				LCD_send(',', DATO);
				LCD_send(((temperatura %10) + 0x30 ), DATO);
				LCD_send('C', DATO);
				LCD_send(' ', DATO);

				LCD_send((humedad / 100 + 0x30 ), DATO);
				LCD_send((((humedad / 10)%10) + 0x30 ), DATO);
				LCD_send(',', DATO);
				LCD_send(((humedad %10) + 0x30 ), DATO);
				LCD_send('%', DATO);

	}else if(linea==2){
		LCD_send(0x80+0x14 , CMND); //0x94 escribe la tercera linea
		printstr(Linea3);

 	}else if(linea==3){
		LCD_send(0x80+0x54 , CMND); //0xD4 escribe la cuartaa linea
		printstr(Linea4);
	}


	linea++;
	if (linea > 3)
		linea = 0;

}
/****************************************************************************************
 * Function Name : void lcd_clear( void);
 * lcd_clear()
 * Description :
 * Input :
 * Output : Void
 * Note :
 ***************************************************************************************/
void lcd_clear( void) {
	LCD4_EscribirHW( 0x01, CMND );
}
/****************************************************************************************
 * Function Name : void lcd_gotoxy(uint32_t , uint32_t);
 * Description :
 * Input :
 * Output : Void
 * Note :
****************************************************************************************/
void lcd_gotoxy(uint32_t x, uint32_t y) {
	if ((y > 3) && (x > 19)) {
		x = 0;
		y = 0;
	}
	if (y == 0) {
		LCD4_EscribirHW(0x80 + x, CMND);
	} else if (y == 1) {
		LCD4_EscribirHW(0xC4 + x, CMND);
	} else if (y == 2) {
		LCD4_EscribirHW(0x94 + x, CMND);
	} else if (y == 3) {
		LCD4_EscribirHW(0xD4 + x, CMND);
	}
}
/****************************************************************************************
 * Function Name : void DemoramS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Miliseconds
 * Output :
 * Void Note :
****************************************************************************************/
void DemoramS(uint32_t milis){
	//uint32_t time = 0;
	uint32_t i,j;

	//DWT->CYCCNT=0;
	for(i=0 ; i<milis ; i++){
		for (j=0 ; j<9088 ; j++);
	}
	//time = DWT->CYCCNT;
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
void DemorauS(uint32_t micros){
	//uint32_t time = 0;
	uint32_t i,j;

	//DWT->CYCCNT=0;
	for(i=0 ; i<micros ; i++){
		for (j=0 ; j<8 ; j++);
	}
	//time = DWT->CYCCNT;
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
void escribir_byte(unsigned char dato) {
    //unsigned char byte_dire;
    /* start */
    i2c_start();
    i2c_tx(PCF_DEVICE_ID); // device id 0100111+Write
                        // (0x27 + Write)
    I2Cdelay();
    /* enviar dato */
    i2c_tx(dato);
    I2Cdelay();
    /* stop */
    i2c_stop();
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
/* --- funciones generales para IIC --- */
void I2Cdelay(void) {
    unsigned int i;
    for (i = 0; i < 50; i++)
        ;
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
void i2c_tx(unsigned char byte) {
    unsigned char i;

    // SDA salida
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA);

    for (i = 0; i < 8; i++) {
        if (byte & 0x80)
            // SDA = 1; //envia cada bit, comenzando por el MSB
        	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, ON);
        else
        	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, OFF);


        Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, ON);

        // SCL = 1;
        I2Cdelay();

        Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, OFF);
        // SCL = 0;
        I2Cdelay();
        byte = byte << 1;
    }

    // SDA entrada
    Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA);

    Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, ON);
    //SCL = 1;
    I2Cdelay();

    if (Chip_GPIO_ReadPortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA))
        i2c_error = 0x01; // error si el ACK no es correcto
    else
        i2c_error = 0;

    Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, OFF);
    //SCL = 0;
    I2Cdelay();
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
void i2c_start() {
    // SDA salida
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL);

    // SDA = 1; // garantizamos el estado inicial de SDA y SCL (reposo)
    Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, ON);

    //SCL = 1;
    Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, ON);
    I2Cdelay();

    // SDA = 0; // condicion START
    Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, OFF);

    I2Cdelay();

    //SCL = 0;
    Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, OFF);

    I2Cdelay();
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
void i2c_stop(void) {
    // SDA salida
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA);

    // SDA = 0;
    Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, OFF);

    I2Cdelay();

    //SCL = 1;
    Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SCL, ON);

    I2Cdelay();
    //SDA = 1;
    Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_I2C_PORT, GPIO_I2C_SDA, ON);

    I2Cdelay();
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
/* ----------------------------------------------------- muestra string --- */
void printstr(char* s) {
    int i = 0;
    while (s[i] != '\0') {
        LCD_send(s[i], DATO);
        i++;
    }
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
/* -------------------------------------------------------- retardo LCD --- */
void delay(unsigned int n) {
	DemorauS(n);
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
/* ------------------------------------------------- inicialización LCD --- */
void LCD_Init() {
    delay(50000);
    delay(50000);
    delay(50000);
    LCD_wr(0x03);
    LCD_wr(0x03);
    LCD_wr(0x03);
    /* --- modo 4 bit --- */
    LCD_wr(0x02);
    /* --- modo 4b - 2 lin - car 5x7 --- */
    LCD_send(0x28, CMND);
    /* --- apaga display --- */
    LCD_send(0x08, CMND);
    /* --- Clear --- */
    LCD_send(0x01, CMND);
    delay(50000);
    /* --- disp on - cur off - blink off --- */
    LCD_send(0x0C, CMND);
    /* --- inc - no scroll --- */
    LCD_send(0x06, CMND);
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
/* ----------------------------------------------------- escribe 4 bits --- */
/* --------------------------------------------------- en patas B[3..0] --- */
void LCD_wr(unsigned char data) {
    // D7 D6 D5 D4  0 E RW RS

    E = 1;
    E = E << 2;
    DATOI2C = 0x08 | E | RS | (data << 4);
    escribir_byte(DATOI2C);
    delay(1000);

    E = 0;
    E = E << 2;
    DATOI2C = 0x08 | E | RS | (data << 4);
    escribir_byte(DATOI2C);
    delay(1000);
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
/* -------------- escribe 4 MSb, luego 4 LSb Si ope = 1 DATO, si 0 CMND --- */
void LCD_send(unsigned char data, unsigned char ope) {
    unsigned char half;
    if (ope != 0)
        RS = 1;
    else
        RS = 0;
    half = data & 0xF0;
    half = half >> 4;
    LCD_wr(half);
    half = data & 0x0F;
    LCD_wr(half);
    RS = 0;
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
void CargarString(char* Linea, char* String){
	int i;
	for(i=0; i<=20; i++)
	{
		Linea[i]=String[i];
		if(String[i] == '\0')
			return;
	}
}
/****************************************************************************************
 * Function Name : void DemorauS(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
void Maquina_Estados(void){
	static int estado = 0;
	static int val = 0;

	if(tecla == '1')
	{
		if(val<7){
			CargarString(Linea3,"Guardando.         ");
		}else if (val < 14){
			CargarString(Linea3,"Guardando..        ");
		}else if (val < 21){
			CargarString(Linea3,"Guardando...       ");
		}else if (val < 28){
			CargarString(Linea3,"Guardando....      ");
		}else{
			val = 0;
		}
		val++;
		CargarString(Linea4,"A-Parar             ");
		estado = 1;
	}
	if(tecla == '2')
	{
		if(val<7){
			CargarString(Linea3,"Enviando.         ");
		}else if (val < 14){
			CargarString(Linea3,"Enviando..        ");
		}else if (val < 21){
			CargarString(Linea3,"Enviando...       ");
		}else if (val < 28){
			CargarString(Linea3,"Enviando....      ");
		}else{
			val = 0;
		}
		val++;
		CargarString(Linea4,"A-Parar             ");
		estado = 1;
	}
	if(tecla == 'A' && estado == 1)
	{
		CargarString(Linea3,"1-Guardar          ");
		CargarString(Linea4,"2-Enviar Muestras  ");
		estado = 0;
	}
}
