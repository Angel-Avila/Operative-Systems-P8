#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vdisk.h"
#include "phys2log.h"

#define LINESIZE 16
#define SECSIZE 512

int main(int argc, char *argv[])
{
	int drive;
	int ncyl, nhead, nsec;
	int fd;
	unsigned char buffer[SECSIZE];
	int offset;
	int i, j, r;
	unsigned char c;

	if (argc == 2)
	{
		drive = 0;
		ncyl = getCilindro(atoi(argv[1]));
		nhead = getSuperficie(atoi(argv[1]));
		nsec = getSecFis(atoi(argv[1]));

		if (*argv[1] > 43188)
		{
			fprintf(stderr, "Sector invalido\n");
			exit(1);
		}
	}
	if (vdreadseclog(atoi(argv[1]), buffer) == -1)
	{
		fprintf(stderr, "Error al abrir disco virtual\n");
		exit(1);
	}

	printf("Desplegando de disco%d.vd Cil=%d, Sup=%d, Sec=%d\n", drive, ncyl, nhead, nsec);

	for (i = 0; i < SECSIZE / LINESIZE; i++)
	{
		printf("\n %3X -->", i * LINESIZE);
		for (j = 0; j < LINESIZE; j++)
		{
			c = buffer[i * LINESIZE + j];
			printf("%2X ", c);
		}
		printf("  |  ");
		for (j = 0; j < LINESIZE; j++)
		{
			c = buffer[i * LINESIZE + j] % 256;
			if (c > 0x1F && c < 127)
				printf("%c", c);
			else
				printf(".");
		}
	}
	printf("\n");
}
