all:	vdfdisk.c
		gcc -g -O -c phys2log.c
		gcc -g -O -c mapainodoshandler.c
		gcc -g -O -c mapadatoshandler.c
		gcc -g -O -c blockhandler.c
		gcc -g -O -c inodehandler.c
		-rm -f disco0.vd
		./createvd 0
		gcc -o vdfdisk vdfdisk.c vdisk.o phys2log.o mapainodoshandler.o mapadatoshandler.o blockhandler.o inodehandler.o
		./vdfdisk
