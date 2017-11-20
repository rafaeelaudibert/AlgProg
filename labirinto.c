//Headers includes
#include "main.h"
#include "structs.h"
#include "labirinto.h"
#include "ghosts.h"
#include "auxiliars.h"

//Inicia e carrega todas as estruturas
int startLab(char lab[HEIGHT][WIDTH], int *qtdePastilhas, pacmanInfo *pacman, ghosts *fantasmas)
{
    int i, j; //Contador do laço da matriz
    int q_fantasmas=0;

    fantasmas->quant=0;
    *qtdePastilhas=0;

    // standard setup of the array dir[];
    setupDir();

    //Chamada das outras duas funções responsáveis pelo labirinto
    if(readLab(lab))
    {
        return 1;
    }

    //Printagem do labirinto na tela
    gotoXY(1,1);
    for(i=0; i<HEIGHT; i++)
    {
        for(j=0; j<WIDTH; j++)
        {
            switch(lab[i][j])
            {
            case 'c':
            case 'C':
                //Origem
                pacman->origin.x = j+1;
                pacman->origin.y = i+1;

                //Atual
                pacman->pos.x = j+1;
                pacman->pos.y=i+1;

                //Matriz
                lab[i][j] = ' ';
                break;
            case 'w':
            case 'W': // Fantasmas
                //Origem
                fantasmas->unid[q_fantasmas].alive = 1; // seta a vida do ghost
                fantasmas->unid[q_fantasmas].origin.x = j;
                fantasmas->unid[q_fantasmas].origin.y = i;
                fantasmas->unid[q_fantasmas].key='W';

                //Atual
                fantasmas->unid[q_fantasmas].pos.x = j;
                fantasmas->unid[q_fantasmas].pos.y = i;

                //Matriz e Contadores
                fantasmas->quant++;
                q_fantasmas++;
                lab[i][j] = ' ';
                break;
            case 'o':
                (*qtdePastilhas)++;
                break;
            }
        }
    }

    return 0;
}


//Mostra o labirinto na tela
int showLab(char lab[HEIGHT][WIDTH], pacmanInfo *pacman, ghosts *fantasmas)
{

    int i, j; //Contador do laço da matriz
    int q_fantasmas=fantasmas->quant;

    //Printagem/reprintagem do labirinto na tela
    gotoXY(1,1);
    for(i=0; i<HEIGHT; i++)
    {
        for(j=0; j<WIDTH; j++)
        {
            switch(lab[i][j])
            {
            case '#':
                textcolor(BRANCO);
                printf("#");
                break;
            case 'o':
                textcolor(AMARELO);
                printf("o");
                break;
            case '*':
                textcolor(VERMELHO);
                printf("*");
                break;
            default:
                printf(" ");
                break;
            }
        }
    }

    //Seta a posição inicial do pacman
    pacman->pos.x=pacman->origin.x;
    pacman->pos.y=pacman->origin.y;

    // seta os fantasmas para as posições iniciais
    for(i=0; i<q_fantasmas; i++)
    {
        fantasmas->unid[i].pos.x = fantasmas->unid[i].origin.x;
        fantasmas->unid[i].pos.y = fantasmas->unid[i].origin.y;
        fantasmas->unid[i].alive = 1; // seta a vida do ghost
    }
    return 0;
}

//Função que lê o arquivo labirinto.txt e configura a matriz do labirinto
int readLab(char lab[HEIGHT][WIDTH])
{
    char str[101];
    int i=0, j=0;
    FILE *arq;

    //Ponteiro para leitura do arquivo no 'endereco'
    arq = fopen(LAB, "r");

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
