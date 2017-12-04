//Headers includes
#include "main.h"
#include "structs.h"
#include "ghosts.h"
#include "pacman.h"
#include "objects.h"
#include "auxiliars.h"

//Pacman Controller
int pacmanControl(int* qtde_pacdots, int* points, pacmanInfo* pacman, clock_t* pacStartTimer, char lab[HEIGHT][WIDTH], ghostsInfo* fantasmas, int speed, int condition)
{
    float correcaoVelocidade= pacman->last.coordinates=='y' ? 1.4 : 1; //Letter distorction correction
    clock_t pacEndTimer=clock(); //Verifies actual system time

    if((pacEndTimer-(*pacStartTimer))>speed*correcaoVelocidade) //After the game has taken 'speed' time, executes the movement of the pacman
    {
        *pacStartTimer=pacEndTimer; //"Start counter=0"
        movePacman(pacman, lab, points, condition);
        checkPacDots(qtde_pacdots, points, lab, *pacman);
        checkPowerPellets(points, lab, pacman, fantasmas, speed);

        if(checkGhostCollision((*pacman), points, fantasmas) && !pacman->pacDotActive) //Checks collision with the ghost
        {
            return 0;
        }

        gotoXY(1,1);
    }

    return 1;

}

///Movement and printing of the pacman in the right position
void movePacman(pacmanInfo *pacman, char lab[HEIGHT][WIDTH], int* points, int condition)
{

    gotoXY(pacman->pos.x, pacman->pos.y); //Erases the actual pacman position
    printf(" ");

    switch(pacman->next.coordinates) //Especulates the next pacman position
    {
    case 'y':
        pacman->pos.y+=pacman->next.up_down;
        break;
    case 'x':
        pacman->pos.x+=pacman->next.up_down;
        break;
    }

    if(condition==1){
        testColision(pacman, lab); //If pacman is inside a wall, takes it out from there
    }else{
        testWalls(pacman, lab, points); //TOP SECRET
    }
    testLimits(pacman); //If pacman is out of the map limits, prints it the right place

    //Prints new pacman position
    textcolor(AMARELO);
    gotoXY(pacman->pos.x,pacman->pos.y);
    printf("%c", PAC);

    return;
}

///Test map limits
void testLimits(pacmanInfo *pacman)
{

    if(pacman->pos.y < TOP)
    {
        pacman->pos.y = HEIGHT;
    }
    else if(pacman->pos.y > HEIGHT)
    {
        pacman->pos.y = TOP;
    }
    else if(pacman->pos.x < LEFT)
    {
        pacman->pos.x = WIDTH;
    }
    else if(pacman->pos.x > WIDTH)
    {
        pacman->pos.x = LEFT;
    }

    return;
}

///Tests pacman collisions with walls
void testColision(pacmanInfo *pacman, char lab[HEIGHT][WIDTH])
{

    if(lab[pacman->pos.y-1][pacman->pos.x-1]=='#' && (pacman->pos.y > TOP-1 && pacman->pos.y < HEIGHT+1) && (pacman->pos.x > LEFT-1 && pacman->pos.x < WIDTH+1)) //If inside a wall
    {
        switch(pacman->next.coordinates) //Undo the 'entering in the wall' movement
        {
        case 'y':
            pacman->pos.y -= pacman->next.up_down;
            break;
        case 'x':
            pacman->pos.x -= pacman->next.up_down;
            break;
        }

        //Makes the pacman keeps walking
        switch(pacman->last.coordinates)
        {
        case 'y':
            pacman->pos.y += pacman->last.up_down;
            break;
        case 'x':
            pacman->pos.x += pacman->last.up_down;
            break;
        }

        //If in a corner, and pacman is forced to get in a wall, makes it come back again
        if(lab[pacman->pos.y-1][pacman->pos.x-1]=='#')
        {
            switch(pacman->last.coordinates)
            {
            case 'y':
                pacman->pos.y -= pacman->last.up_down;
                break;
            case 'x':
                pacman->pos.x -= pacman->last.up_down;
                break;
            }
        }
    }

    else //Else, agrees with the movement action, and sets as a correct movement
    {
        pacman->last.coordinates=pacman->next.coordinates;
        pacman->last.up_down=pacman->next.up_down;
    }

    return;
}

///TOP SECRET
void testWalls(pacmanInfo *pacman, char lab[HEIGHT][WIDTH], int *points){

    if(lab[pacman->pos.y-1][pacman->pos.x-1]=='#' && (pacman->pos.y > TOP-1 && pacman->pos.y < HEIGHT+1) && (pacman->pos.x > LEFT-1 && pacman->pos.x < WIDTH+1)){
        *points+=5000;
        //lab[pacman->pos.y-1][pacman->pos.x-1]=' '; 4fun

        textcolor(BRANCO);
        gotoXY(36, 32);
        printf("Points: %5d", *points);
    }

    return;
}

///Sets the direction that the pacman must go
void setDirection(char key, int* continua, pacmanInfo *pacman, int *condition)
{

    switch(key) //Verifies the pressed key
    {
    case 'w':
        pacman->next.coordinates='y'; //Sets 'y-axis' direction
        pacman->next.up_down=-1; //Sets negative direction
        break;
    case 'x':
        pacman->next.coordinates='y';
        pacman->next.up_down=1; //Sets positive direction
        break;
    case 'a':
        pacman->next.coordinates='x'; //Sets 'x-axis' direction
        pacman->next.up_down=-1;
        break;
    case 'd':
        pacman->next.coordinates='x';
        pacman->next.up_down=1;
        break;
    case 's':
        pacman->next.coordinates='s'; //Stop the pacman
        pacman->next.up_down=0;
        break;
    case 'p': //Pauses the game when 'p' is pressed
        gamePause();
        break;
    case 'b': //TOP SECRET
        *condition=0;
        break;
    case 't': //TOP SECRET
        *condition=1;
        break;
    case ' ':
        *continua=0; //Makes the game to end
        pacman->next.coordinates='s'; //Stops the pacman movement
        pacman->next.up_down=0;
        break;
    }

    return;

}
