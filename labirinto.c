#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "labirinto.h"

void gotoxy(int, int);
void textcolor(int);

//Definição das estruturas
struct
{
    int x;
    int y;
} typedef ghost;

struct
{
    int quant;
    ghost unid[5];
} typedef ghosts;

//instâncias globais dos objetos
ghosts fantasmas;
extern int HEIGHT;
extern int WIDTH;


//Inicia e carrega as estruturas, além de mostrar o labirinto na tela
int showLab(char lab[HEIGHT][WIDTH], int *qtdePastilhas, int *pacmanx, int *pacmany)
{

    int i, j; //Contador do laço da matriz
    int q_fantasmas=0, q_pastilhas=0; //Contagem de objetos

    //Chamada das outras duas funções responsáveis pelo labirinto
    if(readLab(lab))
    {
        return 1;
    }

    //Printagem do labirinto na tela
    for(i=0; i<HEIGHT; i++)
    {
        for(j=0; j<WIDTH; j++)
        {
            switch(lab[i][j])
            {
            case 'c':
            case 'C':
                printf(" ");
                *pacmanx=j+1;
                *pacmany=i+1;
                lab[i][j] = ' ';
                break;
            case 'w':
            case 'W': // Fantasmas
                printf(" ");
                fantasmas.unid[q_fantasmas].x = j;
                fantasmas.unid[q_fantasmas].y = i;
                q_fantasmas++;
                lab[i][j] = ' ';
                break;
            case '#':
                textcolor(15); //BRANCO
                printf("#");
                break;
            case 'o':
                textcolor(14); //AMARELO
                printf("o");
                q_pastilhas++;
                break;
            case '*':
                textcolor(13); //ROSA
                printf("*");
                break;
            default:
                printf(" ");
                break;
            }
        }

    }

    fantasmas.quant=q_fantasmas;

    *qtdePastilhas=q_pastilhas; //Seta a quantidade total de pastilhas

    return 0;
}

//Função que lê o arquivo labirinto.txt e configura a matriz do labirinto
//Retorna 1 se houve algum erro
int readLab(char lab[HEIGHT][WIDTH])
{
    char str[101];
    int i=0, j=0;
    FILE *arq;

    //Ponteiro para leitura do arquivo no 'endereco'
    arq = fopen("data/labirinto.txt", "r");

    //Se não conseguiu ler o arquivo, sai e retorna 1
    if(arq == NULL)
    {
        printf("ERROR: nao foi possivel abrir o arquivo\n");
        return 1;
    }


    //Lê cada uma das linhas do arquivo
    while(fgets(str, 101, arq) != NULL || i<30)
    {
        //Se a leitura retornar uma linha em branco, ignora
        if(str[0] != '\n')
        {
            //Senão, adiciona à matriz lab uma linha
            for(j=0; j<100; j++)
            {
                lab[i][j] = str[j];
            }

            i++;
        }
    }

    return 0;
}

