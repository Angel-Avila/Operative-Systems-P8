#include <stdio.h>
#include <stdlib.h>
#include "phys2log.h"
#include "blockhandler.h"

// *************************************************************************
// Para el mapa de bits del área de nodos i
// *************************************************************************

extern struct SECBOOTPART secboot;
extern int secboot_en_memoria;
int inicio_area_datos; // sector logico del inicio

// **********************************************************************************
// Lectura y escritura de bloques
// **********************************************************************************

int writeblock(int block,char *buffer)
{
	int result;
	int i;

	// Determinar si el sector de boot de la partición está en memoria, si no está en memoria, cargarlo
	if(!secboot_en_memoria)
	{
		// Leer el sector lógico 0, donde está
// el sector de boot de la partición
		result=vdreadseclog(0,(char *) &secboot);
		secboot_en_memoria=1;
	}

	// Inicio area de datos =  setor de inicio de la partición +
// 							sectores reservados +
    //							sectores mapa de bits area de nodos i+
	//							sectores mapa de bits area de datos+
	//							sectores area nodos i (dr)
	inicio_area_datos = secboot.sec_inicpart+secboot.sec_res+secboot.sec_mapa_bits_area_nodos_i +
										  secboot.sec_mapa_bits_bloques+secboot.sec_tabla_nodos_i;

	printf("AREA DE DATOS: %d\n", inicio_area_datos);
	// Escribir todos los sectores que corresponden al
	// bloque
	for(i=0;i<secboot.sec_x_bloque;i++)
		vdwriteseclog(inicio_area_datos+(block-1)*secboot.sec_x_bloque+i,buffer+512*i);
	return(1);
}

int readblock(int block,char *buffer)
{
	int result;
	int i;

	// Determinar si el sector de boot de la partición está en memoria, si no está en memoria, cargarlo
	if(!secboot_en_memoria)
	{
		// Leer el sector lógico 0, donde está
// el sector de boot de la partición
		result=vdreadseclog(0,(char *) &secboot);
		secboot_en_memoria=1;
	}

	// Inicio area de datos =  setor de inicio de la partición +
// 							sectores reservados +
    //							sectores mapa de bits area de nodos i+
	//							sectores mapa de bits area de datos+
	//							sectores area nodos i (dr)
	inicio_area_datos = secboot.sec_inicpart+secboot.sec_res+secboot.sec_mapa_bits_area_nodos_i +
										  secboot.sec_mapa_bits_bloques+secboot.sec_tabla_nodos_i;

	for(i=0;i<secboot.sec_x_bloque;i++)
		vdreadseclog(inicio_area_datos+(block-1)*secboot.sec_x_bloque+i,buffer+512*i);
	return(1);
}
