#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "../labirinto.h"

void gotoxy(int, int);
void textcolor(int);

int const WIDTH_SCREEN = 100;
int const HEIGHT_SCREEN = 30;

void clrscr();

//definição estruturas
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

struct
{
    int x;
    int y;
} typedef pacmanPos;

struct
{
    int x;
    int y;
} typedef coord;

//instâncias globais dos objetos
ghosts fantasmas;
pacmanPos pacman = {1,1};
coord pastilhas[2000]; //Setado para um maximo de 2000 Pastilhas (Provavelmente não será maior do que 1500, embora o maximo possivel seja 2994)
coord superPastilhas[4]; //Setado para um maximo de 4 Super Pastilhas


// Função que lê o arquivo labirinto.txt e configura a matriz do labirinto
// retorna 1 se houve algum erro
// retorna 0 se tudo ocorreu corretamente
int readLab(char lab[30][100])
{
    char str[101];
    FILE *arq;

    // ponteiro para leitura do arquivo no 'endereco'
    arq = fopen("data/labirinto.txt", "r");

    // se não conseguiu ler o arquivo, sai e retorna 0
    if(arq == NULL)
    {
        printf("ERROR: nao foi possivel abrir o arquivo\n");
        return 1;
    }

    // i = linha, j = coluna
    int i=0, j=0;
    // lê cada uma das linhas do arquivo
    while( fgets(str, 101, arq) != NULL || i<30)
    {
        // se a leitura retornar uma linha em branco, ignora
        if(str[0] != '\n')
        {
            // senão, adiciona a matriz lab a linha
            for(j=0; j<100; j++)
            {
                lab[i][j] = str[j];
            }
            // próxima linha da matriz
            i++;
        }
    }

    return 0;
}

void showLab(char lab[30][100])
{
    if(readLab(lab)){
        return;
    }
    iniciaLabirinto(lab);

    int w = 100, h=30;
    int i, j;

    gotoxy(1,1);

    //Printagem do labirinto na tela
    for(i=0; i<h; i++)
    {
        for(j=0; j<w; j++)
        {
            if(lab[i][j]=='#')
            {
                textcolor(15);
                printf("%c", lab[i][j]);
            }
            else if(lab[i][j]=='o')
            {
                textcolor(14);
                printf("%c", lab[i][j]);
            }
            else
            {
                textcolor(0);
                printf(" ");
            }
            textcolor(15);
        }
        if(i<h-1)
        {
            printf("\n");
        }
    }

    return;
}

// inicia e carrega as structs
void iniciaLabirinto(char lab[30][100])
{
    int q_fantasmas=0, q_pastilhas=0, q_superPastilhas=0;
    // mode já deve estar configurado

    int i, j;

    // Carrega a localização de cada um dos objetos, e remove eles do labirinto, se necessario
    for(i=0; i<30; i++)
    {
        for(j=0; j<100; j++)
        {
            switch(lab[i][j])
            {
            case 'c':
            case 'C': // PacMan
                pacman.x = j;
                pacman.y = i;
                lab[i][j] = ' ';
                break;
            case 'w':
            case 'W': // Fantasmas
                fantasmas.unid[q_fantasmas].x = j;
                fantasmas.unid[q_fantasmas].y = i;
                q_fantasmas++;
                lab[i][j] = ' ';
                break;
            case '*': // Super-Pastilhas
                superPastilhas[q_superPastilhas ].x = j;
                superPastilhas[q_superPastilhas ].y = i;
                q_superPastilhas++;
                break;
            case 'o': // Pastilhas
                pastilhas[q_pastilhas].x = j;
                pastilhas[q_pastilhas].y = i;
                q_pastilhas++;
                break;
            }
        }
    }

    fantasmas.quant=q_fantasmas;

    return;
}

//2 FUNÇÕES ABAIXO NÃO FUNCIONAM CORRETAMENTE
//BUGADO
int retornaXPacman(void)
{

    return 2;
}

int retornaYPacman(void)
{

    return 2;
}

