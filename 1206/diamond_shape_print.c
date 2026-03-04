#include <stdio.h>

int main()
{
	int dia_high = 11;
	int dia_width = dia_high;
	int layer = 0,column = 0;
	int n=0;
	int mid = (dia_high-1)/2;
	for(layer=0;layer<dia_high;layer++)
	{
        	for(column=0;column<dia_width;column++)
		{
			if(column<mid-n||column>mid+n)
			{
				putchar(' ');
			}
			else if((column>=mid-n) && (column<=mid+n))
			{
				putchar('*');
			}
		}
	if(layer<mid)
	{
		n++;
	}
	else if(layer>=mid)
	{
		n--;
	}
	putchar('\n');
	}
return 0;
}