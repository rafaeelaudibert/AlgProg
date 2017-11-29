//Headers includes
#include "main.h"
#include "structs.h"
#include "objects.h"
#include "auxiliars.h"

///Checks PacDots collision with pacman
void checkPacDots(int* pacdots, int* points, char lab[HEIGHT][WIDTH], pacmanInfo pacman)
{

    if(lab[pacman.pos.y-1][pacman.pos.x-1]=='o') //If pacman is on top of a pacdot
    {
        lab[pacman.pos.y-1][pacman.pos.x-1]=' '; //Takes it out from the lab matrix
        *points+=10; //Increases points counter
        *pacdots+=1; //Increases the eaten pacdots counter

        textcolor(BRANCO);
        gotoXY(36, 32);
        printf("Points: %5d", *points);
    }

    return;

}

///Checks PowerPellets colision with pacman
void checkPowerPellets(int* points, char lab[HEIGHT][WIDTH], pacmanInfo *pacman, ghosts *fantasmas, int speed)
{

    int i; //Counter

    if(lab[pacman->pos.y-1][pacman->pos.x-1]=='*') //If pacman is on top of a PowerPellet
    {
        lab[pacman->pos.y-1][pacman->pos.x-1]=' '; //Takes it out from the lab matrix
        *points+=50; //Increases the point
        Beep(1000, 50);
        pacman->pacDotActive=5000000/CLOCKS_PER_SEC/speed; //Sets a counter to 5 seconds

        for(i=0; i<fantasmas->quant; i++){
            fantasmas->unid[i].key='w'; //Sets the ghosts printing character to 'w'
        }
        textcolor(BRANCO);
        gotoXY(36, 32);
        printf("Points: %5d", *points);
    }

    //Speed change, and Power timer
    if(pacman->pacDotActive>0)
    {
        pacman->pacDotActive--;
        textcolor(BRANCO);
        gotoXY(32,33);
        printf("Remaining invulnerability time: %4ldms", pacman->pacDotActive*speed*1000/CLOCKS_PER_SEC); //Prints the remaining super-power time
    }
    else
    {
        gotoXY(29,33);
        printf("                                             ");

        for(i=0; i < fantasmas->quant; i++){
            fantasmas->unid[i].key='W'; //Changes the character of the ghost back to 'W'
        }
    }
}
