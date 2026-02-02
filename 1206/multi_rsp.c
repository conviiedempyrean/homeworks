#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int judge(int);
void set_stdin_empty();

int main()
{
int user;
char choose=0;
while(1)
{
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
	printf("wanna try again?\n\"y\" for yes, \"n\" for no\n");
	set_stdin_empty();
	scanf("%c",&choose);
	if(choose == 'n')
	{
		break;
	}
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

void set_stdin_empty()
{
char ch = '\0';
scanf("%c",&ch);
while(ch != '\n')
{
scanf("%c",&ch);
}
}