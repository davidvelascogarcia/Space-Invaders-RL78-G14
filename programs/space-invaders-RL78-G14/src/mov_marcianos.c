/*
 * mov_marcianos.c
 *
 *  Created on: 30/4/2017
 *      Author: Ivan
 */
#include "mov_marcianos.h"
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_timer.h"


extern unsigned char mover;						// Variable externa que se encarga de mover a los marcianos.
unsigned char centro = 14;						// Variable que indica la posición central de los marcianos para la fila inferior, su equivalencia en binario es 0b01110.
unsigned char derecha=28;						// Variable que indica la posición derecha de los marcianos para la fila inferior, su equivalencia en binario es 0b00111.
unsigned char izquierda=7;						// Variable que indica la posición izquierda de los marcianos para la fila inferior, su equivalencia en binario es 0b11100.
unsigned char filaactual=0;						// Variable que nos indica en la fila en la que nos encontramos.
unsigned char centro2=14;						// Variable que indica la posición central de los marcianos para la fila superior, su equivalencia en binario es 0b01110.
unsigned char derecha2=28;						// Variable que indica la posición derecha de los marcianos para la fila superior, su equivalencia en binario es 0b00111.
unsigned char izquierda2=7;						// Variable que indica la posición izquierda de los marcianos para la fila superior, su equivalencia en binario es 0b11100.

void mover_marcianos(void){						// Función encargada de mostrar la posición que ocupan los marcianos para cada momento del movimiento.

	if(mover<=13){								// Condición que pone fin al movimiento, mientras mover sea menor o igual a 13, los marcianos se mueven.
		switch(mover){
		case 0:									// Posición de partida.
			mov_marcianos[6]=centro2;			// Iluminamos los leds de la fila superior de la matriz mov_marcianos y les damos un valor de 14 que equivale en binario a 0b01110
			mov_marcianos[5]=centro;			// Realizamos el mismo procedimiento para la fila inferior
			filaactual=6;						// Esta variable nos indica en la fila en la que nos encontramos. Sera usada posteriormente para comparar la posición de los marcianos y el disparo.
			break;
		case 1:									// Al actualizarse la variable mover, cuyo refresco es de 600 ms entramos en este caso, el cual representa el desplazamiento hacia la derecha de las dos fias de marcianos.
			mov_marcianos[6]=derecha2;			// En este caso mov_marcianos adquire el valor de 28, lo que equivale a 0b00111 en binario (se desplazan los bits hacia la derecha).
			mov_marcianos[5]=derecha;
			break;
		case 2:									// Entramos en este caso tras actualizar de nuevo la variable mover, después de esperar los 600 ms del refrsco.
			mov_marcianos[6]=0;					// Ya que en el case anterior llegamos al extremo derecho, en este tenemos que bajar los marcianos una fila, para ello ponemos a 0 la superior.
			mov_marcianos[5]=derecha2;			
			mov_marcianos[4]=derecha;			// Inicializamos la fila 4 de nuestra matriz con el valor que le corresponde.
			filaactual=5;						// La fila ha decrementado en una unidad ya que como hemos dicho anteriormente en este caso bajamos una fila a los marcianos.
			break;
		case 3:									// En este caso los marcianos comienzan a desplazarse hacia la izquierda, ocupando la poscion central.
			mov_marcianos[5]=centro2;			
			mov_marcianos[4]=centro;
			break;
		case 4:									// En este caso, los marcianos han llegado al extremo izquiedo,por lo que en el siguiente cas0 estos tendrán que bajar una fila 
			mov_marcianos[5]=izquierda2;		// para la misma posición que ocupan ahora y tras esto comenzaran a desplazarse hacia la derecha.		
			mov_marcianos[4]=izquierda;			// Vemos que el movimiento es ciclico y repetitivo, por lo que consideramos que no es necesario seguir explicando el resto de los casos.
			break;
		case 5:
			mov_marcianos[5]=0;
			mov_marcianos[4]=izquierda2;
			mov_marcianos[3]=izquierda;
			filaactual=4;
			break;
		case 6:
			mov_marcianos[4]=centro2;
			mov_marcianos[3]=centro;
			break;
		case 7:
			mov_marcianos[4]=derecha2;
			mov_marcianos[3]=derecha;
			break;
		case 8:
			mov_marcianos[4]=0;
			mov_marcianos[3]=derecha2;
			mov_marcianos[2]=derecha;
			mov_marcianos[6]=7;
			filaactual=3;
			break;
		case 9:
			mov_marcianos[3]=centro2;
			mov_marcianos[2]=centro;
			mov_marcianos[6]=14;
			break;
		case 10:
			mov_marcianos[3]=izquierda2;
			mov_marcianos[2]=izquierda;
			mov_marcianos[6]=28;
			break;
		case 11:
			mov_marcianos[3]=0;
			mov_marcianos[2]=izquierda2;
			mov_marcianos[1]=izquierda;
			mov_marcianos[6]=0;
			filaactual=2;
			break;
		case 12:
			mov_marcianos[2]=centro2;
			mov_marcianos[1]=centro;
			break;
		case 13:
			mov_marcianos[2]=derecha2;
			mov_marcianos[1]=derecha;

			break;
		}
	}
	else{										// Una vez los marcianos llegan al final de su recorrido, estos vuelven a su posiciíon incial.
		TDR02= 0xBE1F;							// Tambien cuando esto suceda, ejecutaremos un sonido de diferente frecuencia, que nos indique que el juego ha finalizado.
		R_TAU0_Channel2_Start();

		mov_marcianos[0]=0;
		mov_marcianos[1]=0;
		mov_marcianos[2]=0;
		mov_marcianos[3]=0;
		mov_marcianos[4]=0;
		mov_marcianos[6]=14;
		mov_marcianos[5]=14;


	}
}


