/*
 * cadena.c
 *
 *  Created on: 4 de may. de 2016
 *      Author: carmen
 */
#include "cadena.h"
void quitaSaltoDeLinea(Cadena cad){
	int lon=strlen(cad);

	if(cad[lon-1]=='\n'){
		cad[lon-1]='\0';
	}
}
