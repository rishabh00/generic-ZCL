#include "myutility.h"

void flush()
{
	char c;
	while((c=getchar())!='\n');
}

void clrscr()
{
	printf("\e[1;1H\e[2J");
}

void end()
{
	printf("--- Press ENTER ----\n");
	flush();
}
