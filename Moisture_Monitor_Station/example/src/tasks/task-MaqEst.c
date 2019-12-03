/*
 * Las variables son locales pero hay que hacerlas globales.
 *
 * UART:
 * dummyVecTEM (vector para enviar TEMPERATURA, formato: 30.20 (max 5 bytes)
 * dummyVecHUM (vector para enviar HUMEDAD, formato: 30.20 (max 5 bytes)
 * bool blockUART
 *
 * DISPLAY:
 * char Linea1[]="Moiture Monitor Sta";
 * char Linea2[]="1-Acceder a Memoria";
 * char Linea3[]="2-Sensar";
 * char Linea4[]="3-Guardar Muestra";
 *
 * DHT22:
 * float humedad = 0;
 * float temp = 0;
 * bool blockDHT
 *
 * TECLADO:
 * tecla
 */

/*
 *  Pantalla Principal, pantalla por defecto
    char Linea1[]="Moiture Monitor Sta";
	char Linea2[]=
	char Linea3[]="1-Acceder a Memoria";
	char Linea4[]="2-Sensar";

	tecla "D" vuelve a inicio

	Si te preciona 1 - Acceder a Memoria
	char Linea1[]="Memoria"
	char Linea2[]="Ingrese el ID del bloque que desea:" Ingresar ID y muestra promedio del bloque
					   -> A - Enviar, B - Borrar, D - Volver
	char Linea3[]=
	char Linea4[]="A - Muestra IDs"
					   -> Muestra IDs...
					   -> "D" vuelve a INICIO
					   -> 5 Sube
					   -> 8 Baja

	Si te preciona 2 - Sensar  -> ¿Guardo o no?
	char Linea1[]="Sensar"
	char Linea2[]="1 - Medicion Instantanea"
					   -> Muestra promedio 10 muestras
					   -> A, guarda y muestra ID
					   -> B, guarda, muestra ID y envia
					   -> D, vuelve a INICIO
	char Linea3[]="2 - Medicion continua" -> Muestra ultima medicion
					   -> "C" termina de medir
					   	   -> A, guarda y muestra ID
					   	   -> B, guarda, muestra ID y envia
					   	   -> D, vuelve a INICIO
	char Linea4[]=
 */

/*
 * Pantalla 1
 	char Linea1[]="Moiture Monitor Sta";
	char Linea2[]=
	char Linea3[]="1-Acceder a Memoria";
	char Linea4[]="2-Sensar";
 * Pantalla 2
 	char Linea1[]="Memoria";
	char Linea2[]="Ingrese el ID del bloque que desea:"
	char Linea3[]=
	char Linea4[]="A - Muestra IDs"
 * Pantalla 3
 	char Linea1[]="Memoria";
	char Linea2[]="ID: ingresada"
	char Linea3[]=
	char Linea4[]="A - Enviar, B - Borrar, D - Volver"
 * Pantalla 4
 	char Linea1[]="Muestra IDs";
	char Linea2[]="D- vuelve, 5 sube, 8 baja"
	char Linea3[]= muestra IDs
	char Linea4[]= muestra IDs-1
 * Pantalla 5
 	char Linea1[]="Sensar";
	char Linea2[]="1 - Medicion Instantanea"
	char Linea3[]="2 - Medicion continua"
	char Linea4[]=
 * Pantalla 6
 	char Linea1[]="Medicion Instantanea";
	char Linea2[]="Promedio: prom"
	char Linea3[]="A - SAVE & ShowID, B - A+Send"
	char Linea4[]="D, HOME"
 * Pantalla 7
  	char Linea1[]="Medicion Continua";
	char Linea2[]="Ultima Muestra: muestra"
	char Linea3[]=
	char Linea4[]="C, PARAR"
 * Pantalla 8
   	char Linea1[]="Medicion Continua";
	char Linea2[]="Promedio: prom"
	char Linea3[]="A - SAVE & ShowID, B - A+Send"
	char Linea4[]="D, HOME"
 */
