//Libraries inclusion
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //Detect keystrokes
#include <ctype.h> //To use tolower();
#include <time.h> //To use clock();
#include <conio.h> //To use textcolor() & kbhit();

//Local headers inclusions
#include "labirinto.h"


//Structs
struct //Stores axis and direction in it
{
    char coordinates; //'x', 'y' ou 's'
    int aumenta_diminui; //1 - Aumenta; -1 - Diminui; 0 - parado
} typedef directions;

struct //Stores pacman position, and its information
{
    int x; //x-axis position
    int y; //y-axis position
    int lives; //l=Lives remaining
    int pacDotActive; //SuperPower
    directions next; //Next direction to walk
    directions last; //Last direction walked
} typedef pacmanInfo;



//Function Prototypes
void gameStart(void);
void pacmanControl(int*, int*);

void gamePause(void);
void gameEnd(void);
void gameWin(void);
void gameLost(void);
char detectKey(void);

void movePacman(void);
void checkPacDots(int*, int*);
void checkPowerPellets(int*);
void testLimits(void);
void testColision(void);
void setDirection(char, int*);

void startMenu(void);
int startMessage(int);
void reconstructMaze(int, int, int, int);
void gotoXY(int, int);
void cursorType(int);
void textcolor(); //conio.c



//Constants
int  const  TOP = 1, //Top map limit (Never less than 1)
            LEFT = 1, //Left map limit (Never less than 1)
            HEIGHT = 30, //Bottom map limit
            WIDTH = 100, //Right map limit
            CURSOR = 0, // 0 - no cursor; 1 - box cursor; 2 - normal cursor
            NORMAL_SPEED=100, //Default game's speed
            FAST_SPEED=70; //Speed after eating a PowerPellet

//Variaveis Globais
pacmanInfo pacman; //Struct whose stores the pacman
char lab[30][100]; //Variable whose stores the maze
int speed; //Variable whose stores the actual speed of the game
clock_t pacStartTimer, pacEndTimer; //Pacman timers



//Pac-man Main
int main()
{
    system("mode 100, 37"); //Defines screen size
    cursorType(CURSOR); //Sets the cursor according to a value declared in the constant 'CURSOR'

    //startMenu(); //Start message

    pacman.lives=3; //Sets the initial number of lives of the pacman
    speed=NORMAL_SPEED; //Sets the initial speed of the game
    pacman.pacDotActive=0; //Sets pacman "not powered"
    gameStart(); //The Game 'per se'

    return EXIT_SUCCESS; //End of the program
}



//Start of the game
void gameStart(void)
{
    int points=0; //Points counter
    int showStartMessage=1; //Starting Message Flag
    int continueGame=1, *point_continua=&continueGame; //Game loop flag
    int totalPacDots, eatenPacDots=0; //PacDots quantities
    char key; //Stroked key

    pacman.lives--; //When starts the game, takes one live out of pacman
    if(!pacman.lives)  //If pacman has 0 lives, ends the game
    {
        gameLost();
        return;
    }

    if(showLab(lab, &totalPacDots, &pacman.x, &pacman.y))  //Loads the maze, pac's, pacDots's, powerPellets' & ghost's coordinates in the memory
    {
        printf("ERROR!");
        system("Pause");
        return; //If there is an error at loading it, finishes the game
    }

    //Exit message
    gotoXY(36, 31);
    printf("Press 'Space' or 'ESC' to quit");
    gotoXY(46, 32);
    printf("Pontos: %d", points);

    //Starts timer, and makes the game to start
    pacStartTimer=clock();
    while(continueGame)
    {
        if(showStartMessage>-1)
        {
            showStartMessage=startMessage(showStartMessage); //Shows start message when necessarie
        }

        if(kbhit()) //When a key is stroked
        {
            //Flag to make the start message not appear in the next iteration
            //First, it makes not to print the initial message anymore, after rebuilds the maze under it
            if(showStartMessage>-1)
            {
                showStartMessage--;
            }

            key=tolower(detectKey()); //Detects pressed key
            setDirection(key, point_continua); //Decodifies pressed key
        }

        pacmanControl(&eatenPacDots, &points); //Controls pacman

        if(eatenPacDots==totalPacDots) //If all pacDots have been eaten, ends the game
        {
            gameWin();
            return;
        }

    } //WHILE END

    gameEnd(); //Ends the game

    return;
}

//Pacman Controller
void pacmanControl(int* qtde_pacdots, int* points)
{
    float correcaoVelocidade;

    if (pacman.last.coordinates=='y')
    {
        correcaoVelocidade=1.4; //Correção da distorção das letras
    }
    else
    {
        correcaoVelocidade=1;
    }

    pacEndTimer=clock(); //Seta para verificar tempo atual do sistema
    if((pacEndTimer-pacStartTimer)>speed*correcaoVelocidade) //Ao ter passado tempo igual a velocidade, executa o loop
    {
        pacStartTimer=pacEndTimer; //"Zera" o contador inicio
        movePacman();
        checkPacDots(qtde_pacdots, points);
        checkPowerPellets(points);
    }
}



//Pausa o jogo
void gamePause(void)
{
    char key='m'; //Inicializa com qualquer outro valor, para nao cair no While
    int count;

    gotoXY(41,13);
    printf("                     ");
    gotoXY(41,14);
    printf("    Game paused!     ");
    gotoXY(41,15);
    printf(" Press 'R' to resume ");
    gotoXY(41,16);
    printf("                     ");
    gotoXY(41,17);
    printf("                     ");


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
        gotoXY(45, 16);
        printf("%d seconds...", count);
        Sleep(1000);
    }

    reconstructMaze(12,18,40,62);

    return;
}

//Mensagem de término de jogo, caso jogador simplesmente desista
void gameEnd(void)
{
    gotoXY(36,31);
    printf("The program will be finished!  ");
    gotoXY(35,32);
    printf("Press any key to close the game");
    gotoXY(29,33);
    printf("                                              ");

    textcolor(0); //Deixa texto em preto, tornando-o invisivel
    gotoXY(1,33);
    system("pause");
    textcolor(15);

}

//Mensagem de término de jogo, caso player ganhe o jogo
void gameWin(void)
{
    int contador=0;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/pacmanWin.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    system("cls");
    gotoXY(26, contador+6);
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
            gotoXY(26, contador+6);
            Sleep(10); //Imprime uma linha por vez
        }
    }

    getch();
    printf("\n\n");
    textcolor(0);
    system("pause");
    return;
}

//Mensagem de término de jogo, caso player perca o jogo
void gameLost(void)
{
    int contador=0;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/pacmanLost.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    system("cls");
    gotoXY(26, contador+6);
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
            gotoXY(26, contador+6);
            Sleep(10); //Imprime uma linha por vez
        }
    }

    getch();
    printf("\n\n");
    system("pause");
    return;
}

//Detecta a tecla pressionada
char detectKey(void)
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
    else //Senão retorna uma letra que não significa nenhuma direção, assim será utilizada a ultima direção andada
    {
        key='m';
    }

    return key;

}



//Movimentação e impressão do PacMan na posição correta
void movePacman(void)
{

    gotoXY(pacman.x, pacman.y); //Apaga a posição atual do pacman
    printf(" ");

    switch(pacman.next.coordinates) //Calcula a proxima posição do pacman
    {
    case 'y':
        pacman.y+=pacman.next.aumenta_diminui;
        break;
    case 'x':
        pacman.x+=pacman.next.aumenta_diminui;
        break;
    }

    testColision(); //Caso comando coloque o pacman dentro de uma parede, tira ele de lá
    testLimits(); //Caso tenha chegado nos limites do mapa, coloca pacman no outro lado

    //Imprime a nova posição do pacman
    gotoXY(pacman.x,pacman.y);
    printf("C");

    return;
}

//Checagem das pastilhas
void checkPacDots(int* pacdots, int* points)
{

    if(lab[pacman.y-1][pacman.x-1]=='o')
    {
        lab[pacman.y-1][pacman.x-1]=' ';
        *points+=10; //Aumenta contador de pontos
        *pacdots+=1; //Incrementa 1 no contador de pastilhas comidas
        //Beep(500, 30); //- Comentado até sabermos se utilizaremos isso ou não


        gotoXY(46, 32);
        printf("Pontos: %d", *points);
    }

}

//Checagem das super-pastilhas
void checkPowerPellets(int* points)
{

    if(lab[pacman.y-1][pacman.x-1]=='*')
    {
        lab[pacman.y-1][pacman.x-1]=' ';
        *points+=50;
        Beep(1000, 50);
        pacman.pacDotActive=5000/FAST_SPEED; //Fica mais rapido pela quantidade de clocks possiveis em 5 segundos

        gotoXY(46, 32);
        printf("Pontos: %d", *points);
    }

    //Alteração da velocidade, e timer da duração do super-poder
    if(pacman.pacDotActive>0)
    {
        speed=FAST_SPEED;
        pacman.pacDotActive--;
        gotoXY(29,33);
        printf("Tempo restante de invulnerabilidade: %4dms", pacman.pacDotActive*FAST_SPEED);
    }
    else
    {
        speed=NORMAL_SPEED;
        gotoXY(29,33);
        printf("                                             ");
    }
}

//Testa se pacman chegou no limite do mapa, mandando ele para o outro lado
void testLimits(void)
{

    if(pacman.y<TOP)
    {
        pacman.y=HEIGHT;
    }
    else if(pacman.y>HEIGHT)
    {
        pacman.y=TOP;
    }
    else if(pacman.x<LEFT)
    {
        pacman.x=WIDTH;
    }
    else if(pacman.x>WIDTH)
    {
        pacman.x=LEFT;
    }

    return;
}

//Testa colisão do pacman com as paredes
void testColision(void)
{

    if(lab[pacman.y-1][pacman.x-1]=='#') //Caso esteja dentro de um campo 'parede'
    {
        switch(pacman.next.coordinates) //Volta a ultima posição andada
        {
        case 'y':
            pacman.y-=pacman.next.aumenta_diminui;
            break;
        case 'x':
            pacman.x-=pacman.next.aumenta_diminui;
            break;
        }

        if(pacman.next.coordinates!=pacman.last.coordinates) //Faz continuar andando na ultima posição andada nessa iteração
        {
            switch(pacman.last.coordinates)
            {
            case 'y':
                pacman.y+=pacman.last.aumenta_diminui;
                break;
            case 'x':
                pacman.x+=pacman.last.aumenta_diminui;
                break;
            }
        }

        if(lab[pacman.y-1][pacman.x-1]=='#') //Caso ja esteja em uma esquina, e seja forçado a entrar na parede, faz não entrar nela
        {
            switch(pacman.last.coordinates)
            {
            case 'y':
                pacman.y-=pacman.last.aumenta_diminui;
                break;
            case 'x':
                pacman.x-=pacman.last.aumenta_diminui;
                break;
            }
        }

    }
    else //Senão, confirma que ocorreu um movimento valido, e seta a ultima posição para ser igual a atual, para funcionar a proxima iteração
    {
        pacman.last.coordinates=pacman.next.coordinates;
        pacman.last.aumenta_diminui=pacman.next.aumenta_diminui;
    }

    return;
}

//Seta direção que o PacMan irá seguir
void setDirection(char key, int* continua)
{

    switch(key) //Verifica para onde será a nova direção
    {
    case 'w':
        pacman.next.coordinates='y'; //Seta direção no eixo y
        pacman.next.aumenta_diminui=-1; //Seta direção negativa
        break;
    case 'x':
        pacman.next.coordinates='y';
        pacman.next.aumenta_diminui=1; //Seta direção positiva
        break;
    case 'a':
        pacman.next.coordinates='x'; //Seta direção no eixo x
        pacman.next.aumenta_diminui=-1;
        break;
    case 'd':
        pacman.next.coordinates='x';
        pacman.next.aumenta_diminui=1;
        break;
    case 's':
        pacman.next.coordinates='s';
        pacman.next.aumenta_diminui=0;
        break;
    case 'p': //Pausa o jogo ao pressionar 'P'
        gamePause();
        break;
    case ' ':
        *continua=0; //Irá fazer o programa terminar sua execução
        pacman.next.coordinates='s'; //Faz pacman parar sua movimentação
        pacman.next.aumenta_diminui=0;
        break;
    }

}



//Apresenta menu de inicio do jogo
void startMenu(void)
{
    int contador=0;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/pacmanStart.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    gotoXY(26, contador+5);
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
            gotoXY(26, contador+5);
            Sleep((contador-25)*-6); //Acelera a impressão de cada linha aos poucos
        }
    }

    getch();
    system("cls");
    return;
}

//Mensagem que aparece no inicio do jogo
int startMessage(int flag)
{

    if (flag==1)  //Mensagem de inicio
    {
        gotoXY(39, 14);
        printf("                         ");
        gotoXY(39,15);
        printf(" Press any key to start  ");
        gotoXY(39, 16);
        printf("                         ");
    }
    else if(flag==0)
    {
        reconstructMaze(13,15,38,65);
        flag--;
    }

    return flag;
}

//Função que reconstroi parte do labirinto conforme parametros passados
void reconstructMaze(int y_inicial, int y_final, int x_inicial, int x_final)
{

    int contador, count;
    for(contador=y_inicial; contador<=y_final; contador++)
    {
        for (count=x_inicial; count<=x_final; count++)
        {
            gotoXY(count+1,contador+1);
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

    gotoXY(pacman.x, pacman.y);
    printf("C");

    return;
}

// Função gotoXY, tem limite igual a: system("mode 'x-1', 'y-1'");
// índice inicia em 0
void gotoXY(int x, int y)
{
    COORD coord;
    coord.X = x-1;
    coord.Y = y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Selects cursor appearance.
// Sets the cursor type to
//
// _NOCURSOR	Turns off the cursor
// _NORMALCURSOR	Normal underscore cursor
// _SOLIDCURSOR	Solid block cursor
void cursorType(int cursor)
{
   CONSOLE_CURSOR_INFO cursorInfo;

   switch(cursor) {
      case 0:
         cursorInfo.dwSize = 100;
         cursorInfo.bVisible = FALSE;
         break;
      case 1:
         cursorInfo.dwSize = 100;
         cursorInfo.bVisible = TRUE;
         break;
      case 2:
         cursorInfo.dwSize = 10;
         cursorInfo.bVisible = TRUE;
         break;
   }
   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}




/*
TODO LIST:

• CMD
  - COMPLETO!!!

• PACMAN
  - Detecção de colisão com os fantasmas, pacman.lives--, Respawn;

• FANTASMAS
  - Movimentação dos fantasmas (Movimentação com SuperPastilha OFF/ON - Movimentação 30% menor)
  - Timer para ressuscitar
  - Na struct, adicionar parametro 'Alive' (0 - Morto, 1 - Vivo)

• PASTILHAS
  - Criar função que detecta que comeu todas pastilhas -> EndGame - WIN

• SUPER-PASTILHAS
  - Criar ligação entre super-pastilhas e fantasmas



• EXTRAS:
  Dijkshtra - Sistema de detecção de caminho dos fantasmas mais inteligente, utilizando grafos
  Highscores
  Adicionar Cheat no F9 (Desativa deteccção de colisão com as paredes - Paredes valem 10000 pontos)

  FIM DO ARQUIVO
*/
