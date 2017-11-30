#include <stdio.h>
#include "vdisk.h"
#include "phys2log.h"
#include "mapainodoshandler.h"
#include "mapadatoshandler.h"
#include "blockhandler.h"
#include "inodehandler.h"

#pragma pack(1)

struct PARTITION {
	unsigned char drive_status;	// 1
	unsigned char chs_begin[3];	// 3
	unsigned char partition_type; // 1
	unsigned char chs_end[3];	// 3
	unsigned int lba;			// 4
	unsigned int secs_partition; // 4
};

struct MBR {
	unsigned char bootstrap_code[446];
	struct PARTITION partition[4];	// 4 * 16 = 64
	unsigned short signature;		// 16 bits = 2 bytes
};

// printf("%d\n",sizeof(struct SECBOOT));

#define LINESIZE 16
#define SECSIZE 512

int main()
{
	struct MBR mbr;

	for(int i = 0; i < 446; i++)
		mbr.bootstrap_code[i] = 170 & 0xAA;													// Llenar de As el bootstrap_code para ver su tamano en oktate

  mbr.partition[0].drive_status = 128 & 0xFF; 								// Status; bit 7 is set for active or bootable
	// Crear la primera partición que será desde
	// 	Cilindro= 0, Superficie = 0, Sector Físico = 2
	mbr.partition[0].chs_begin[0]=0; 														// Superficie
	mbr.partition[0].chs_begin[1]=2 & 0x1F;										  // Los 5 bits menos significativos son el sector
	mbr.partition[0].chs_begin[2]=0;														// Cilindro

  mbr.partition[0].partition_type = 0x08;											// Logical sectored FAT12 or FAT16
	mbr.partition[0].chs_end[0]=5; 															// Superficie
	mbr.partition[0].chs_end[1]=17 & 0x1F;											// Los 5 bits menos significativos son el sector
	mbr.partition[0].chs_end[2]=299 & 0xFF;											// Cilindro
	mbr.partition[0].chs_end[1]=mbr.partition[0].chs_end[1] |
								(299 & 0x300) >> 2;

  mbr.partition[0].lba = 2 & 0xFFFFFFFF;											// Logical base addressing of first absolute sector in partition
  mbr.partition[0].secs_partition = 43199 & 0xFFFFFFFF;			  // Number of sections in partition (bloques)

	mbr.signature = 0xAA55;

	//printf("end de la primera particion %X %X %X\n", mbr.partition[0].chs_end[0], mbr.partition[0].chs_end[1], mbr.partition[0].chs_end[2]);

	for(int i = 1; i < 4; i++) {
		mbr.partition[i].drive_status = 0; 					  // Status; bit 7 is set for active or bootable

		mbr.partition[i].chs_begin[0]=0; 							// Superficie
		mbr.partition[i].chs_begin[1]=0;							// Los 5 bits menos significativos son el sector
		mbr.partition[i].chs_begin[2]=0;						  // Cilindro

		mbr.partition[i].partition_type = 0;					// Logical sectored FAT12 or FAT16
		mbr.partition[i].chs_end[0]=0; 	 							// Superficie
		mbr.partition[i].chs_end[1]=0;								// Los 5 bits menos significativos son el sector
		mbr.partition[i].chs_end[2]=0;								// Cilindro

		mbr.partition[i].lba = 0;										 // Logical base addressing of first absolute sector in partition
		mbr.partition[i].secs_partition = 0;			   // Number of sections in partition (bloques)
	}

	vdwritesector(0, 0, 0, 1, 1, (char *) &mbr);

	struct SECBOOTPART sbp;
	int seclog = 0;

	// Obtener de la tabla de particiones los valores de:
	// 	sfip = Sector físico inicial de la partición
	//	sip = Superficie inicial de la partición
	//	cip = Cilindro inicial de la partición

	sbp.sec_inicpart=0;
	sbp.sec_res=1;																	// 1 sector reservado para el sector de boot de la partición
	sbp.sec_mapa_bits_area_nodos_i=1;
	sbp.sec_mapa_bits_bloques=6;
	sbp.sec_tabla_nodos_i=3;
	sbp.sec_log_particion=43199;
	sbp.sec_x_bloque=2;
	sbp.heads=8;
	sbp.cyls=200;
	sbp.secfis=27;

	for(int i = 0; i < 484; i++)
		sbp.restante[i] = 170 & 0xAA;									// Lleno restante de puras A para ver mejor su lugar en el disco

	// Escribir el contenido de la estructura sbp en el sector físico inicial de la partición
	vdwriteseclog(seclog, (char *) &sbp);

	printf("==============================================================\n");

	unsigned char buffer[SECSIZE*2];

	for(int i = 0; i < SECSIZE*2; i++)
		buffer[i] = 0xAA;

	writeblock(1, buffer);

	for(int i = 0; i < 2; i++) {
		assigninode(i);
		printf("Next free inode %d\n", nextfreeinode());
		printf("---------------------------------------\n");
	}

	printf("==============================================================\n");

	for(int i = 0; i < 2; i++) {
		assignblock(i);
		//printf("Next free block %d\n", nextfreeblock());
		//printf("---------------------------------------\n");
	}

	printf("==============================================================\n");

	char buf[18];

	for(int i=0;i<18;i++)
		buf[i] = 0xC;

	for(int i=0;i<24;i++)
		setninode(i, buf, 0xF, 0xD, 0xE);

}
