//Headers includes
#include "main.h"
#include "structs.h"
#include "labirinto.h"
#include "pacman.h"
#include "ghosts.h"
#include "objects.h"
#include "messages.h"
#include "auxiliars.h"


///Variaveis Globais
pacmanInfo pacman; //Pacman informations
ghosts fantasmas; //Ghost informations
char lab[30][100]; //Variable whose stores the maze
clock_t pacStartTimer, ghostsTime; //Game timers

///Pac-man Main
int main()
{
    //Setting variables
    int points=0; //Points counter
    int totalPacDots, eatenPacDots=0; //PacDots quantities

    //Initial definitions
    system("mode 100, 37"); //Defines CMD's screen size
    system("title Pacman - AlgProg - 2017/2"); //Defines CMD's title
    cursorType(CURSOR); //Sets the cursor according to a value declared in the constant 'CURSOR' (main.h)
    srand(time(NULL)); // Feeds the rand seed with the system time

    //Setting some useful pacman data
    pacman.lives=3; //Sets the initial number of lives of the pacman
    pacman.pacDotActive=0; //Sets pacman "not powered"

    startLab(lab, &totalPacDots, &pacman, &fantasmas); // Set the initial positions of the game
    startMenu(); //Start message

    //THE GAME
    while(pacman.lives>=0) //While pacman still has lives, keeps playing the game
    {
        pacman.lives--; //When starts the game, takes one live out of pacman
        if(pacman.lives>=0)
        {
            gameStart(&points, &eatenPacDots, totalPacDots); //The Game 'per se'
            while(kbhit())
            {
                getch();
            }
        }
    }
    //END OF THE GAME

    if(pacman.lives==-1) //If the game ends with 0 lives, shows that the player lost
    {
        gameLost();
    }

    if(pacman.lives!=-2)
    {
        highscores(points); //Highscore Table
    }


    gotoXY(1,40);
    textcolor(PRETO);
    return EXIT_SUCCESS; //End of the program
}



///Start of the game
void gameStart(int *points, int *eatenPacDots, int totalPacDots)
{
    int showStartMessage=1; //Starting Message Flag
    int continueGame=1;//Game loop flag
    char key='j'; //Stroked key with a non useful, but known value

    clock_t lastRevive = 0;

    system("cls");
    if(showLab(lab, &pacman, &fantasmas))  //Loads the maze, pac's, pacDots's, powerPellets' & ghost's coordinates in the memory
    {
        printf("ERROR!");
        system("Pause");
        return; //If there is an error at loading it, finishes the game
    }

    //Exit message
    textcolor(BRANCO);
    gotoXY(36, 31);
    printf("Press 'Space' or 'ESC' to quit");
    gotoXY(36, 32);
    printf("Points: %5d         Lives: %d", *points, pacman.lives);

    //Starts timers, and makes the game to start
    pacStartTimer=clock();
    ghostsTime=clock();
    while(continueGame)
    {
        if(showStartMessage>-1)
        {
            showStartMessage=startMessage(showStartMessage, pacman, lab); //Shows start message when necessary
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
            setDirection(key, &continueGame, &pacman); //Decodifies pressed key
        }


        if(!pacmanControl(eatenPacDots, points, &pacman, &pacStartTimer, lab, &fantasmas))  //Controls pacman
        {
            pacman.next.coordinates='s';
            pacman.next.aumenta_diminui='0';
            return;  //Ends the game loop, if there is a collision between the ghost and the pacman

        }

        int RESPAWN = 8000;
        // respawn the ghosts
        if( (clock() - lastRevive) > RESPAWN){
           reviveGhosts(&fantasmas);
           lastRevive = clock();
        }

        if(key!='j' && !ghostsControl(points, pacman, &ghostsTime, lab, &fantasmas)) //Controls ghosts
        {
            pacman.next.coordinates='s';
            pacman.next.aumenta_diminui='0';
            return; //Ends the game loop, if there is a collision between the ghost and the pacman
        }

        if((*eatenPacDots)==totalPacDots) //If all pacDots have been eaten, ends the game
        {
            (*points)+=((pacman.lives+1)*150); //Gives 150 points, each lasting life
            gameWin((*points));
            return;
        }

    } //WHILE END

    gameEnd(); //Ends the game
    pacman.lives=-2; //This way, the Highscore menu is not shown

    return;
}

///Pausa o jogo
void gamePause(void)
{
    char key='m'; //Inicializa com qualquer outro valor, para nao cair no While
    int count;

    textcolor(BRANCO);
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
            if(GetAsyncKeyState(0x52))  //Ao clicar no R sair� desse loop
            {
                key='r';
            }
        }
    }

    for(count=3; count>0; count--)  //Contagem regressiva ao voltar para o jogo
    {
        textcolor(BRANCO);
        gotoXY(45, 16);
        printf("%d seconds...", count);
        Sleep(1000);
    }

    reconstructMaze(12,18,40,62, lab, pacman);

    return;
}

///Mensagem de t�rmino de jogo, caso jogador simplesmente desista
void gameEnd(void)
{
    textcolor(BRANCO);
    gotoXY(36,31);
    printf("The program will be finished!  ");
    gotoXY(32,32);
    printf("Press any key twice to close the game");
    gotoXY(29,33);
    printf("                                              ");

    textcolor(PRETO); //Deixa texto em preto, tornando-o invisivel
    gotoXY(1,33);
    system("pause");
    textcolor(BRANCO);

}

//Mensagem de t�rmino de jogo, caso player ganhe o jogo
void gameWin(int points)
{
    int contador=0;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/pacmanWin.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    system("cls");
    gotoXY(26, contador+6);
    Sleep(50);
    while( (ch=fgetc(arq))!=EOF)   //Impress�o do PacMan
    {
        if(contador>19)
        {
            textcolor(BRANCO);
        }
        else
        {
            textcolor(AMARELO);
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
    textcolor(PRETO);
    system("pause");
    pacman.lives=-3;
    return;
}

///Mensagem de t�rmino de jogo, caso player perca o jogo
void gameLost(void)
{
    int contador=0;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/pacmanLost.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    system("cls");
    gotoXY(26, contador+6);
    Sleep(50);
    while( (ch=fgetc(arq))!=EOF)   //Impress�o do PacMan
    {
        if(contador>19)
        {
            textcolor(BRANCO);
        }
        else
        {
            textcolor(AMARELO);
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
    textcolor(PRETO);
    system("pause");
    return;
}


///Detecta a tecla pressionada
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
    else if (GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_SPACE)) //Tecla 'ESC' ou tecla 'Espa�o'
    {
        key = ' ';
    }
    else //Sen�o retorna uma letra que n�o significa nenhuma dire��o, assim ser� utilizada a ultima dire��o andada
    {
        key='m';
    }

    return key;

}


/*
TODO LIST:

� CMD
  - COMPLETO!!!

� PACMAN
  - COMPLETO!!

� FANTASMAS
  - Timer para ressuscitar

� PASTILHAS
  - COMPLETO!!!

� SUPER-PASTILHAS
  - COMPLETO!!!

� EXTRAS:
  Dijkshtra - Sistema de detec��o de caminho dos fantasmas mais inteligente, utilizando grafos
  HIGHSCORES -> FEITO
  EFEITOS SONOROS -> Super-pastilhas -> OK
                     Morte PACMAN -> TODO
                     Vitoria PACMAN -> TODO
  Adicionar Cheat no F9 (Desativa detecc��o de colis�o com as paredes - Paredes valem 10000 pontos)
  Menu inicial, com sele��es de mapas e/ou dificuldades + cr�ditos

  FIM DO ARQUIVO
*/
