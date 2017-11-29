//Headers includes
#include "main.h"
#include "structs.h"
#include "labirinto.h"
#include "pacman.h"
#include "ghosts.h"
#include "objects.h"
#include "messages.h"
#include "auxiliars.h"
#include "menu.h"
#include "resources.h"

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
    clock_t initialTimer; //Start timer

    int difficulty, speed, map;  //Controllers of the game
    char up, down, right, left, stop; //Shortcuts of movement

    //Initial definitions
    system("mode 100, 38"); //Defines CMD's screen size
    system("title Pacman - AlgProg - 2017/2"); //Defines CMD's title
    cursorType(CURSOR); //Sets the cursor according to a value declared in the constant 'CURSOR' (main.h)
    srand(time(NULL)); // Feeds the rand seed with the system time

    //Setting some useful game data
    readSettings(&difficulty, &speed, &map, &up, &down, &right, &left, &stop); //Reads the main variables of the game from a file
    pacman.lives=menu(&difficulty, &speed, &map, &up, &down, &right, &left, &stop); //Sets pacman lives and load the main menu
    pacman.pacDotActive=0; //Sets pacman "not powered"

    if(pacman.lives!=-2) //If the options in menu wasn't to exit the game
    {
        system("cls"); //Cleans the screen
        startLab(lab, &totalPacDots, &pacman, &fantasmas, map); // Set the initial positions of the game
        startMenu(); //Start message
        initialTimer=clock();
    }
    else //Shows the closing game message
    {
        system("cls"); //Cleans the screen
        textcolor(BRANCO);
        gotoXY(40,18);
        printf("The game will be closed!");
        gotoXY(46,19);
        printf("_____________");
        gotoXY(46,20);
        printf("Press any key");
    }

    //THE GAME
    while(pacman.lives>=0) //While pacman still has lives, keeps playing the game
    {
        pacman.lives--; //When starts the game, takes one live out of pacman
        if(pacman.lives>=0)
        {
            gameStart(&points, &eatenPacDots, totalPacDots, difficulty, speed, map, up, down, right, left, stop); //The Game 'per se'
            if(pacman.lives>=0){
                beepLost();
            }
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
        highscores(points, (clock()-initialTimer)); //Highscore Table
    }


    gotoXY(1,40);
    textcolor(PRETO);
    return EXIT_SUCCESS; //End of the program

}



///Start of the game
void gameStart(int *points, int *eatenPacDots, int totalPacDots, int difficulty, int speed, int map, char up, char down, char right, char left, char stop)
{
    int showStartMessage=1; //Starting Message Flag
    int continueGame=1;//Game loop flag
    int condition=1;//TOP SECRET
    char key='j'; //Stroked key with a non useful, but known value
    clock_t lastReviveTry = 0; //Last time the ghosts tried to respawn

    system("cls");
    if(showLab(lab, &pacman, &fantasmas))  //Loads the maze, pac's, pacDots's, powerPellets' & ghost's coordinates in the memory
    {
        printf("ERROR!");
        system("Pause");
        return; //If there is an error at loading it, finishes the game
    }

    //Information message
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

            key=tolower(detectKey(up, down, right, left, stop)); //Detects pressed key
            setDirection(key, &continueGame, &pacman, &condition); //Decodifies pressed key
        }


        if(!pacmanControl(eatenPacDots, points, &pacman, &pacStartTimer, lab, &fantasmas, speed, condition))  //Controls pacman
        {
            pacman.next.coordinates='s';
            pacman.next.up_down='0';
            return;  //Ends the game loop, if there is a collision between the ghost and the pacman

        }

        // Respawn the ghosts
        if( (clock() - lastReviveTry) > RESPAWN)
        {
            reviveGhosts(&fantasmas, speed);
            lastReviveTry = clock();
        }

        if(key!='j' && !ghostsControl(points, pacman, &ghostsTime, lab, &fantasmas, speed, difficulty)) //Controls the ghosts
        {
            pacman.next.coordinates='s';
            pacman.next.up_down='0';
            return; //Ends the game loop, if there is a collision between the ghost and the pacman
        }

        if((*eatenPacDots)==totalPacDots) //If all pacDots have been eaten, ends the game
        {
            (*points)+=((pacman.lives+1)*150); //Gives 150 points, each lasting life
            beepWin();
            gameWin((*points));
            return;
        }

    } //WHILE END

    gameEnd(); //Ends the game
    pacman.lives=-2; //This way, the Highscore menu is not shown

    return;
}

///Pauses the game
void gamePause(void)
{
    char key='m';
    int count;
    textcolor(BRANCO);
    gotoXY(39,12);
    printf("/---------------------\\");
    gotoXY(39,13);
    printf("|                     |");
    gotoXY(39,14);
    printf("|    Game paused!     |");
    gotoXY(39,15);
    printf("| Press 'R' to resume |");
    gotoXY(39,16);
    printf("|                     |");
    gotoXY(39,17);
    printf("|                     |");
    gotoXY(39,18);
    printf("\\---------------------/");


    while(key!='r'){
        if(GetAsyncKeyState(0x52)){ //'R' key
            key='r';
        }
    }

    for(count=3; count>0; count--)  //Countdown
    {
        textcolor(BRANCO);
        gotoXY(45, 16);
        printf("%d seconds...", count);
        Sleep(1000000/CLOCKS_PER_SEC); //1 second delay
    }

    reconstructMaze(11,18,38,65, lab, pacman);

    return;
}

///End message, if the player gives up
void gameEnd(void)
{
    textcolor(BRANCO);
    gotoXY(36,31);
    printf("The program will be finished!  ");
    gotoXY(32,32);
    printf("Press any key twice to close the game");
    gotoXY(29,33);
    printf("                                              ");

    textcolor(PRETO); //Black text, "invisible"
    gotoXY(1,33);
    system("pause");
    textcolor(BRANCO);

}

///End game message, if player has won
void gameWin(int points)
{
    int counter=0;
    char ch;
    FILE *arq;

    arq = fopen("data/pacmanWin.txt", "r");

    system("cls");
    gotoXY(26, counter+6);
    Sleep(50);
    while( (ch=fgetc(arq))!=EOF)   //Message printing
    {
        if(counter>19)
        {
            textcolor(BRANCO);
        }
        else
        {
            textcolor(AMARELO);
        }

        printf("%c", ch);

        if(ch=='\n' && counter<25)
        {
            counter++;
            gotoXY(26, counter+6);
            Sleep(10); //One line each time
        }
    }

    getch();
    printf("\n\n");
    textcolor(PRETO);
    system("pause");
    pacman.lives=-3;
    return;
}

///End game message, if the player has lost it
void gameLost(void)
{
    int contador=0;
    char ch;
    FILE *arq;

    arq = fopen("data/pacmanLost.txt", "r");

    system("cls");
    gotoXY(26, contador+6);
    Sleep(50);
    while( (ch=fgetc(arq))!=EOF)   //Message printing
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
            Sleep(10); //Prints one line each time
        }
    }

    getch();
    printf("\n\n");
    textcolor(PRETO);
    system("pause");
    return;
}


///Detects stroked key
char detectKey(char up, char down, char right, char left, char stop)
{
    char key='m';

    if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(up)) //Up key
    {
        key = 'w';
    }
    else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(down)) //Down key
    {
        key = 'x';
    }
    else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(left)) //Left key
    {
        key = 'a';
    }
    else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(right)) //Right key
    {
        key = 'd';
    }
    else if (GetAsyncKeyState(stop)) //Stop key
    {
        key = 's';
    }
    else if (GetAsyncKeyState(VK_BACK)){ //BackSpace - TOP SECRET
        key = 'b';
    }
    else if (GetAsyncKeyState(VK_TAB)){ //BackSpace - TOP SECRET
        key = 't';
    }
    else if (GetAsyncKeyState(0x50)) //Key 'P' -> Pause
    {
        key = 'p';
    }
    else if (GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_SPACE)) //'Esc' or 'Space' key
    {
        key = ' ';
    }

    return key;

}

void beepLost(void){

Beep(880,40);Sleep(20);
Beep(587,40);Sleep(20);
Beep(830,40);Sleep(20);
Beep(587,40);Sleep(20);
Beep(783,40);Sleep(20);
Beep(587,40);Sleep(20);
Beep(739,40);Sleep(20);
Beep(587,40);Sleep(20);
Beep(698,40);Sleep(20);
Beep(587,40);Sleep(20);
Beep(659,40);Sleep(20);
Beep(587,40);Sleep(20);
Beep(659,40);Sleep(20);
Beep(587,40);Sleep(20);
Beep(880,100);
Sleep(100);
Beep(880,100);

return;
}

void beepWin(void){

Beep(784,60);Sleep(30);
Beep(784,60);Sleep(30);
Beep(784,60);Sleep(120);
Beep(784,120);Sleep(150);
Beep(1150,220);Sleep(50);
Beep(1150,100);Sleep(50);
Beep(1567,500);Sleep(100);
Beep(1567,500);Sleep(100);

    return;

}


/*
TODO LIST:

• EXTRAS:
  HIGHSCORES -> FEITO
  Adicionar Cheat no F10 (Desativa deteccção de colisão com as paredes - Paredes valem 5000 pontos)
  EFEITOS SONOROS -> FEITO
  Menu Inicial -> COMPLETO
  */
