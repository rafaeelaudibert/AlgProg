//Headers includes
#include "main.h"
#include "structs.h"
#include "ghosts.h"
#include "objects.h"
#include "auxiliars.h"

//Global Variables

//Available directions -> UP, RIGHT, DOWN, LEFT
coord dir[4]={{0,-1},{1,0},{0,1},{-1,0}};


///Ghost Controller
int ghostsControl(int *points, pacmanInfo pacman, clock_t *ghostsTimer, char lab[HEIGHT][WIDTH], ghostsInfo *ghosts, int speed, int chase_chance)
{

    float speedCorrection = pacman.last.coordinates=='y' ? 1.4 : 1; //Corrects the speed of the game, as the pacman's movement axis changes
    clock_t actualTime=clock();

    if((actualTime - *ghostsTimer) > ( pacman.pacDotActive ? speed*1.3*speedCorrection : speed*speedCorrection))
    {
        *ghostsTimer = actualTime;
        moveGhost(pacman, lab, ghosts, chase_chance); // Updates the movement of the ghosts
        showGhosts(pacman, lab, ghosts, speed); // Updates and shows the position of the ghosts

        if(checkGhostCollision(pacman, points, ghosts) && !pacman.pacDotActive) //If there is collision between a ghost and the pacman
        {
            return 0;
        }
    }
    return 1;
}


///Moves each one of the ghosts
void moveGhost(pacmanInfo pac, char lab[30][100], ghostsInfo *ghosts, int chase_chance)
{

    int i, q = ghosts->quant; //fantasmas.quant;
    // Iterates on each ghost
    for(i=0; i<q; i++)
    {
        // Actual test ghost
        ghost g = ghosts->unid[i];

        // Ghost's chance of stalking the Pacman
        int chance = rand() % MAX_RANDOM;

        //Decisions to change direction
        if(freeSides(g, lab) == 1) //Only one exit, inverts the movement direction
        {
            ghosts->unid[i].mov.y = ghosts->unid[i].mov.y * -1;
            ghosts->unid[i].mov.x = ghosts->unid[i].mov.x * -1;
        }
        else if(changeDirection(g, lab) == 1)
        {

            if(chance < abs(chase_chance-10)) //If doesn't needs to chase
            {

                //Chooses a direction, different from the one that the ghost actually is
                //Randomically sets the direction vector
                shuffleDir();
            }
            else
            {
                //Chooses the path whose makes the distance to the pacman lower
                //Sorts the vector of directions according to cartesian distance to the pacman
                chasePacman(pac, g, lab);
            }
            chooseDirection(pac, &(ghosts->unid[i]), lab);
        }
    }
}


///Prints the ghosts in the screen
void showGhosts(pacmanInfo pac, char lab[30][100], ghostsInfo *ghosts, int speed)
{
    int i;
    for(i=0; i < ghosts->quant; i++) //Reprints the object who was under the ghost
    {
        coord posg; //Decreasing size code/complexity
        posg.x = ghosts->unid[i].pos.x;
        posg.y = ghosts->unid[i].pos.y;
        gotoXY(posg.x+1, posg.y+1);

        //Prints with the right colors
        if(lab[posg.y][posg.x] =='o')
        {
            textcolor(AMARELO);
            printf("%c", PACDOT);
        }
        else if(lab[posg.y][posg.x] =='*')
        {
            textcolor(VERMELHO);
            printf("%c", POWERPELLET);
        }else{
            printf(" ");
        }

        //Updates the position
        if(ghosts->unid[i].alive==1)
        {
            ghosts->unid[i].pos.x += ghosts->unid[i].mov.x;
            ghosts->unid[i].pos.y += ghosts->unid[i].mov.y;
        }

        testGhostLimits(&ghosts->unid[i]);

        if(ghosts->unid[i].alive==1) //If ghost is alive
        {
            //Prints the new position, with the right color
            if(pac.pacDotActive>(2000/speed))
            {
                textcolor(VERDE_AGUA);
            }
            else
            {
                //Makes the ghost blink when super power is ending, or to be the right color when not not powered
                if(pac.pacDotActive%2)
                {
                    textcolor(VERDE_AGUA);
                }
                else
                {
                    textcolor(LILAS);
                }
            }

            gotoXY(ghosts->unid[i].pos.x+1, ghosts->unid[i].pos.y+1);
            printf("%c", ghosts->unid[i].key);
        }
        else if(ghosts->unid[i].alive==2) //If ghost is ressurecting
        {
            if (!(ghosts->unid[i].reviveTime%2))
            {
                textcolor(PRETO); //Blinks the color of the ghost, while ressurecting
            }
            else
            {
                textcolor(LILAS);
            }
            ghosts->unid[i].reviveTime--; //Decreases the lefting ressurecting time

            if(!(ghosts->unid[i].reviveTime))
            {
                ghosts->unid[i].alive=1; //If the time to ressurect is over, ressurects it completely
            }

            gotoXY(ghosts->unid[i].pos.x+1, ghosts->unid[i].pos.y+1);
            printf("%c", ghosts->unid[i].key);
        }
    }
}


///Verifies if the ghost are in a bifurcation or in a corner
//Returns:  1: If needs to decide to change or not direction
//          0: Doesn't need to decide to change direction
int changeDirection(ghost gh, char lab[30][100])
{
    // Verifies which direction is the ghost going
    if( gh.mov.x != 0)
    {
        // If horizontal, verifies if can go up or down
        if( lab[gh.pos.y+1][gh.pos.x] != '#' || lab[gh.pos.y-1][gh.pos.x] != '#') return 1;
    }
    else
    {
        // If vertical, verifies if can go right or left
        if( lab[gh.pos.y][gh.pos.x+1] != '#' || lab[gh.pos.y][gh.pos.x-1] != '#') return 1;
    }
    return 0;
}


///Verifies which are the possible directions for the ghost, and chooses one according to the preference order of the array dir[]
void chooseDirection(pacmanInfo pac, ghost *gh, char lab[30][100])
{
    int i, d;
    ghost g = *gh;

    //Iterates in each of the possible directions
    for(i=0; i<4; i++)
    {
        //Sees if must start in the directions who lets the ghost closer to the pacman, or further
        if(pac.pacDotActive != 0)
        {
            d = i; //Gets closes to the pacman
        }
        else
        {
            d = 3 - i; //Gets further from the pacman
        }

        //Verifies which sides aren't walls
        if( lab[ (g.pos.y + dir[d].y) ][ (g.pos.x + dir[d].x) ] != '#')
        {
            //Makes the ghost never returns to the way it is coming from
            if( dir[d].x != (g.mov.x * -1) ||
                    dir[d].y != (g.mov.y * -1) )
            {
                // Modifies the directions in the ghost
                gh->mov = dir[d];
                d = 5;
            }
        }
    }
}


///Returns an ordered ascending array, according to the cartesian distance between the ghost and the pacman
void chasePacman(pacmanInfo pac, ghost gh, char lab[30][100])
{
    int d, i, t;

    //Distance array in each of the possible directions
    int dists[4];
    for(i=0; i<4; i++)
    {
        // Cartesian Distance - no Sqr (Pithagoras)
        dists[i] = pow( pac.pos.y - gh.pos.y + dir[i].y, 2) + pow(pac.pos.x - gh.pos.x + dir[i].x, 2);
    }

    //Sorts the array dir according to the array dists, making an ascending distance order
    for(d=0; d<4; d++)
    {
        for(i=0; i<4; i++)
        {
            if(d != i)
            {
                if(dists[d] > dists[i])
                {
                    t = dists[d];
                    dists[d] = dists[i];
                    dists[i] = t;

                    coord temp = dir[d];
                    dir[d] = dir[i];
                    dir[i] = temp;
                }
            }
        }
    }
}


///Test if the ghost is inside or outside of the map limits
//If outside, returns 1
int testGhostLimits(ghost *gh)
{
    if (gh->pos.x < 0)
    {
        gh->pos.x = WIDTH-1;
    }
    else if(gh->pos.x > (WIDTH-1))
    {
        gh->pos.x=0;
    }
    else if(gh->pos.y < 0)
    {
        gh->pos.y = HEIGHT-1;
    }
    else if(gh->pos.y > (HEIGHT-1))
    {
        gh->pos.y = 0;
    }
    else
    {
        return 0;
    }

    return 1;
}


///Checks if the actual ghost and the pacman are in the same spot
int checkGhostCollision(pacmanInfo pac, int *points, ghostsInfo *ghosts)
{
    int i;

    for(i=0; i < ghosts->quant; i++)
    {
        ghost gh = ghosts->unid[i];
        if(gh.alive==1 && (pac.pos.x-1 == gh.pos.x) && (pac.pos.y-1 == gh.pos.y)) //If ghost alive, and the same position as the pacman
        {
            if(pac.pacDotActive) //If pacman has powers, kills the ghost
            {
                ghosts->unid[i].alive = 0;
                ghosts->unid[i].deathTime = clock();
                (*points)+=200;

                textcolor(BRANCO);
                gotoXY(36, 32);
                printf("Points: %5d", *points); //Increases the points
            }
            return 1;
        }
    }
    return 0;
}


/// Revives the first found dead ghost
void reviveGhosts(ghostsInfo *ghostsInfo, int speed)
{

    int i;

    for(i=0; i < ghostsInfo->quant; i++)
    {
        if( !ghostsInfo->unid[i].alive && (( clock() - ghostsInfo->unid[i].deathTime) > RESPAWN) )
        {
            ghostsInfo->unid[i].pos = ghostsInfo->unid[i].origin;
            ghostsInfo->unid[i].reviveTime=4000000/speed/CLOCKS_PER_SEC; //4 seconds to revive
            ghostsInfo->unid[i].alive = 2; //Revive status
            return;
        }
    }
    return;
}


///Returns how many sides are free to the ghost walk
int freeSides(ghost gh, char lab[30][100])
{
    int sum = 0;

    //Test each of the possible directions
    if( lab[gh.pos.y][ gh.pos.x + 1] != '#') sum++;
    if( lab[gh.pos.y][ gh.pos.x - 1] != '#') sum++;
    if( lab[ gh.pos.y + 1][gh.pos.x] != '#') sum++;
    if( lab[ gh.pos.y - 1][gh.pos.x] != '#') sum++;

    return sum;
}


///Randomize the directions array
void shuffleDir()
{
    coord t; //Auxiliar
    int i, j=0;
    for(i=0; i<4; i++)
    {
        j = rand() % 4;
        t = dir[j];
        dir[j] = dir[i];
        dir[i] = t;
    }
}
