//Headers includes
#include "main.h"
#include "structs.h"
#include "ghosts.h"
#include "auxiliars.h"
#include "labirinto.h"


//Inicia e carrega todas as estruturas
int startLab(char lab[HEIGHT][WIDTH], int *qtdePastilhas, pacmanInfo *pacman, ghosts *fantasmas, int labOption)
{
    int i, j; //Contador do laço da matriz

    //Inicialização de algumas variáveis
    *qtdePastilhas=0;
    fantasmas->quant=0;

    // standard setup of the array dir[];
    setupDir();

    //Chamada das outras duas funções responsáveis pelo labirinto
    if(readLab(lab, labOption))
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
                //Origin do pacman
                pacman->origin.x = j+1;
                pacman->origin.y = i+1;

                //Atual do pacman
                pacman->pos.x = j+1;
                pacman->pos.y = i+1;

                lab[i][j] = ' ';
                break;
            case 'w':
            case 'W': // Fantasmas
                if(fantasmas->quant < MAX_GHOSTS)
                {
                    //Origin dos fantasmas
                    fantasmas->unid[fantasmas->quant].origin.x=j;
                    fantasmas->unid[fantasmas->quant].origin.y=i;

                    //Posição atual dos fantasmas
                    fantasmas->unid[fantasmas->quant].pos.x = j;
                    fantasmas->unid[fantasmas->quant].pos.y = i;
                    fantasmas->unid[fantasmas->quant].alive = 1; // seta a vida do ghost
                    fantasmas->unid[fantasmas->quant].key = 'W';
                    (fantasmas->quant)++;
                }
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
                textcolor(CINZA_CLARO);
                printf("%c", PAREDE);
                break;
            case 'o':
                textcolor(AMARELO);
                printf("%c", PACDOT);
                break;
            case '*':
                textcolor(VERMELHO);
                printf("%c", POWERPELLET);
                break;
            default:
                printf(" ");
                break;
            }
        }
        //printf("\n");
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
int readLab(char lab[HEIGHT][WIDTH], int labOption)
{
    char str[101];
    int i=0, j=0;
    FILE *arq;
    char url[30];

    switch(labOption){
case 1:
    strcpy(url, "data/labirinto.txt");
    break;
case 2:
    strcpy(url, "data/labirintoOneDot.txt");
    break;
case 3:
    strcpy(url, "data/labirintoProfessor.txt");
    break;
    }

    //Ponteiro para leitura do arquivo no 'endereco'
    arq = fopen(url, "r");

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
