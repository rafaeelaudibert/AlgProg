//Headers includes
#include "main.h"
#include "structs.h"
#include "ghosts.h"
#include "auxiliars.h"
#include "labirinto.h"



///Reads the maze archive
int readLab(char lab[HEIGHT][WIDTH], int labOption)
{
    char str[101];
    int i=0, j=0;
    FILE *arq;
    char url[30];

    switch(labOption)
    {
    case 1:
        strcpy(url, "data/labirinto.txt");
        break;
    case 2:
        strcpy(url, "data/labirintoOneDot.txt");
        break;
    case 3:
        strcpy(url, "data/labirintoProfessor.txt");
        break;
    case 4:
        strcpy(url, "data/labirintoNoLab.txt");
        break;
    default:
        strcpy(url, "nolab.txt");
        break;
    }

    //Pointer to the file read
    arq = fopen(url, "r");

    //Tests the file opening
    if(arq == NULL)
    {
        printf("ERROR: The lab couldn't be opened\n");
        fclose(arq);
        return 1;
    }


    //Reads each of the file's line
    while(fgets(str, 101, arq) != NULL || i<30)
    {
        //If the line it isn't a blank line
        if(str[0] != '\n')
        {
            //Add's to the lab the string
            for(j=0; j<100; j++)
            {
                lab[i][j] = str[j];
            }

            i++;
        }
    }
    fclose(arq);

    return 0;
}


///Starts and load all the structs
int startLab(char lab[HEIGHT][WIDTH], int *qtPacDots, pacmanInfo *pacman, ghostsInfo *fantasmas, int labOption)
{
    int i, j; //Matrix counter

    //Variables inicializations
    *qtPacDots=0;
    fantasmas->quant=0;


    if(readLab(lab, labOption)) //Reads the lab
    {
        return 1;
    }

    //Prints the lab
    gotoXY(1,1);
    for(i=0; i<HEIGHT; i++)
    {
        for(j=0; j<WIDTH; j++)
        {
            switch(lab[i][j])
            {
            case 'c':
            case 'C':
                //Pacman's origin (x,y)
                pacman->origin.x = j+1;
                pacman->origin.y = i+1;

                //Pacman's actual (x,y)
                pacman->pos.x = j+1;
                pacman->pos.y = i+1;

                lab[i][j] = ' ';
                break;
            case 'w':
            case 'W': // Ghosts
                if(fantasmas->quant < MAX_GHOSTS)
                {
                    //Ghost's origin (x,y)
                    fantasmas->unid[fantasmas->quant].origin.x=j;
                    fantasmas->unid[fantasmas->quant].origin.y=i;

                    //Ghost's actual (x,y)
                    fantasmas->unid[fantasmas->quant].pos.x = j;
                    fantasmas->unid[fantasmas->quant].pos.y = i;
                    fantasmas->unid[fantasmas->quant].alive = 1; // Ghost live
                    fantasmas->unid[fantasmas->quant].key = 'W';
                    (fantasmas->quant)++;
                }
                lab[i][j] = ' ';
                break;
            case 'o': //Pastils
                (*qtPacDots)++;
                break;
            }
        }
    }

    return 0;

}


///Prints the lab in the screen
int showLab(char lab[HEIGHT][WIDTH], pacmanInfo *pacman, ghostsInfo *fantasmas)
{

    int i, j;
    int q_fantasmas=fantasmas->quant;

    //Lab's Print/reprint
    gotoXY(1,1);
    for(i=0; i<HEIGHT; i++)
    {
        for(j=0; j<WIDTH; j++)
        {
            switch(lab[i][j])
            {
            case '#':
                textcolor(CINZA_CLARO);
                printf("%c", PAREDE);
                break;
            case 'o':
                textcolor(AMARELO);
                printf("%c", PACDOT);
                break;
            case '*':
                textcolor(VERMELHO);
                printf("%c", POWERPELLET);
                break;
            default:
                printf(" ");
                break;
            }
        }
        //printf("\n"); //Must uncomment in some PC's
    }

    //Sets the position of the pacman
    pacman->pos.x=pacman->origin.x;
    pacman->pos.y=pacman->origin.y;

    //Sets the initial position of the ghost
    for(i=0; i<q_fantasmas; i++)
    {
        fantasmas->unid[i].pos.x = fantasmas->unid[i].origin.x;
        fantasmas->unid[i].pos.y = fantasmas->unid[i].origin.y;
        fantasmas->unid[i].alive = 1; // Sets ghosts lives
    }
    return 0;
}
