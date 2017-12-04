all:	vdfdisk.c
		gcc -g -O -c phys2log.c
		gcc -g -O -c mapainodoshandler.c
		gcc -g -O -c mapadatoshandler.c
		gcc -g -O -c blockhandler.c
		gcc -g -O -c inodehandler.c
		gcc -o dumpsec dumpsec.c phys2log.o vdisk.o
		gcc -o dumpseclog dumpseclog.c phys2log.o vdisk.o
		gcc -o -c filehandler.c vdisk.o phys2log.o mapainodoshandler.o mapadatoshandler.o blockhandler.o inodehandler.o
		-rm -f disco0.vd
		./createvd 0
		gcc -o vdfdisk vdfdisk.c vdisk.o phys2log.o mapainodoshandler.o mapadatoshandler.o blockhandler.o inodehandler.o
		gcc -o shell shell.c mapainodoshandler.o mapadatoshandler.o inodehandler.o blockhandler.o phys2log.o filehandler.o
		./vdfdisk
