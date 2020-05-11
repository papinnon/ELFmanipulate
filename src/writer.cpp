#include "./pfile.hpp"
#include <string.h>

int main(int argc, char ** argv)
{
	//SwipeELF(strtoull(argv[2],NULL, 16), strtoull(argv[3],NULL,16));
try{
	pfile myelf("./test101");
	//myelf.offset_del(0xf00, 0x100);
	myelf.replace((void*)"\x24\x10",0x18,2);
	myelf.close();
}
catch(fileExcept & e)
{
	e.what();
}
	//myelf.replace((void*)"\x30\x0f", 0x18, 2);
}
