#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "../labirinto.h"


void gotoxy(int x, int y);
void textcolor(int newcolor);

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
} typedef pacmanS;

struct
{
    int x;
    int y;
} typedef coord;

//instâncias globais dos objetos
ghosts fantasmas;
pacmanS pacmanTeste;

coord pastilhas[100];
int q_pastilhas = 0;

coord superPastilhas[10];
int q_superPastilhas = 0;

// função que lê o arquivo labirinto.txt e configura todos os objetos lidos
// retorna 0 se houve algum erro
// retorna 1 se tudo ocorreu corretamente
int readLab(char endereco[], char lab[30][100])
{
    char str[150];
    FILE *arq;

    // ponteiro para leitura do arquivo no 'endereco'
    arq = fopen(endereco, "r");

    // se não conseguiu ler o arquivo, sai e retorna 0
    if(arq == NULL)
    {
        printf("ERROR: nao foi possivel abrir o arquivo\n");
        return 0;
    }

    // i = linha, j = coluna
    int i=0, j=0;
    // lê cada uma das linhas do arquivo
    while( fgets(str, 101, arq) != NULL || i<30){
        // se a leitura retornar uma linha em branco, ignora
        if(str[0] != '\n'){
            // senão, adiciona a matriz lab a linha
            for(j=0; j<100; j++){
                lab[i][j] = str[j];
            }
            // próxima linha da matriz
            i++;
        }
    }

    return 1;
}

int carregado = 0;

void showLab(char lab[30][100])
{
    clrscr();
    int w = 100;
    int h = 30;

    int i, j;
    textcolor(7);
    gotoxy(1,1);

    for(i=0; i<h; i++)
    {
        for(j=0; j<w; j++)
        {
            if(lab[i][j]=='o')
            {
                textcolor(14);
                printf("%c", lab[i][j]);
            }
            else if(lab[i][j]=='#')
            {
                textcolor(15);
                printf("%c", lab[i][j]);
            }
            else
            {
                textcolor(13);
                printf(" ");
            }


            textcolor(15);
        }
        if(i<h-1){
        printf("\n");
        }


    }


    // se tiver carregado as structs no iniciaLabirinto(), mostra elas nos pontos e com outras cores
    // por enquanto meio bugado
    if(carregado)
    {
        textcolor(2);
        gotoxy(1, 1);
        printf("R");
        textcolor(6);
        gotoxy(pacmanTeste.x, pacmanTeste.y);
        printf("C");
        printf("\nposx %d, posy %d\n", pacmanTeste.x, pacmanTeste.y);
    }
    textcolor(7);
}

// inicia e carrega as structs
int iniciaLabirinto(char lab[30][100], char endereco[])
{
    fantasmas.quant = 0;
    int q=0;
    // mode já deve estar configurado
    if( !readLab(endereco, lab))
    {
        printf("Error: leitura incompleta!\n");
        return 0;
    }

    int i, j;
    //showLab(lab);
    //printf("\nCarregado puro\n");
    //printf("Carregando, agora, somente labirinto ");
    //(i=0; i<8; i++)
    //{
    //    Sleep(300);
    //    printf(".");
    //}

    printf("\n");
    // carrega cada um dos objectos e suas localizações originais
    for(i=0; i<30; i++)
    {
        for(j=0; j<100; j++)
        {
            switch(lab[i][j])
            {
            case 'c':
                // pacman
                pacmanTeste.x = j;
                pacmanTeste.y = i;
                lab[i][j] = ' ';
                printf("\nPosicao do pacman:\nx: %d, y: %d\n", pacmanTeste.x, pacmanTeste.y);
                break;
            case 'w':
                // fantasmas
                fantasmas.unid[ q ].x = j;
                fantasmas.unid[ q ].y = i;
                q++;
                lab[i][j] = ' ';
                printf("\nPosicao do fantasma %d: x=%d e y=%d", q, i, j);
                break;
            case '*':
                superPastilhas[ q_superPastilhas ].x = j;
                superPastilhas[ q_superPastilhas ].y = i;
                q_superPastilhas++;
                lab[i][j] = ' ';
                printf("\nPosicao da super-pastilha %d: x=%i e y=%d", q_superPastilhas, i, j);
                break;
            case 'o':
                pastilhas[ q_pastilhas ].x = j;
                pastilhas[ q_pastilhas ].y = i;
                q_pastilhas++;
                lab[i][j] = ' ';
                break;
            default:
                // nothing
                break;
            }
        }
    }

    fantasmas.quant=q;
    // objetos setados e carregados, carregado = 1
    carregado = 0; // 0 enquanto estiver bugado
    showLab(lab);

    printf("\nCarregado apos lido");
    printf("\nQuantidade de pastilhas: %d                      \n", q_pastilhas);
    return 1;
}

int main2()
{
    char lab[30][100];

    /* RODE ISSO PARA SOMENTE TESTAR A EXECUÇÃO DO LABIRINTO
    system("mode 100,50");
    readLab("labirinto.txt", lab);
    showLab(lab);
    }
    */

    // RODE ISSO PARA TESTAR O LABIRINTO COMPLETO COM INCLUSÃO DAS ESTRUTURAS
    testaLabirinto();

    return 0;
}


void testaLabirinto()
{
    // [ linha ] [ coluna ]
    char lab[HEIGHT_SCREEN][WIDTH_SCREEN];
    // tamanho da tela do CMD, em caracteres
    system("mode 100, 50");
    // lê o arquivo e verifica se ocorreu algum erro
    if( !readLab("labirinto.txt", lab) )
    {
        printf("\nErro: leitura incompleta!\n");
    }
    else
    {
        iniciaLabirinto(lab, "labirinto.txt");
    }

    printf("\n");
    system("pause");

    return;
}
