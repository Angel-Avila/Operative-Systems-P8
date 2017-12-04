#define TAMBLOQUE 1024

struct OPENFILES {
	int inuse;		// 0 cerrado, 1 abierto
	unsigned short inode;
	int currpos;
	int currbloqueenmemoria;
	char buffer[1024];
	unsigned short buffindirect[512]; //
};

int vdopen(char *filename,unsigned short mode);
int vdcreat(char *filename,unsigned short perms);
int vdunlink(char *filename);
int vdseek(int fd, int offset, int whence);
int vdwrite(int fd, char *buffer, int bytes);
int vdread(int fd, char *buffer, int bytes);
int vdclose(int fd);
unsigned short *postoptr(int fd,int pos);
unsigned short *currpostoptr(int fd);
