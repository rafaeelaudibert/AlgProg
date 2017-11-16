#include <stdio.h>
#include <stdlib.h>

struct
{
    char nome[30];
    int pontos;
    char date[15];
    char time[15];
} typedef DADOS;

//Programa para resetar o arquivo Highscores.txt
int main() 
{

    DADOS Highscore={"-----",0,"--/--/--","--:--:--"};
    FILE *arq;
    char url[25]= {"../data/Highscores.txt"};
    int i;

    arq = fopen(url, "wt");
    
    if (arq == NULL) // Se não conseguiu criar
    {
        printf("Problemas na CRIACAO do arquivo\n");
	system("pause");
        return;
    }
    else //Grava dados no arquivo
    {
        for(i=0; i<10; i++)
        {

            fprintf(arq,"%d\n",i+1); //Colocação, começando do 1
            fprintf(arq,"%s\n",Highscore.nome);
            fprintf(arq,"%d\n",Highscore.pontos);
            fprintf(arq,"%s\n",Highscore.date);
            fprintf(arq,"%s\n",Highscore.time);

        }
    }

    fclose(arq);

    return 0;
}
