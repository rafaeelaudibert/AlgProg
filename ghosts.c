//Headers includes
#include "main.h"
#include "structs.h"
#include "ghosts.h"
#include "objects.h"
#include "auxiliars.h"

//Variaveis globais

 //Available directions -> UP, RIGHT, DOWN, LEFT
coord dir[4]={{0,-1},{1,0},{0,1},{-1,0}};


///Ghost Controller
int ghostsControl(int *points, pacmanInfo pacman, clock_t *ghostsTimer, char lab[HEIGHT][WIDTH], ghosts *fantasmas, int speed, int chase_chance)
{

    float speedCorrection = pacman.last.coordinates=='y' ? 1.4 : 1; //Corrects the speed of the game, as the pacman's movement axis changes
    clock_t actualTime=clock();

    if((actualTime - *ghostsTimer) > ( pacman.pacDotActive ? speed*1.3*speedCorrection : speed*speedCorrection))
    {
        *ghostsTimer = actualTime;
        moveGhost(pacman, lab, fantasmas, chase_chance); // Updates the movement of the ghosts
        showGhosts(pacman, lab, fantasmas, speed); // Updates and shows the position of the ghosts

        if(checkGhostCollision(pacman, points, fantasmas) && !pacman.pacDotActive) //If there is collision between a ghost and the pacman
        {
            return 0;
        }
    }
    return 1;
}


///Moves each one of the ghosts
void moveGhost(pacmanInfo pac, char lab[30][100], ghosts *fantasmas, int chase_chance)
{

    int i, q = fantasmas->quant; //fantasmas.quant;
    // Iterates on each ghost
    for(i=0; i<q; i++)
    {
        // Actual test ghost
        ghost g = fantasmas->unid[i];

        // Ghost's chance of stalking the Pacman
        int chance = rand() % MAX_RANDOM;

        //Decisions to change direction
        if(freeSides(g, lab) == 1) //Only one exit, inverts the movement direction
        {
            fantasmas->unid[i].mov.y = fantasmas->unid[i].mov.y * -1;
            fantasmas->unid[i].mov.x = fantasmas->unid[i].mov.x * -1;
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
            chooseDirection(pac, &(fantasmas->unid[i]), lab);
        }
    }
}


///Prints the ghosts in the screen
void showGhosts(pacmanInfo pac, char lab[30][100], ghosts *fantasmas, int speed)
{
    int i;
    for(i=0; i < fantasmas->quant; i++)
    {
        //Reprints the object who was under the ghost
        coord posg;
        posg.x = fantasmas->unid[i].pos.x;
        posg.y = fantasmas->unid[i].pos.y;
        gotoXY(posg.x+1, posg.y+1);
        //Print colors
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
        if(fantasmas->unid[i].alive==1)
        {
            fantasmas->unid[i].pos.x += fantasmas->unid[i].mov.x;
            fantasmas->unid[i].pos.y += fantasmas->unid[i].mov.y;
        }

        testGhostLimits(&fantasmas->unid[i]);

        if(fantasmas->unid[i].alive==1)
        {
            //Prints the new position, if the ghost is alive
            if(pac.pacDotActive>(2000/speed))
            {
                textcolor(VERDE_AGUA);
            }
            else
            {
                if(pac.pacDotActive%2)
                {
                    textcolor(VERDE_AGUA);
                }
                else
                {
                    textcolor(LILAS);
                }
            }
            gotoXY(fantasmas->unid[i].pos.x+1, fantasmas->unid[i].pos.y+1);
            printf("%c", fantasmas->unid[i].key);
        }
        else if(fantasmas->unid[i].alive==2) //If ghost is ressurecting
        {
            if (!(fantasmas->unid[i].reviveTime%2))
            {
                textcolor(PRETO); //Blinks the color of the ghost, while ressurecting
            }
            else
            {
                textcolor(LILAS);
            }
            fantasmas->unid[i].reviveTime--;

            if(!(fantasmas->unid[i].reviveTime))
            {
                fantasmas->unid[i].alive=1;
            }

            gotoXY(fantasmas->unid[i].pos.x+1, fantasmas->unid[i].pos.y+1);
            printf("%c", fantasmas->unid[i].key);
        }
    }
}


///Verifies if the ghost are in a bifurcation or in a corner
//Returns:  1: If needs to decide to change or not direction
//          0: Doesn't need to decide to change direction
int changeDirection(ghost g, char lab[30][100])
{
    // Verifies which direction is the ghost going
    if( g.mov.x != 0)
    {
        // If horizontal, verifies if can go up or down
        if( lab[g.pos.y+1][g.pos.x] != '#' || lab[g.pos.y-1][g.pos.x] != '#') return 1;
    }
    else
    {
        // If vertical, verifies if can go right or left
        if( lab[g.pos.y][g.pos.x+1] != '#' || lab[g.pos.y][g.pos.x-1] != '#') return 1;
    }
    return 0;
}


///Verifies which are the possible directions for the ghost, and chooses one according to the preference order of the array dir[]
void chooseDirection(pacmanInfo pac, ghost *pg, char lab[30][100])
{
    int i, d;
    ghost g = *pg;

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
                pg->mov = dir[d];
                d = 5;
            }
        }
    }
}


///Returns an ordered ascending array, according to the cartesian distance between the ghost and the pacman
void chasePacman(pacmanInfo pac, ghost g, char lab[30][100])
{
    int d, i, t;

    //Distance array in each of the possible directions
    int dists[4];
    for(i=0; i<4; i++)
    {
        // Cartesian Distance (Pithagoras)
        dists[i] = pow( pac.pos.y - g.pos.y + dir[i].y, 2) + pow(pac.pos.x - g.pos.x + dir[i].x, 2);
    }

    //Sorts the array according to its distances
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
int testGhostLimits(ghost *fantasma)
{
    if (fantasma->pos.x < 0)
    {
        fantasma->pos.x = WIDTH-1;
    }
    else if(fantasma->pos.x > (WIDTH-1))
    {
        fantasma->pos.x=0;
    }
    else if(fantasma->pos.y < 0)
    {
        fantasma->pos.y = HEIGHT-1;
    }
    else if(fantasma->pos.y > (HEIGHT-1))
    {
        fantasma->pos.y = 0;
    }
    else
    {
        return 0;
    }

    return 1;
}


///Checks if the actual ghost and the pacman are in the same spot
int checkGhostCollision(pacmanInfo pac, int *points, ghosts *fantasmas)
{
    int i;

    for(i=0; i<fantasmas->quant; i++)
    {
        ghost g = fantasmas->unid[i];
        if(g.alive==1 && pac.pos.x-1 == g.pos.x && pac.pos.y-1 == g.pos.y) //If ghost alive, and the same position as the pacman
        {
            if(pac.pacDotActive) //If pacman has powers, kills the ghost
            {
                fantasmas->unid[i].alive = 0;
                fantasmas->unid[i].deathTime = clock();
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
void reviveGhosts(ghosts *ghosts, int speed)
{

    int i;

    for(i=0; i < ghosts->quant; i++)
    {
        if( !ghosts->unid[i].alive && (( clock() - ghosts->unid[i].deathTime) > RESPAWN) )
        {
            ghosts->unid[i].pos = ghosts->unid[i].origin;
            ghosts->unid[i].reviveTime=4000/speed; //4 seconds blinking
            ghosts->unid[i].alive = 2;
            return;
        }
    }
    return;
}


///Returns how many sides are free to the ghost walk
int freeSides(ghost g, char lab[30][100])
{
    int sum = 0;

    //Test each of the possible directions
    if( lab[g.pos.y][ g.pos.x + 1] != '#') sum++;
    if( lab[g.pos.y][ g.pos.x - 1] != '#') sum++;
    if( lab[ g.pos.y + 1][g.pos.x] != '#') sum++;
    if( lab[ g.pos.y - 1][g.pos.x] != '#') sum++;
    return sum;
}


///Unsorts the array of directions, randomically
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
