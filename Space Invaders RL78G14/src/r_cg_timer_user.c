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
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/G14 V2.04.02.01 [15 May 2015]
* Device(s)    : R5F104PF
* Tool-Chain   : GCCRL78
* Description  : This file implements device driver for TAU module.
* Creation Date: 29/04/2017
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "matriz_marciano.h"
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "matriz_marciano.h"
#include "mov_marcianos.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
extern unsigned char refresco_fila;					// Referencias externas a todas las variables a usar en este fichero.
extern unsigned char start;
extern unsigned char mov_canon;
extern unsigned int A;
extern unsigned char mover;
extern signed char disparo;
extern unsigned char copia_canon;
unsigned char up=0;									// Variable encargada de hacer subir el disparo hacia arriba.
unsigned B=0;										// Variable encargada de cuantificar 200 ms, que es la velocidad seleccionada para el disparo.
extern unsigned char mov_disparo[7];
extern unsigned char filaactual;
extern unsigned char derecha;
extern unsigned char izquierda;
extern unsigned char centro;
extern unsigned char derecha2;
extern unsigned char izquierda2;
extern unsigned char centro2;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	
	// Este switch es fundamental para el desarollo del programa, ya que se va a encargar de realizar un barrido por filas, mostrandose
	// así todos dos leds que forman el juego de forma correcta.
	
	switch(refresco_fila){
	case 0:											// El primer case se corresponde con la linea inferior de la pantalla. Y siempre realizamos el mismo proceso para cada una de las filas:
		P1=0;										//	1º. Ponemos a 0 todos los leds de las columnas, para evitar que al refrescar la fila se produzcan encendidos indeseados.
		P15_bit.no3=0;								//	2º. Apagamos la fila anterior, ya que si no se encenderian varias filas simultaneamente.
		if(start==0)								// Condición que nos permite mostrar la pantalla de inicio y de la cual salimos cuando se pulsa el boton de start del juego.
			P1= matriz_marciano[6];					// Se muestra la matriz_marciano hasta que comienza el juego.
		else
			P1= mov_marcianos[0]|mov_canon;			// Si se pulsa el botón de start, se muestra tanto el mov_marcianos como el mov_canon con una mascara OR entre ellos. El objetivo de esta mascara es la de poder ver simultanemiente ambos elementos.
		P15_bit.no5=1;								//	3º. Encendemos la fila sobre la que queremos mostrar los leds.
		break;
	case 1:
		P1=0;
		P15_bit.no5=0;
		if(start==0)
			P1= matriz_marciano[5];
		else
			P1= mov_marcianos[1]|mov_disparo[1];	// En este caso la mascara OR se realiza entre el mov_marcianos y el mov_disparo, para poder visualizar de forma simultanea a los marcianos descendiendo y al disparo ascendiendo.
		P15_bit.no6=1;
		break;
	case 2:
		P1=0;
		P15_bit.no6=0;
		if(start==0)
			P1= matriz_marciano[4];
		else
			P1= mov_marcianos[2]|mov_disparo[2];
		P7_bit.no1=1;
		break;
	case 3:
		P1=0;
		P7_bit.no1=0;
		if(start==0)
			P1= matriz_marciano[3];
		else
			P1= mov_marcianos[3]|mov_disparo[3];
		P7_bit.no2=1;
		break;
	case 4:
		P1=0;
		P7_bit.no2=0;
		if(start==0)
			P1= matriz_marciano[2];
		else
			P1= mov_marcianos[4]|mov_disparo[4];
		P7_bit.no0=1;
		break;
	case 5:
		P1=0;
		P7_bit.no0=0;
		if(start==0)
			P1= matriz_marciano[1];
		else
			P1= mov_marcianos[5]|mov_disparo[5];
		P15_bit.no4=1;
		break;
	case 6:
		P1=0;
		P15_bit.no4=0;
		if(start==0)
			P1= matriz_marciano[0];
		else
			P1= mov_marcianos[6]|mov_disparo[6];
		P15_bit.no3=1;
		break;
	}

	if(start!=0){									// Condición que marca el inicio de los distintos temporizadores de software cuando salimos de la pantalla de inicio.
		
		// Contador de software para el movimiento de los marcianos.
		
		A++;										
		if(A>=600){
			mover++;								// Actualización movimiento marcianos cada 600 ms.
			A=0;
		}
		
		// Funcion de disparo
		
		if(disparo>1){							
			B++;											
			if(B>=200){					
				up++;								// Actualización del ascenso de la bala cada 200 ms.
				mov_disparo[up-1]=0;				// Ponemos a 0 la fila anterior al movimiento del disparo, ya que si no hacemos esto se iliminan todos los leds de la columna donde se ejecuta el disparo simultaneamente.
				mov_disparo[up]=copia_canon;		// Almacenamos en mov_disparo el valor del punto en el que se ha ejecutado el disparo y le hacemos ascender mediante el uso de la variable up.
				//************************************
				//Fila inferior de los marcianos
				if(up==filaactual){											//Una vez que la bala a llegado al valor de la fila actual de los marcianos
					if(mov_marcianos[filaactual-1]>copia_canon){			//Si el valor de la fila encendida - 1 en el numero de fila es superior al cañon significa que en esa fila no se ha borrado el marciano de esa fila por lo tanto el borrado en este ciclo sera de esa fila y no de la otra.
						if(mov_marcianos[filaactual-1]==derecha){			//Si la posición de los marcianos en la fila es derecha, es decir, los tres led de la derecha
							derecha=derecha-copia_canon;					//Restamos el valor de la columna en la que este la bala, para borrar el marciano de dicha posición, en esta y las demas derechas de esta fila dado que es una variable
							if(copia_canon==16){							//Si el columna es la derecha del todo, para afectar al borrado de las demas posiciones izquierda y centro borramos con su equivalencia
								izquierda=izquierda-4;						//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 16 en izquierda es 4 y en centro 8
								centro=centro-8;}							//Restamos la equivalencia
							if(copia_canon==8){								//Si el columna es la derecha del todo, para afectar al borrado de las demas posiciones izquierda y centro borramos con su equivalencia
								izquierda=izquierda-2;						//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 8 en izquierda es 2 y en centro 4
								centro=centro-4;}							//Restamos la equivalencia
							if(copia_canon==4){								//Si el columna es la derecha del todo, para afectar al borrado de las demas posiciones izquierda y centro borramos con su equivalencia
								izquierda=izquierda-1;						//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 4 en izquierda es 1 y en centro 2
								centro=centro-2;}							//Restamos la equivalencia
						}
					if(mov_marcianos[filaactual-1]==centro){				//Si nos encontramos en el caso de marcianos en el centro
						centro=centro-copia_canon;							//Restamos en centro la columna de deisparo
						if(copia_canon==8){									//Si el columna es la centro, para afectar al borrado de las demas posiciones derecha e izquierda borramos con su equivalencia
							izquierda=izquierda-4;							//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 8 en izquierda es 4 y en derecha 16
							derecha=derecha-16;}							//Restamos la equivalencia
						if(copia_canon==4){									//Si el columna es la centro, para afectar al borrado de las demas posiciones derecha e izquierda borramos con su equivalencia
							izquierda=izquierda-2;							//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 4 en izquierda es 2 y en derecha 8
							derecha=derecha-8;}								//Restamos la equivalencia
						if(copia_canon==2){									//Si el columna es la centro, para afectar al borrado de las demas posiciones derecha e izquierda borramos con su equivalencia
							izquierda=izquierda-1;							//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 2 en izquierda es 1 y en derecha 4
							derecha=derecha-4;}								//Restamos la equivalencia
					}
					if(mov_marcianos[filaactual-1]==izquierda){				//Si nos encontramos en el caso de marcianos en la derecha
						izquierda=izquierda-copia_canon;					//Restamos en la izquierda el valor del cañon
						if(copia_canon==4){									//Si el columna es la izquierda, para afectar al borrado de las demas posiciones derecha y centro borramos con su equivalencia
							centro=centro-8;								//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 4 en centro es 8 y en derecha 16
							derecha=derecha-16;}							//Restamos la equivalencia
						if(copia_canon==2){									//Si el columna es la izquierda, para afectar al borrado de las demas posiciones derecha y centro borramos con su equivalencia
							centro=centro-4;								//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 2 en centro es 4 y en derecha 8
							derecha=derecha-8;								//Restamos la equivalencia
						if(copia_canon==1){									//Si el columna es la izquierda, para afectar al borrado de las demas posiciones derecha y centro borramos con su equivalencia
							centro=centro-2;								//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 1 en centro es 2 y en derecha 4
							derecha=derecha-4;}								//Restamos la equivalencia
					}
					copia_canon=0;											//Ponemos a cero el cañon para parar el disparo, tras eliminar a un invasor.

					}				}

// Fila superior de los marcianos
				if(up==filaactual+1){										//Una vez que la bala se encuentra en la fila superior de los marcianos
					if(mov_marcianos[filaactual-1]<copia_canon){			//Cuando el valor del cañon es superior que el valor de la fila significa que ese marciano ya habia sido borrado previamente por lo que borramos marciano de la fila superior
						if(mov_marcianos[filaactual]==derecha2){			//Si la posición de los marcianos en la fila es derecha, es decir, los tres led de la derecha
							derecha2=derecha2-copia_canon;					//Restamos el valor de la columna en la que este la bala, para borrar el marciano de dicha posición, en esta y las demas derechas de esta fila dado que es una variable
							if(copia_canon==16){							//Si el columna es la derecha del todo, para afectar al borrado de las demas posiciones izquierda y centro borramos con su equivalencia
								izquierda2=izquierda2-4;					//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 16 en izquierda es 4 y en centro 8
								centro2=centro2-8;}							//Restamos la equivalencia
							if(copia_canon==8){								//Si el columna es la derecha del todo, para afectar al borrado de las demas posiciones izquierda y centro borramos con su equivalencia
								izquierda2=izquierda2-2;					//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 8 en izquierda es 2 y en centro 4
								centro2=centro2-4;}							//Restamos la equivalencia
							if(copia_canon==4){								//Si el columna es la derecha del todo, para afectar al borrado de las demas posiciones izquierda y centro borramos con su equivalencia
								izquierda2=izquierda2-1;					//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 4 en izquierda es 1 y en centro 2
								centro2=centro2-2;}							//Restamos la equivalencia
						}
					if(mov_marcianos[filaactual]==centro2){					//Si nos encontramos en el caso de marcianos en el centro
						centro2=centro2-copia_canon;						//Restamos en centro la columna de disparo
							if(copia_canon==8){								//Si el columna es la centro, para afectar al borrado de las demas posiciones derecha e izquierda borramos con su equivalencia
								izquierda2=izquierda2-4;					//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 8 en izquierda es 4 y en derecha 16
								derecha2=derecha2-16;}						//Restamos la equivalencia
							if(copia_canon==4){								//Si el columna es la centro, para afectar al borrado de las demas posiciones derecha e izquierda borramos con su equivalencia
								izquierda2=izquierda2-2;					//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 4 en izquierda es 2 y en derecha 8
								derecha2=derecha2-8;}						//Restamos la equivalencia
							if(copia_canon==2){								//Si el columna es la centro, para afectar al borrado de las demas posiciones derecha e izquierda borramos con su equivalencia
								izquierda2=izquierda2-1;					//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 2 en izquierda es 1 y en derecha 4
								derecha2=derecha2-4;}						//Restamos la equivalencia
					}
					if(mov_marcianos[filaactual]==izquierda2){				//Si nos encontramos en el caso de marcianos en la derecha
						izquierda2=izquierda2-copia_canon;					//Restamos izquierda a la columna de disparo
							if(copia_canon==4){								//Si el columna es la izquierda, para afectar al borrado de las demas posiciones derecha y centro borramos con su equivalencia
								centro2=centro2-8;							//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 4 en centro es 8 y en derecha 16
								derecha2=derecha2-16;}						//Restamos la equivalencia
							if(copia_canon==2){								//Si el columna es la izquierda, para afectar al borrado de las demas posiciones derecha y centro borramos con su equivalencia
								centro2=centro2-4;							//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 2 en centro es 4 y en derecha 8
								derecha2=derecha2-8;}						//Restamos la equivalencia
							if(copia_canon==1){								//Si el columna es la izquierda, para afectar al borrado de las demas posiciones derecha y centro borramos con su equivalencia
								centro2=centro2-2;							//Dado que izquierda = 1-2-4, centro = 2-4-8 y derecha = 4-8-16, el equivalente de 1 en centro es 2 y en derecha 4
								derecha2=derecha2-4;}						//Restamos la equivalencia
					}
					copia_canon=0;											//Ponemos a cero el cañon para parar el disparo, tras eliminar a un invasor.
					}
				}

				//************************************
				if(up>=8){							// Condicion que determina si el disparo ha llegado al marge superior de la pantalla.	
					up=0;							// Si se da el caso, tanto la variable up como disparo vuelven a su valor incial.
					disparo=0;
				}
				B=0;
			}
		}
	}
	refresco_fila++;								// Variable encargada de refrescar la fila. En este caso hemos seleccionado un refresco de 1 ms para cada fila.
	if(refresco_fila>=7){							// Condición que reinicia el contador cuando el valor el superior o igual a 7, que son las filas que posee nuestra matriz de leds.
		refresco_fila=0;
	}
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
