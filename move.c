#include <stdio.h>
#include <stdlib.h>

int main(){
	char first, sec, third;
	char str[5] = {0};
	printf("Digite 0 para sair\n");
	do {
		printf("digite um character: ");
//		scanf("%s", str);
		str[0] = getchar();
		if(str[0] == 27){
			str[1] = getchar();
			str[2] = getchar();
			str[3] = getchar();
		}
		printf("%d %d %d %d %d\n", str[0], str[1], str[2], str[3], str[4]);
		str[1] = 0;
		str[2] = 0;
		str[3] = 0;
		str[4] = 0;
	} while(first != '0');
	
	return 0;
}
