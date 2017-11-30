#include <stdio.h>
#include <stdlib.h>
#include "phys2log.h"
#include "mapainodoshandler.h"

// *************************************************************************
// Para el mapa de bits del área de nodos i
// *************************************************************************

struct SECBOOTPART secboot;
unsigned char inodesmap[512];
int mapa_bits_nodos_i; // sector logico del mapa de bits de nodos i
int secboot_en_memoria = 0, inodesmap_en_memoria = 0;

// Usando el mapa de bits, determinar si un nodo i, está libre u ocupado.
int isinodefree(int inode)
{
	int offset=inode/8;
	int shift=inode%8;
	int result;

	// Checar si el sector de boot de la partición está en memoria
	if(!secboot_en_memoria)
	{
		// Si no está en memoria, cárgalo
		result=vdreadseclog(0,(char *) &secboot);
		secboot_en_memoria=1;
	}
	mapa_bits_nodos_i= secboot.sec_inicpart + secboot.sec_res;
//Usamos la información del sector de boot de la partición para
						//determinar en que sector inicia el
						// mapa de bits de nodos i

	// Ese mapa está en memoria
	if(!inodesmap_en_memoria)
	{
		// Si no está en memoria, hay que leerlo del disco
		result=vdreadseclog(mapa_bits_nodos_i,inodesmap);
		inodesmap_en_memoria=1;
	}

	if(inodesmap[offset] & (1<<shift))
		return(0); // El nodo i ya está ocupado
	else
		return(1); // El nodo i está libre
}

// Buscar en el mapa de bits, el primer nodo i que esté libre, es decir, que su bit está en 0, me regresa ese dato
int nextfreeinode()
{
	int i,j;
	int result;

	// Checar si el sector de boot de la partición está en memoria
	if(!secboot_en_memoria)
	{
		// Si no está en memoria, cárgalo
		result=vdreadseclog(1,(char *) &secboot);
		secboot_en_memoria=1;
	}
	mapa_bits_nodos_i= secboot.sec_inicpart + secboot.sec_res;
	//Usamos la información del sector de boot de la partición para
						//determinar en que sector inicia el\
						// mapa de bits de nodos i

	// Ese mapa está en memoria
	if(!inodesmap_en_memoria)
	{
		// Si no está en memoria, hay que leerlo del disco
		result=vdreadseclog(mapa_bits_nodos_i,inodesmap);
		inodesmap_en_memoria=1;
	}

	// Recorrer byte por byte mientras sea 0xFF sigo recorriendo
	i=0;
	while(inodesmap[i]==0xFF && i<3)
		i++;

	if(i<3)
	{
		// Recorrer los bits del byte, para encontrar el bit
		// que está en cero
		j=0;
		while((inodesmap[i] & (1<<j)) && j<8)
			j++;

		printf("I %d, J %d\n", i, j);
		return(i*8+j); // Regresar el bit del mapa encontrado en cero
	}
	else // Todos los bits del mapa de nodos i están en 1
		return(-1); // -1 significa que no hay nodos i disponibles
}


// Poner en 1, el bit que corresponde al número de inodo indicado
int assigninode(int inode)
{

	int offset=inode/8;
	int shift=inode%8;
	int result;

	printf("Assign inode %d, offset %d, shift %d\n", inode, offset, shift);

	// Checar si el sector de boot de la partición está en memoria
	if(!secboot_en_memoria)
	{
		printf("Secboot no estaba en memoria\n");
		// Si no está en memoria, cárgalo
		result=vdreadseclog(0,(char *) &secboot);
		secboot_en_memoria=1;
	}
	mapa_bits_nodos_i= secboot.sec_inicpart + secboot.sec_res;
//Usamos la información del sector de boot de la partición para
						//determinar en que sector inicia el
						// mapa de bits de nodos i

	// Ese mapa está en memoria
	if(!inodesmap_en_memoria)
	{
		// Si no está en memoria, hay que leerlo del disco
		printf("Inodesmap no estaba en memoria\n");
		result=vdreadseclog(mapa_bits_nodos_i,inodesmap);
		inodesmap_en_memoria=1;
	}

	inodesmap[offset]|=(1<<shift); // Poner en 1 el bit indicado
	vdwriteseclog(mapa_bits_nodos_i,inodesmap);
	return(1);
}

// Poner en 0, el bit que corresponde al número de inodo indicado
int unassigninode(int inode)
{
	int offset=inode/8;
	int shift=inode%8;
	int result;

	// Checar si el sector de boot de la partición está en memoria
	if(!secboot_en_memoria)
	{
		// Si no está en memoria, cárgalo
		result=vdreadseclog(0,(char *) &secboot);
		secboot_en_memoria=1;
	}
	mapa_bits_nodos_i= secboot.sec_inicpart +secboot.sec_res;
//Usamos la información del sector de boot de la partición para
						//determinar en que sector inicia el
						// mapa de bits de nodos i

	// Ese mapa está en memoria
	if(!inodesmap_en_memoria)
	{
		// Si no está en memoria, hay que leerlo del disco
		result=vdreadseclog(mapa_bits_nodos_i,inodesmap);
		inodesmap_en_memoria=1;
	}


	inodesmap[offset]&=(char) ~(1<<shift); // Poner en cero el bit que corresponde al inodo indicado
	vdwriteseclog(mapa_bits_nodos_i,inodesmap);
	return(1);
}
