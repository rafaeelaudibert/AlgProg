#ifndef LIBRARIES_H_
#define LIBRARIES_H_ //Libraries of the entire project

#include <stdio.h> //To use I/O commands
#include <stdlib.h> //To use CMD commands
#include <windows.h> //To detect keystrokes
#include <ctype.h> //To use tolower();
#include <time.h> //To use clock();
#include <conio.h> //To use getch() & kbhit();
#include <string.h> //To use gets() & fgets();
#include <math.h> //To use pow();

#endif // LIBARIES_H_

#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_

int main(void);
void gameStart(int*, int*, int, int, int, int, char, char, char, char, char); ///Starts the game
void gamePause(void); ///Pauses the game
void gameEnd(void); ///Finishes the game, when given up by the player
void gameWin(int); ///Finishes the game, when won by the player
void gameLost(void); ///Finishes the game, when lost by the player
void beepWin(void); ///Sound when the player wins the game
void beepLost(void); ///Sound when the pacman dies
char detectKey(char, char, char, char, char); ///Detects the stroked key

#endif // MAIN_FUNCTIONS_H_


#ifndef ENUMS_H_
#define ENUMS_H_

enum colors
{
    PRETO=0,
    CINZA_CLARO=7,
    CINZA, //7+1
    AZUL,
    VERDE,
    VERDE_AGUA,
    VERMELHO,
    LILAS,
    AMARELO,
    BRANCO //15
};

#endif // ENUMS_H_


#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define TOP 1                       //Top map limit (Never less than 1)
#define LEFT 1                      //Left map limit (Never less than 1)
#define HEIGHT 30                   //Bottom map limit (Size in the vertical)
#define WIDTH 100                   //Right map limit (Size in the horizontal)
#define CURSOR 0                    //0 - no cursor; 1 - box cursor; 2 - normal cursor
#define QT_MAP 4                    //Quantity of available maps
#define MAX_RANDOM 10               //Random function will generate a number in the interval [0,MAX_RANDOM]
#define MAX_GHOSTS 5                //Maximum number of ghosts in the lab
#define RESPAWN 7000                //After this much of clocks, the game tries to revive a Ghost, if there is one already dead

#endif // CONSTANTS_H_

#ifndef LAB_CHARS_H_
#define LAB_CHARS_H_

#define PAREDE 35                   //219 to print █, 35 to print #
#define PAC 67                      //67 to print C
#define PACDOT 111                  //250 to print ·, 111 to print o
#define POWERPELLET 42              //184 to print ©, 169 to print ®, 42 to print *

#endif // LAB_CHARS_H_
