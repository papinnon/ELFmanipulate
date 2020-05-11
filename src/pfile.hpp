#ifndef PFILE_H
#define PFILE_H

#include <string>
using std::string;
typedef unsigned long long int u64;

class pfile{
	FILE * fp;
	int 	size;
	u64 	cur;
	string 	path;
	string 	mode;
	enum args{
		MAX_FILENAME = 255,
		MAX_PATH = 0x1000
	};
	public:
	pfile():fp(NULL),size(0),cur(0),path(),mode(){}
	pfile(const char * path);
	int open(const char * path, const char * mode);
	int close();
	int offset_del(u64 off, u64 count);
	int replace(void * from, u64 offset, u64 count);
	u64 calcsize();
	
};
#include "pfile.cpp"
#endif
