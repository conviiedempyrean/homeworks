#include <stdio.h>



int main()
{
	int flag = 0;
	printf("here are prime numbers in 2 to 200:\n");
	for(int i=2;i<200;i++)
	{
		flag = 0;
		for(int j = 2;j<i;j++)
		{
			if(i%j==0)
				{
					flag=1;
					break;
				}
		}
		if(flag==0)
		{
			printf("%d\n",i);
		}
	}
}