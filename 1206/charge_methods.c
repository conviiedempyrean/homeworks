#include <stdio.h>

int main() {
    	int count = 0;
    	int target = 100;
    
    	printf("凑成%d元的所有方式(使用50, 20, 10):\n", target);
    	printf("\n");
    	printf("序号\t50元\t20元\t10元\t\n");
    	printf("\n");
    
    	for (int i = 0; i <= target / 50; i++) {
        	for (int j = 0; j <= target / 20; j++) {
        	    	int remaining = target - 50 * i - 20 * j;
           		if (remaining >= 0 && remaining % 10 == 0) 
			{
          	    		int k = remaining / 10;  
               			count++;
				printf("%2d\t %d\t %d\t %d\t\n", count, i, j, k);
            		}
       	 	}
    	}
   	printf("\n");
   	printf("共 %d 种不同的换算方式\n", count);
   	return 0;
}