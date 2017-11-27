#include <stdio.h>

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

// Debe medir 512 bytes
struct SECBOOTPART {
	char jump[4];
	char nombre_particion[8];
	// Tabla de parámetros del bios
	// Están los datos sobre el formato de la partición
	unsigned short sec_inicpart;		// 1 sector
	unsigned char sec_res;		// 1 sector reservado para el sector de boot de la partición
	unsigned char sec_mapa_bits_area_nodos_i;// 1 sector
	unsigned char sec_mapa_bits_bloques;	// 6 sectores
	unsigned short sec_tabla_nodos_i;	// 3 sectores
	unsigned int sec_log_particion;		// 43199 sectores
	unsigned char sec_x_bloque;			// 2 sectores por bloque
	unsigned char heads;				// 8 superficies
	unsigned char cyls;				// 200 cilindros
	unsigned char secfis;				// 27 sectores por track
	char restante[484];	// Código de arranque
};
// printf("%d\n",sizeof(struct SECBOOT));

// Debe medir 64 bytes, importante es que el tamaño sea potencia de 2
struct INODE {
	char name[18];
	unsigned int datetimecreat;	// 32 bits
	unsigned int datetimemodif;	// 32 bits
	unsigned int datetimelaacc; // 32 bits
	unsigned short uid;		// 16 bits
	unsigned short gid;		// 16 bits
	unsigned short perms;	// 16 bits
	unsigned int size;			// 32 bits
	unsigned short direct_blocks[10];	// 10 x 16 bits = 20 bytes
	unsigned short indirect;	// 16 bits
	unsigned short indirect2;	// 16 bits
};

int main()
{
	struct MBR mbr;
	printf("%ld\n",sizeof(struct MBR));

	// Crear la primera partición que será desde
	// 	Cilindro= 0, Superficie = 0, Sector Físico = 2
	mbr.partition[0].chs_begin[0]=0; // Superficie
	mbr.partition[0].chs_begin[1]=2 & 0x1F;	// Los 5 bits menos significativos son el sector
	mbr.partition[0].chs_begin[2]=0;

	mbr.partition[0].chs_end[0]=5; // Superficie
	mbr.partition[0].chs_end[1]=17 & 0x1F;	// Los 5 bits menos significativos son el sector
	mbr.partition[0].chs_end[2]=299 & 0xFF;	// Cilindro
	mbr.partition[0].chs_end[1]=mbr.partition[0].chs_end[1] |
								(299 & 0x300) >> 2;

	printf("chs_end de la primera partición %X %X %X\n",mbr.partition[0].chs_end[0],mbr.partition[0].chs_end[1],mbr.partition[0].chs_end[2]);

	vdwritesec(0, 0, 0, 0, 1, (char *) &mbr);

	int part_formatear=0;
	struct SECBOOTPART sbp;
	int unidad = 0;
	int sfip = 2,sip = 0,cip= 0;

	// Obtener de la tabla de particiones los valores de:
	// 	sfip = Sector físico inicial de la partición
	//	sip = Superficie inicial de la partición
	//	cip = Cilindro inicial de la partición

	sbp.sec_inicpart=2;
	sbp.sec_res=1;
	sbp.sec_mapa_bits_area_nodos_i=1;
	sbp.sec_mapa_bits_bloques=6;
	sbp.sec_tabla_nodos_i=3
	sbp.sec_log_particion=43100;
	sbp.sec_x_bloque=2;
	sbp.heads=8;
	sbp.cyls=200;
	sbp.secfis=27;

	// Escribir el contenido de la estructura sbp en el sector físico inicial de la
	// partición
	vdwritesec(unidad,cip,sip,sfip,1,(char *) &sbp);

}
