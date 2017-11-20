//Headers includes
#include "main.h"
#include "structs.h"
#include "labirinto.h"
#include "pacman.h"
#include "ghosts.h"
#include "objects.h"
#include "messages.h"
#include "auxiliars.h"

//Função que reconstroi parte do labirinto conforme parametros passados
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
                textcolor(BRANCO);
                printf("%c", lab[contador][count]);
            }
            else if(lab[contador][count]=='o')
            {
                textcolor(AMARELO);
                printf("%c", lab[contador][count]);
            }
            else
            {
                printf(" ");
            }

        }
    }

    textcolor(AMARELO);
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

    switch(cursor)
    {
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

