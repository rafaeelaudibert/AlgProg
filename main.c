//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <conio.h> //Para utilizar funções de movimentações do jogo
#include <windows.h> //Para detectar keystrokes
#include <ctype.h> //Para utilizar o tolower();
#include "labirinto.h"

//DEFINES
#define SPEED 100  //Velocidade padrao do jogo
#define CURSOR 0 // 0 para sem cursor; 1 para cursor de caixa; 2 para cursor normal

//Structs
struct lastDirection
{
    char coordinates; //'x', 'y' ou 's'
    int aumenta_diminui; //1 - Aumenta; -1 - Diminui; 0 - parado
};

struct pacmanPosition
{
    int x;
    int y;
    int lives;
};

struct
{
    char cood;
    int valor;
} typedef nextDirection;

//Inclusão dos protópipos das funções da conio.c
void clrscr(void); //Função que limpa a tela
void gotoxy2(int, int); //Função de movimentação de cursor
void highvideo(void); //Função para aumentar contraste da tela
void _setcursortype(int); //Função para modificar o tipo de cursor
void textcolor(int); //Função que controla cor do texto

//Protótipo das funções locais
void menuStartGame(void);
void pacmanStart(void);
void pacmanPause(void);
void pacmanEnd(void);
void makeLastDirection(struct lastDirection);
void movePacman(struct lastDirection);
void testLimits(void);
void speedControl(struct lastDirection);
char detectKey(struct lastDirection);
void reconstructMaze(int, int, int, int);

// Função gotoxy2, refatorada para ter limite igual ao system("mode x-1, y-1");
// índice inicia em 0
// vai até system(mode x-1, y-1)
void gotoxy2(int x, int y)
{
    COORD coord;
    coord.X = x-1;
    coord.Y = y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Variaveis Globais
struct pacmanPosition pacman;
char lab[30][100];

//Constantes
int const   TOP = 1,
            LEFT = 1,
            HEIGHT = 30,
            WIDTH = 100;

//Pac-man Main
int main()
{
    system("mode 100, 35"); //Define tamanho da tela
    highvideo(); //Increased contrast on screen
    _setcursortype(CURSOR); //Makes cursor not show

    menuStartGame(); //Start message
    pacman.lives=3; //Seta o numero inicial de vidas do PacMan

    pacmanStart(); //The Game
    return EXIT_SUCCESS;
}

//Funções Locais
void menuStartGame(void)
{
    int contador=0;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/pacman.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    gotoxy2(20, contador+1);
    Sleep(50);
    while( (ch=fgetc(arq))!=EOF)   //Impressão do PacMan
    {
        if(contador>19)
        {
            textcolor(15);
        }
        else
        {
            textcolor(14);
        }

        printf("%c", ch);

        if(ch=='\n' && contador<25)
        {
            contador++;
            gotoxy2(20, contador+1);
            Sleep((contador-25)*-15); //Acelera a impressão de cada linha aos poucos
        }
    }

    printf("\n");
    getch();
    fflush(stdin);
    clrscr();
    return;
}

void pacmanStart(void) //Jogo em si
{
    int flag = 1, flag_2=1;
    char key;
    struct lastDirection lastDirection; //Struct que armazena dados sobre a ultima direcao andada

    pacman.x=3; //Bugado
    pacman.y=3; //Bugado

    showLab(lab);

    gotoxy2(36, 31);
    printf("Press 'space' or 'esc' to quit"); //Mensagem de saida

    while(flag)
    {
        gotoxy2(pacman.x, pacman.y);
        printf("C");

        if (flag_2==1)  //Mensagem de inicio
        {
            gotoxy2(39, 14);
            printf("                         ");
            gotoxy2(39,15);
            printf(" Press any key to start  ");
            gotoxy2(39, 16);
            printf("                         ");
        }else if(flag_2==0) //Reconstrução do labirinto embaixo após a mensagem de início
        {
            reconstructMaze(13,15,35,70);
            flag_2--;
        }


        if(kbhit())
        {
            //Detecta teclas pressionadas
            key = tolower(detectKey(lastDirection));

            //Flag para imprimir mensagem inicial, e limpar mensagem inicial
            //Primeiro faz não imprimir mais a mensagem inicial, depois reconstroi o labirinto embaixo dela
            if(flag_2>-1)
            {
                flag_2--;
            }

            switch(key) //Verifica para onde será a nova direção
            {
            case 'w':
                pacman.y--;
                lastDirection.coordinates='y'; //Seta ultima direção no eixo y
                lastDirection.aumenta_diminui=-1; //Seta direção negativa
                break;
            case 'x':
                pacman.y++;
                lastDirection.coordinates='y';
                lastDirection.aumenta_diminui=1; //Seta direção positiva
                break;
            case 'a':
                pacman.x--;
                lastDirection.coordinates='x';
                lastDirection.aumenta_diminui=-1;
                break;
            case 'd':
                pacman.x++;
                lastDirection.coordinates='x';
                lastDirection.aumenta_diminui=1;
                break;
            case 's':
                lastDirection.coordinates='s';
                lastDirection.aumenta_diminui=0;
                break;
            case 'p': //Pausa o jogo ao pressionar 'P'
                pacmanPause();
                break;
            case ' ':
                flag = 0;
                break;
            default:
                makeLastDirection(lastDirection); //Se clicarmos em qualquer outra tecla, continua indo na ultima direcao
            }
        }
        else
        {
            makeLastDirection(lastDirection); //Ao nao clicarmos em nenhuma outra tecla, continua indo na ultima direcao
        }


        //TESTES E MOVIMENTAÇÕES DO JOGO
        testLimits(); //Ao chegar nos limites do mapa, vai para o outro lado
        movePacman(lastDirection); //Realiza movimentação;
        speedControl(lastDirection); //Controla a velocidade do jogo

    } //FIM DO WHILE

    pacmanEnd(); //Finaliza o jogo

    return;

}

void pacmanPause(void)  //Pausa o jogo
{
    char key='m'; //Inicializa com qualquer outro valor, para nao cair no While
    int count;

    gotoxy2(39,11);
    printf("                         ");
    gotoxy2(39,12);
    printf("       Game paused!      ");
    gotoxy2(39,13);
    printf("    Press 'R' to resume  ");
    gotoxy2(39,14);
    printf("                         ");

    while(key!='r')
    {
        if(kbhit())
        {
            if(GetAsyncKeyState(0x52))  //Ao clicar no R sairá desse loop
            {
                key='r';
            }
        }
    }

    for(count=3; count>0; count--)  //Contagem regressiva ao voltar para o jogo
    {
        gotoxy2(46, 14);
        printf("%d seconds...      ", count);
        Sleep(1000);
    }

    reconstructMaze(11,14,38,70);

    return;
}

void pacmanEnd(void) //Termina o jogo
{
    gotoxy2(36,32);
    printf("The program will be finished!");

    gotoxy2(31,33);
    system("pause");

    return;
}

void makeLastDirection(struct lastDirection last) //Contabiliza a proxima posição, com base na ultima posição
{
    switch(last.coordinates)  //Verifica a ultima direção que foi solicitado para andar
    {
    case 'x':
        pacman.x+=last.aumenta_diminui;
        break;
    case 'y':
        pacman.y+=last.aumenta_diminui;
        break;
    default:
        break;
    }

    return;
}

void movePacman(struct lastDirection last) //Movimentação do PacMan
{

    switch(last.coordinates)
    {
    case 'y':
        if (last.aumenta_diminui==1)
        {
            gotoxy2(pacman.x, pacman.y-1);
            printf(" ");
            gotoxy2(pacman.x, pacman.y);
            printf("%c", 'C');
        }
        else
        {
            gotoxy2(pacman.x, pacman.y+1);
            printf(" ");
            gotoxy2(pacman.x, pacman.y);
            printf("%c", 'C');
        }
        break;
    case 'x':
        if(last.aumenta_diminui==1)
        {
            gotoxy2(pacman.x-1, pacman.y);
            printf(" %c", 'C');
        }
        else
        {
            gotoxy2(pacman.x, pacman.y);
            printf("%c ", 'C');
        }
        break;
    }

    return;
}

void testLimits(void) //Testa limites do mapa
{

    if(pacman.y < TOP) //Ao chegar nos limites do mapa, vai para o outro lado
    {
        pacman.y= HEIGHT;
    }
    else if(pacman.y > HEIGHT)
    {
        pacman.y = TOP;
    }
    else if(pacman.x < LEFT)
    {
        pacman.x=WIDTH;
    }
    else if(pacman.x > WIDTH)
    {
        pacman.x = LEFT;
    }

    return;
}

void speedControl(struct lastDirection last) //Controla velocidade do jogo
{

    //Caso o computador esteja com o CMD padrao, usar esse código abaixo
    if (last.coordinates=='y')
    {
        Sleep(SPEED*1.5); //Corrige velocidade no eixo y
    }
    else
    {
        Sleep(SPEED);
    }

    //Caso o computador esteja com o CMD alterado para ter letras quadradas, usar o código abaixo
    //Sleep(SPEED);
}

char detectKey(struct lastDirection last) //Detecta a tecla pressionada
{

    char key;

    if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) //Tecla para cima ou tecla 'W'
    {
        key = 'w';
    }
    else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x58)) //Tecla para baixo ou tecla 'X'
    {
        key = 'x';
    }
    else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) //Tecla para esquerda ou tecla 'A'
    {
        key = 'a';
    }
    else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) //Tecla para direita ou tecla 'D'
    {
        key = 'd';
    }
    else if (GetAsyncKeyState(0x53)) //Tecla 'S'
    {
        key = 's';
    }
    else if (GetAsyncKeyState(0x50)) //Tecla 'P'
    {
        key = 'p';
    }
    else if (GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_SPACE)) //Tecla 'ESC' ou tecla 'Espaço'
    {
        key = ' ';
    }
    else //Senão retorna qualquer tecla, assim será utilizada a ultima direção andada
    {
        key='m';
    }

    return key;

}

void reconstructMaze(int y_inicial, int y_final, int x_inicial, int x_final)
{

    int contador, count;
    for(contador=y_inicial; contador<=y_final; contador++)
    {
        for (count=x_inicial; count<=x_final; count++)
        {
            gotoxy2(count+1,contador+1);
            if(lab[contador][count]=='#')
            {
                textcolor(15);
                printf("%c", lab[contador][count]);
            }
            else if(lab[contador][count]=='o')
            {
                textcolor(14);
                printf("%c", lab[contador][count]);
            }
            else
            {
                printf(" ");
            }

        }
    }

    gotoxy2(pacman.x, pacman.y);
    printf("C");

    return;
}

//TODO:

//CMD
//Mensagem contadora dos pontos
//Mudar timer do jogo, para ser em função do tempo do Sistema

//PACMAN
//Detecção da posição inicial do Pacman, passada para a struct - Implantada, mas bugada
//Detecção de colisão com as paredes
//Detecção de colisão com os fantasmas, pacman.lives--, Respawn; pacman.lives!=0? pacmanStart() : EndGame();
//Respawn - if pacman.lives==0; not Restart;
//Reinicio do jogo, após sua morte, por mais 2x

//FANTASMAS
//Movimentação dos fantasmas (Movimentação com SuperPastilha OFF/ON - Movimentação 30% menor)
//Timer para ressuscitar
//Na struct, adicionar parametro 'Alive' (0 - Morto, 1 - Vivo)

//PASTILHAS
//Criar detector de "comeu pastilha" -> Aumenta pontos
//Criar variavel contadora de pontos

//SUPER-PASTILHAS
//Criar função da super-pastilha
//Criar time - "contador" da super-pastilha

//EXTRAS:
//Dijkshtra
//Sistema de som
//Highscores
//Adicionar Cheat no F9 (Desativa deteccção de colisão com as paredes - Paredes valem 10000 pontos)

//FIM DO ARQUIVO
