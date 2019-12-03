/*--------------------------------------------------------------------*-
    task-gpio_DHT22_lpc1769.c (Released 2019-04)

 	Simple GPIO_DHT22 task for LPC1769.
-*--------------------------------------------------------------------*/

// Project header
#include "../main/main.h"

// Task header
#include "task-gpio_DHT22_lpc1769.h"
#include "task-gpio_lcd_lpc1769.h"

// -------- Public variable ------------------------------------------
#define MIN_INTERVAL 2000 /**< min interval value */
#define TIMEOUT -1        /**< timeout on */
#define TIMELIMIT 10000

//extern char* Linea3;
extern int temperatura;
extern int humedad;

uint8_t data[5];
bool lastresult = 0;
bool blockDHT = 1;


// -------- Private constants ----------------------------------------

// -------- Private variable -----------------------------------------

// -------- Functions ------------------------------------------------
/****************************************************************************************
 * Function Name : uint32_t expectPulse(uint32_t);
 * Description : Blocking delay
 * Input : Time in Microseconds
 * Output :
 * Void Note :
****************************************************************************************/
uint32_t expectPulse(uint32_t estado){
	uint32_t time;
	DWT->CYCCNT=0;
	if(estado){
		while( (!Chip_GPIO_GetPinState(LPC_GPIO, GPIO_DHT22_PORT, GPIO_DHT22_DATA)) && ((int)((DWT->CYCCNT)/100) < TIMELIMIT));
	}else{
		while( (Chip_GPIO_GetPinState(LPC_GPIO, GPIO_DHT22_PORT, GPIO_DHT22_DATA)) && ((int)((DWT->CYCCNT)/100) < TIMELIMIT));
	}
	time = (int)((DWT->CYCCNT)/100);
	if(time >= TIMELIMIT){
		return TIMEOUT;
	}else{
		return time;
	}
}
/****************************************************************************************
 * Function Name : void GPIO_DHT22_update(void);
 * Description : Blocking read
 * Input :
 * Output :
 * Void Note :
****************************************************************************************/
void GPIO_DHT22_Update(void) {
	static int espera = 100;
	static int estado = 0;
	float temp = 0;

	// Check if sensor was read less than two seconds ago and return early to use last reading.
	// Reset 40 bits of received data to zero.
	uint32_t cycles[80];
	uint32_t lowCycles = 0;
	uint32_t highCycles = 0;
	uint8_t i;


	//Chip_GPIO_SetPinDIRInput(LPC_GPIO, DHT22_PORT, DHT22_PIN);
	if (blockDHT) {
		if (!--espera) {
			espera = 100;
			estado = 1;
		}

		if (estado) {
			estado--;
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_DHT22_PORT,
					GPIO_DHT22_DATA);

			for (i = 0; i < 5; i++) {
				data[i] = 0;
			}
			// Send start signal.  See DHT datasheet for full signal diagram:
			// http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf

			// Go into high impedence state to let pull-up raise data line level and
			// start the reading process.
			//Chip_GPIO_SetPinDIRInput(LPC_GPIO, DHT22_PORT, DHT22_PIN);
			//DemorauS(1000); //demora de 1 milisegundo

			//Chip_GPIO_SetPinToggle(LPC_GPIO, TIME_PORT, TIME_PIN);

			// First set data line low for a period according to sensor type
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_DHT22_PORT,
					GPIO_DHT22_DATA);
			Chip_GPIO_SetPinOutLow(LPC_GPIO, GPIO_DHT22_PORT, GPIO_DHT22_DATA);
			DemorauS(1100); // data sheet says "at least 1ms"

			// End the start signal by setting data line high for 40 microseconds.
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_DHT22_PORT,
					GPIO_DHT22_DATA);

			// Delay a moment to let sensor pull data line low.
			DemorauS(50);

			// Now start reading the data line to get the value from the DHT sensor.

			// Turn off interrupts temporarily because the next sections
			// are timing critical and we don't want any interruptions.

			// First expect a low signal for ~80 microseconds followed by a high signal
			// for ~80 microseconds again.
			if (expectPulse(0) == TIMEOUT) {
				//DEBUG_PRINTLN(F("DHT timeout waiting for start signal low pulse."));
				lastresult = false;
			}
			if (expectPulse(1) == TIMEOUT) {
				//DEBUG_PRINTLN(F("DHT timeout waiting for start signal high pulse."));
				lastresult = false;
			}

			// Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
			// microsecond low pulse followed by a variable length high pulse.  If the
			// high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
			// then it's a 1.  We measure the cycle count of the initial 50us low pulse
			// and use that to compare to the cycle count of the high pulse to determine
			// if the bit is a 0 (high state cycle count < low state cycle count), or a
			// 1 (high state cycle count > low state cycle count). Note that for speed
			// all the pulses are read into a array and then examined in a later step.

			for (i = 0; i < 81; i += 2) {
				cycles[i] = expectPulse(0);
				cycles[i + 1] = expectPulse(1);
			}
			// Timing critical code is now complete.

			// Inspect pulses and determine which ones are 0 (high state cycle count < low
			// state cycle count), or 1 (high state cycle count > low state cycle count).
			for (int i = 0; i < 41; ++i) {
				lowCycles = cycles[(2 * i) + 1];
				highCycles = cycles[(2 * i) + 2];
				if ((lowCycles == TIMEOUT) || (highCycles == TIMEOUT)) { //DHT timeout waiting for pulse
					lastresult = false;
				}
				data[i / 8] <<= 1;
				// Now compare the low and high cycle times to see if the bit is a 0 or 1.
				if (highCycles > lowCycles) {
					// High cycles are greater than 50us low cycle count, must be a 1.
					data[i / 8] |= 1;
				}
				// Else high cycles are less than (or equal to, a weird case) the 50us low
				// cycle count so this must be a zero.  Nothing needs to be changed in the
				// stored data.
			}

			// Check we read 40 bits and that the checksum matches.
			//if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
			if (data[0] != 0 ) {
				lastresult = true;
				humedad = (int)(readHumidity()*10);
				temperatura = (int) (readTemperature()*10);


				temp = 1;
			} else {
				//DEBUG_PRINTLN(F("DHT checksum failure!"));
				lastresult = false;
			}

		}
	}
}
/****************************************************************************************
 * Function Name : readHumidiy();
 * Description :
 * Input :
 * Output : float value - humidity in percent
 * Note :
****************************************************************************************/
float readHumidity(void) {
  float f = 0;
  if(lastresult){
	  f = ((uint8_t)data[0]) << 8 | data[1];
	  f *= 0.1;
	  return f;
  }else{
	  return 0;
  }
}
/****************************************************************************************
 * Function Name : readTemperature();
 * Description :
 * Input :
 * Output : Temperature value in selected scale
 * Note :
****************************************************************************************/
float readTemperature(void){
  float f = 0;
  if(lastresult){
	  f = ((uint8_t)(data[2] & 0x7F)) << 8 | data[3];
	  f *= 0.1;

	  if (data[2] & 0x80) f *= -1;
	  return f;
  }else{
	  return 0;
  }
}
/****************************************************************************************
 * Function Name : GPIO_DHT22_Init();
 * Description :
 * Input :
 * Output : Void
 * Note :
****************************************************************************************/
void GPIO_DHT22_Init(void)
{
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_DHT22_PORT, GPIO_DHT22_DATA, IOCON_MODE_PULLUP, IOCON_FUNC0);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_DHT22_PORT, GPIO_DHT22_DATA);

}
