#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int judge(int);

int main()
{
	int user;
	printf("\"0\" for Rock,\"1\" for scissors,\"2\" for paper\nenter your choose:");
	scanf("%d",&user);
 	int ret=judge(user);
	switch(ret)
	{
		case 1:
		printf("You win\n");
		break;
		case 0:
		printf("You drew\n");
		break;
		case -1:
		printf("You lose\n");
		break;
	}
	return 0;
}

int judge(int u)
{
    srand(time(NULL));
	int ret=0;
 	int m=(int)rand() % 3;
	if(u==m)
	ret=0;
	else
	{
		if((u+1)%3==m)
		ret=1;
		else
		ret=-1;
	}
	return ret;
}

