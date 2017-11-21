//Headers includes
#include "main.h"
#include "structs.h"
#include "labirinto.h"
#include "pacman.h"
#include "ghosts.h"
#include "objects.h"
#include "messages.h"
#include "auxiliars.h"

//Checagem das pastilhas
void checkPacDots(int* pacdots, int* points, char lab[HEIGHT][WIDTH], pacmanInfo pacman)
{

    if(lab[pacman.y-1][pacman.x-1]=='o')
    {
        lab[pacman.y-1][pacman.x-1]=' ';
        *points+=10; //Aumenta contador de pontos
        *pacdots+=1; //Incrementa 1 no contador de pastilhas comidas

        textcolor(BRANCO);
        gotoXY(36, 32);
        printf("Points: %5d", *points);
    }

}

//Checagem das super-pastilhas
void checkPowerPellets(int* points, char lab[HEIGHT][WIDTH], pacmanInfo *pacman)
{

    if(lab[pacman->y-1][pacman->x-1]=='*')
    {
        lab[pacman->y-1][pacman->x-1]=' ';
        *points+=50;
        Beep(1000, 50);
        pacman->pacDotActive=5000/NORMAL_SPEED; //Fica mais rapido pela quantidade de clocks possiveis em 5 segundos

        textcolor(BRANCO);
        gotoXY(36, 32);
        printf("Points: %5d", *points);
    }

    //Alteração da velocidade, e timer da duração do super-poder
    if(pacman->pacDotActive>0)
    {
        pacman->pacDotActive--;
        textcolor(BRANCO);
        gotoXY(32,33);
        printf("Remaining invulnerability time: %4dms", pacman->pacDotActive*NORMAL_SPEED);
    }
    else
    {
        gotoXY(29,33);
        printf("                                             ");
    }
}
