/*--------------------------------------------------------------------*-

    task-base_lpc1769.h (Released 2019-04)

--------------------------------------------------------------------

    - See task-base_lpc1769.c for details.

-*--------------------------------------------------------------------*/


#ifndef _UART0_TX_H
#define _UART0_TX_H 1


// ------ Public constants -----------------------------------------


// ------ Public data type declarations ----------------------------



// ------ Public function prototypes -------------------------------
void UART0_Init(void);
void UART0_Update(void);
void CargarUART (char* , int , int);

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
