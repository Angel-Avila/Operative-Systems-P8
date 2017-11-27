#include "vdisk.h"

int vdwriteseclog(int nseclog, char *buffer)
{
	int secfis,cilindro,superficie;
	int unidad=0;
	// Obtener los datos de la partición para saber
	// en que sector físico, cilindro y superficie inicia 
	// la partición
	// Probablemente sea necesario también tener en memoria
	// el sector de boot de la partición para los datos de
	// la geometría de la partición.
	
	//  Calcular secfis,cilindro,superficie a partir de nseclog
	
	vdwritesector(unidad,superficie,cilindro,secfis,1,buffer);
}

int vdreadseclog(int nseclog, char *buffer)
{
	int secfis,cilindro,superficie;
	int unidad=0;
	// Obtener los datos de la partición para saber
	// en que sector físico, cilindro y superficie inicia 
	// la partición
	// Probablemente sea necesario también tener en memoria
	// el sector de boot de la partición para los datos de
	// la geometría de la partición.
	
	//  Calcular secfis,cilindro,superficie a partir de nseclog
	
	vdreadsector(unidad,superficie,cilindro,secfis,1,buffer);
}

// Funciones para la lectura y escritura de bloques, a partir 
// de un número de bloques

int writeblock(int nblock,char *buffer)
{
	int slad; // Sector lógico del área de datos
	int secs_x_bloque;
	int i;

	// Obtener los datos del sector de boot de la partición
	// para:
	// 	1.- Determinar en que sector lógico inicia el área
	// 	de bloques de datos, y guardarlo en la variable slad
	//
	//	2.- Obtener el número de sectores por bloque y guardarlo
	// 	en la variable secs_x_bloque
	
	// Calcular en que sector lógico inicia el bloque
	seclog=slad+(nblock-1)*secs_x_bloque;

	// Escribir en todos los sectores lógicos del bloque
	for(i=0;i<secs_x_bloque;i++)
	{	
		vdwriteseclog(seclog,buffer);
		seclog++;
		buffer+=512;
	}
}

int readblock(int nblock,char *buffer)
{
	int slad; // Sector lógico del área de datos
	int secs_x_bloque;
	int i;

	// Obtener los datos del sector de boot de la partición
	// para:
	// 	1.- Determinar en que sector lógico inicia el área
	// 	de bloques de datos, y guardarlo en la variable slad
	//
	//	2.- Obtener el número de sectores por bloque y guardarlo
	// 	en la variable secs_x_bloque
	
	// Calcular en que sector lógico inicia el bloque
	seclog=slad+(nblock-1)*secs_x_bloque;

	// Escribir en todos los sectores lógicos del bloque
	for(i=0;i<secs_x_bloque;i++)
	{	
		vdreadseclog(seclog,buffer);
		seclog++;
		buffer+=512;
	}
}