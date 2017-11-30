#pragma pack(1)
// Debe medir 64 bytes, importante es que el tamaño sea potencia de 2
struct INODE {
	char name[18];							// 144 bits - 18 bytes
	unsigned int datetimecreat;	// 32 bits  - 4 bytes
	unsigned int datetimemodif;	// 32 bits  - 4 bytes
	unsigned int datetimelaacc; // 32 bits  - 4 bytes
	unsigned short uid;		// 16 bits  			- 2 bytes
	unsigned short gid;		// 16 bits  			- 2 bytes
	unsigned short perms;	// 16 bits  			- 2 bytes
	unsigned int size;			// 32 bits  		- 4 bytes
	unsigned short direct_blocks[10];	// 10 x 16 bits = 20 bytes
	unsigned short indirect;	// 16 bits  	- 2 bytes
	unsigned short indirect2;	// 16 bits  	- 2 bytes
};

struct DATE {
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
};

int setninode(int num, char *filename,unsigned short atribs, int uid, int gid);
int searchinode(char *filename);
int removeinode(int numinode);

unsigned int datetoint(struct DATE date);
int inttodate(struct DATE *date,unsigned int val);
unsigned int currdatetimetoint();
