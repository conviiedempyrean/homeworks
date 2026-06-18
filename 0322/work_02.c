#include <stdio.h>
#include <string.h>
#include <ctype.h>

void capitalizeWords(char *s)
{
    int i = 0;
    int newWord = 1;

    while (s[i] != '\0') 
    {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n') 
        {
            newWord = 1;
        } 
        else if (newWord) 
        {
            if (islower((unsigned char)s[i])) 
            {
                s[i] = toupper((unsigned char)s[i]);
            }
            newWord = 0;
        }
        i++;
    }
}

int main(int argc, char *argv[])
{
    char input[256];

    printf("请输入字符串：");
    if (fgets(input, sizeof(input), stdin) == NULL) 
    {
        return 1;
    }

    input[strcspn(input, "\r\n")] = '\0';

    capitalizeWords(input);
    printf("处理后字符串：%s\n", input);

    return 0;
}
