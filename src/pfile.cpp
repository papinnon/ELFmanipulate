#ifndef PFILE_CPP
#define PFILE_CPP
#include "pfile.hpp"
#include "except.hpp"

pfile::pfile(const char * path)
{
	open(path,"rw+");
	mode = "rw+";
	size = calcsize();
	cur =0;
}

#include <stdio.h>
int pfile::open(const char * path, const char * mode)
{
	this->path = string(path);
	if(this->path.length() > MAX_PATH)
		throw fileExcept(string_format("open: path exceed MAX_PATH : %d", MAX_PATH).c_str());
	fp = fopen(this->path.c_str(),mode);
	if(!fp)
	{
		throw fileExcept("open: fp is NULL");
	}
	calcsize();
	this->mode = string(mode);
	return 0;
}

int pfile::close()
{
	::fflush(fp);
	::fclose(fp);
	fp = NULL;
	return 0;
}
u64 pfile::calcsize()
{
	if(!fp)
		throw fileExcept("calcsize: fp is NULL");
	fseek(fp, 0, SEEK_END);
	this->size = (u64)ftell(fp);
	return this->size;
}


int pfile::offset_del(u64 off, u64 count)
{
	char *	buf1 = (char * )malloc(off);
	char * 	buf2 = (char *)malloc(this->size-off-count);
	if(off+count > size)
		throw fileExcept("offset_del: offset overflow");
	if(!fp)
		throw fileExcept("offset_del: fp is NULL");
	fseek(fp, 0, SEEK_SET);
	fread(buf1, 1, off, fp);
	fseek(fp, count, SEEK_CUR);
	if(feof(fp))
		fseek(fp, 0, SEEK_END);
	fread(buf2, 1, size -off-count, fp);
	rewind(fp);
	fclose(fp);
	fp = NULL;
	fp = fopen(path.c_str(), "w+");
	fwrite(buf1,1, off, fp);
	fwrite(buf2, 1, size -off-count, fp);
	fflush(fp);
	fclose(fp);
	fp = NULL;
	fp = fopen(path.c_str(), mode.c_str());
	return 0;
}
int pfile::replace(void * from, u64 offset, u64 count)
{
	char * buf;
	if(!fp)
		throw fileExcept("Replace: fp is NULL");
	buf = (char *)malloc(size);
	fseek(fp, offset+count, SEEK_SET);
	fread(buf, 1, size-offset-count, fp);
	fseek(fp, offset, SEEK_SET);
	fwrite(from, 1, count, fp);
	fwrite(buf, 1, size-offset-count, fp);
	fflush(fp);
	return 0;

}
#endif
