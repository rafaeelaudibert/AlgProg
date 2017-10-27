#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <conio.h> //Para utilizar funções de movimentações do jogo
#include <windows.h> //Para utilizar o tosleep();
#include <ctype.h> //Para utilizar o tolower();
#include "labirinto.h"

#define SPEED 160  //Velocidade padrao do jogo
#define CURSOR 0 // 0 para sem cursor; 1 para cursor de caixa; 2 para cursor normal

//Structs locais
struct lastDirection
{
    char coordinates; //x, y ou s
    int aumenta_diminui; //1 - Aumenta; -1 - Diminui; 0 - parado
};

struct pacmanPosition
{
    int x;
    int y;
    int lives;
};

//Funções da conio.h
void clrscr(void);
void gotoxy(int x, int y);
void highvideo(void);
void _setcursortype(int _NOCURSOR); //Função para modificar o tipo de cursor

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


//Variaveis Globais
struct pacmanPosition pacman;
char lab[30][100];

//Constantes
int const   TOP = 1,
            LEFT = 1,
            HEIGHT = 24,
            WIDTH = 80,
            PACMAN_START_X=2,
            PACMAN_START_Y=2;


//Pac-man Main
int main()
{

    system("mode 100, 50");
    highvideo(); //Increased contrast on screen
    _setcursortype(CURSOR); //Makes cursor not show
    menuStartGame(); //Start message
    pacman.lives=3;


    readLab("data/labirinto.txt", lab);
    pacmanStart(); //The Game

    return EXIT_SUCCESS;
}




//Funções Locais
void menuStartGame(void) //Menu de inicio de jogo
{
    system("color 0E");
    printf("                  ############            \n");
    printf("               ##################         \n");
    printf("             ######################       \n");
    printf("           ##################   #####     \n");
    printf("          ###################    #####    \n");
    printf("         ##############################   \n");
    printf("         ###########################      \n");
    printf("        #########################         \n");
    printf("        ######################            \n");
    printf("        ##################                \n");
    printf("        ##################                \n");
    printf("        ######################            \n");
    printf("        #########################         \n");
    printf("         ###########################      \n");
    printf("         ##############################   \n");
    printf("          ############################    \n");
    printf("           ##########################     \n");
    printf("             ######################       \n");
    printf("               ##################         \n");
    printf("                  ############            \n");

    printf("\n     #####################################\n");
    printf("                  PAC-MAN                 \n");
    printf("       Press any key to start the game    \n");
    printf("     #####################################\n                          ");
    getch();
    clrscr();
    system("color 0F");

    return;
}

void pacmanStart(void) //Jogo em si
{
    int flag = 1, flag_2=1, count;
    char key;
    struct lastDirection lastDirection; //Struct que armazena dados sobre a ultima direcao andada

    pacman.x=PACMAN_START_X;
    pacman.y=PACMAN_START_Y;

    while(flag)
    {


        gotoxy(8, 25);
        printf("Press 'space' or 'esc' to quit"); //Mensagem de saida

        if (flag_2==1)  //Mensagem de inicio
        {
            gotoxy(12,13);
            printf("Press any  key to start");
        }

        if (flag_2==0)
        {
            showLab(lab);
        }


        if(kbhit())
        {
            //Detecta teclas pressionadas


            key = tolower(detectKey(lastDirection));
            flag_2--;

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

                for(count=3; count>0; count--)
                {
                    gotoxy(19, 14); //Contagem regressiva ao voltar para o jogo
                    printf("%d seconds...", count);
                    Sleep(1000);
                }

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
    char key='m'; //Inicializa com qualquer outro valor

    gotoxy(19,12);
    printf("Game paused!");
    gotoxy(16,13);
    printf("Press 'R' to resume");

    while(key!='r')
    {
        if(kbhit())
        {
            if(GetAsyncKeyState(0x52))
            {
                key='r';
            }
        }
    }

    return;

}

void pacmanEnd(void) //Termina o jogo
{
    clrscr();
    gotoxy(1,1);
    printf("The program will be finished!\n");

    system("pause");

    return;
}

void makeLastDirection(struct lastDirection last) //Contabiliza a proxima posição, com base na ultima posição
{
    switch(last.coordinates) //Verifica a ultima direção que foi solicitado para andar
    {
    case 'x':
        if (last.aumenta_diminui==1)
        {
            pacman.x++;
        }
        else
        {
            pacman.x--;
        }
        break;
    case 'y':
        if (last.aumenta_diminui==1)
        {
            pacman.y++;
        }
        else
        {
            pacman.y--;
        }
    case 's':
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
            gotoxy(pacman.x, pacman.y-1);
            printf(" ");
            gotoxy(pacman.x, pacman.y);
            printf("%c", 'C');
            gotoxy(pacman.x, pacman.y);
        }
        else
        {
            gotoxy(pacman.x, pacman.y+1);
            printf(" ");
            gotoxy(pacman.x, pacman.y);
            printf("%c", 'C');
            gotoxy(pacman.x, pacman.y);
        }
        break;
    case 'x':
        if(last.aumenta_diminui==1)
        {
            gotoxy(pacman.x-1, pacman.y);

            printf(" %c", 'C');
        }
        else
        {
            gotoxy(pacman.x, pacman.y);

            printf("%c ", 'C');

        }
        break;
    }



    gotoxy(pacman.x, pacman.y);
    printf("%c", 'C');
    gotoxy(pacman.x, pacman.y);

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

//TODO:

//CMD
//Arrumar tamanho do buffer
//Testar System(mode ...);
//Arrumar impressão do labirinto
//Transformar pacman da tela inicial em um arquivo.txt
//Mensagem contadora dos pontos

//PACMAN
//Detecção da posição inicial do Pacman, passada para a struct  ...
//Detecção de colisão com as paredes
//Detecção de colisão com os fantasmas, pacman.lives--, Respawn; pacman.lives!=0? pacmanStart() : EndGame();
//Respwan - if pacman.lives==0; not Restart;
//Reinicio do jogo, após sua morte, por mais 2x

//FANTASMAS
//Movimentação dos fantasmas (Movimentação com SuperPastilha OFF/ON - Movimentação 30% menor)
//Timer para ressuscitar
//Na struct, adicionar parametro 'Alive' (0 - Morto, 1 - Vivo)

//PASTILHAS
//Criar detector de "comeu pastilha"
//Criar contador de pontos

//SUPER-PASTILHAS
//Criar função da super-pastilha
//Criar time - "contador" da super-pastilha

//EXTRAS:
//Dijkshtra
//Sistema de som
//Highscores
//Adicionar Cheat no F9 (Desativa deteccção de colisão com as paredes - Paredes valem 10000 pontos)



//FIM DO ARQUIVO
