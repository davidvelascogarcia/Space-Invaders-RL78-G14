/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_intc_user.c
* Version      : CodeGenerator for RL78/G14 V2.04.02.01 [15 May 2015]
* Device(s)    : R5F104PF
* Tool-Chain   : GCCRL78
* Description  : This file implements device driver for INTC module.
* Creation Date: 28/04/2017
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_intc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
unsigned char start=0; 				// Variable que marca el cominenzo del juego.
signed char desplazar=2; 			// Variable que nos indica en la posción en la que se encuentra el cañon en todo momento, siendo 2 la posción central.
unsigned char canon; 				// Variable encargada de llevar a cabo el movimiento del cañón.
signed char disparo=-1;				// Variable encargada de inciar el disparo.
unsigned char copia_canon;			// Esta variable realiza una copia de la posión en la que se encuentra el cañón al realizar el disparo.
extern unsigned char mov_canon;		// Variable externa encargada de realizar el movimiento del cañón a lo largo del juego.
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_intc8_interrupt
* Description  : This function is INTP8 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_intc8_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	
	// Esta función que se encarga de desplazar el cañon hacia la derecha. La variable desplazar se incializa con valor 2 (Posicion central)
	
	desplazar ++; 					// Cada vez que pulsamos el botón aumentamos el valor de la variable desplazar. 
	if(desplazar < 4){ 				// Esta condición define la posición máxima hasta la que nos podemos desplazar.
		mov_canon<<=1; 				// Cada vez que entramos dentro de esta condición desplazamos un bit a la derecha.
	}else{
		desplazar=4; 				// Si desplazar es mayor que 4, forzamos que este siga siendo 4 para que no se desplace mas allá de la posición máxima.
		mov_canon=0b10000;			// Forzamos a la variable mov_cañon a permanecer en el extremo.
	}
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_intc9_interrupt
* Description  : This function is INTP9 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_intc9_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	
	// Función que se encarga de desplazar el cañon hacia la izquierda.
	
	desplazar --; 					// Cada vez que pulsamos el botón la variable desplazar decrementa en una unidad.
	if(desplazar > 0){ 				// Esta condición define la posción maxima que puede alcanzar el cañon en la parte izquierda de la pantalla.
		mov_canon>>=1;				// Si estamos dentro de esta condición desplazamos el bit de la variable mov_cañon hacia izquierda.
	}else{
		desplazar=0; 				// Cuando no se cumple la condición anterior, forzamos que la variable desplazar tenga valor 0, es decir, que se mantenga en el extremo izquierdo de la pantalla.
		mov_canon=0b00001; 			// Cuando estamos en esta situación, forzamos a la variable mov_cañon a mantiene el valor del extremo izquierdo de la pantalla.
	}
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_intc10_interrupt
* Description  : This function is INTP10 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_intc10_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	
	// Este fragmento de código es el reponsable de realizar el sonido de diparo.
	// Para ello recurrimos a los registros de interrupción externa para flanco de bajada.
	
	if(EGN1_bit.no2==1)				// EGN10 flanco de bajada. Siempre que detecte un flanco de bajada, el sonido comienza.
	{
	R_TAU0_Channel2_Start();		// Iniciamos el sonido.
	EGN1_bit.no2=0;					// Ponemos a 0 lógico el registro que detecta el flanco de bajada.
	EGP1_bit.no2=1;  				// Ponemos a 1 lógico el registro de flanco de subida, para que al soltar el botón no entre en esta condición y se pueda parar el sonido.
	}
	else
	{
	R_TAU0_Channel2_Stop();			// Función encargada de detener el sonido cuando el botón ha dejado de pulsarse.
	EGN1_bit.no2=1;					// Activamos el registro de flanco de bajada, para que a la siguiente pulsación el sonido se ejecute correctamente.
	EGP1_bit.no2=0;					// Ponemos a 0 lógico el registro de flanco de subida.
	}
	
	// Variable que marca el comienzo del juego. Al pulsar este switch pasamos de la pantalla inicial que muestra un marciano al desarrollo del juego.
	
	start=1;
	
	// Mecanismo de disparo
	
	disparo++;
	if(desplazar==4)				// Estas condiciones se encargan de detectar en la posición en la que se encuentra el cañón en el momento del disparo.
		copia_canon=16;				// A través de esta variable, copiamos la posición del cañón para que posteriormente esta sea mostrada en forma de disparo por la matriz mov_disparo[7].
	if(desplazar==3)				// Repetimos el proceso para cada una de las posiciones del cañón.
		copia_canon=8;
	if(desplazar==2)
		copia_canon=4;
	if(desplazar==1)
		copia_canon=2;
	if(desplazar==0)
		copia_canon=1;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
