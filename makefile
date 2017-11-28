all:	vdfdisk.c
		gcc -o vdfdisk vdfdisk.c vdisk.o phys2log.o
		gcc -g -O -c phys2log.c

clean:
		rm disco0 *.vd
		./createvd 0
