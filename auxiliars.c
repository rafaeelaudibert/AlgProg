//Headers includes
#include "main.h"
#include "structs.h"
#include "auxiliars.h"

///Reads the file with the settings of the game
void readSettings(int *difficulty, int *speed, int *map, char *up, char *down, char *right, char *left, char *stop)
{

    FILE* arq;

    arq=fopen("data/settings.bin", "rb");

    if(!arq)
    {
        *difficulty=6;
        *speed=115;
        *map=1;
        *up='W';
        *down='X';
        *right='D';
        *left='A';
        *stop='S';
    }
    else
    {
        fread(difficulty, sizeof(int), 1, arq);
        fread(speed, sizeof(int), 1, arq);
        fread(map, sizeof(int), 1, arq);
        fread(up, sizeof(char), 1, arq);
        fread(down, sizeof(char), 1, arq);
        fread(right, sizeof(char), 1, arq);
        fread(left, sizeof(char), 1, arq);
        fread(stop, sizeof(char), 1, arq);
    }

    fclose(arq);

    return;

}


///Writes the settings of the game in the file, after they have been changed
void writeSettings(int difficulty, int speed, int map, char up, char down, char right, char left, char stop)
{

    FILE* arq;

    arq=fopen("data/settings.bin", "wb");
    if(!arq)
    {
        gotoXY(18,33);
        printf("It wasn't possible to save the configurations to the next games!");
    }
    else
    {
        fwrite(&difficulty, sizeof(int), 1, arq);
        fwrite(&speed, sizeof(int), 1, arq);
        fwrite(&map, sizeof(int), 1, arq);
        fwrite(&up, sizeof(char), 1, arq);
        fwrite(&down, sizeof(char), 1, arq);
        fwrite(&right, sizeof(char), 1, arq);
        fwrite(&left, sizeof(char), 1, arq);
        fwrite(&stop, sizeof(char), 1, arq);
    }
    fclose(arq);
    return;
}


///Reconstructs part of the lab, as asked by the parameters
void reconstructMaze(int y_inicial, int y_final, int x_inicial, int x_final, char lab[HEIGHT][WIDTH], pacmanInfo pacman)
{

    int contador, count;
    for(contador=y_inicial; contador<=y_final; contador++)
    {
        for (count=x_inicial; count<=x_final; count++)
        {
            gotoXY(count+1,contador+1);
            if(lab[contador][count]=='#')
            {
                textcolor(CINZA_CLARO);
                printf("%c", PAREDE);
            }
            else if(lab[contador][count]=='o')
            {
                textcolor(AMARELO);
                printf("%c", PACDOT);
            }
            else if(lab[contador][count]=='*')
            {
                textcolor(VERMELHO);
                printf("%c", POWERPELLET);
            }
            else
            {
                printf(" ");
            }
        }
    }

    textcolor(AMARELO);
    gotoXY(pacman.pos.x, pacman.pos.y);
    printf("C");

    return;
}

///Puts the ConsoleCursor in a given x,y position
void gotoXY(int x, int y)
{
    //Limit: Has limit equal to x-1, y-1 as of the values passed on system(mode x,y)
    COORD coord;
    coord.X = x-1;
    coord.Y = y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

///Selects cursor appearance.
void cursorType(int cursor)
{
    CONSOLE_CURSOR_INFO cursorInfo;

    switch(cursor)
    {
    case 0: //Turns off the cursor
        cursorInfo.dwSize = 100;
        cursorInfo.bVisible = FALSE;
        break;
    case 1: //Solid block cursor
        cursorInfo.dwSize = 100;
        cursorInfo.bVisible = TRUE;
        break;
    case 2:
        cursorInfo.dwSize = 10; //Normal underscore cursor
        cursorInfo.bVisible = TRUE;
        break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}


