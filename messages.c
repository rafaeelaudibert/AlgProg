//Headers includes
#include "main.h"
#include "structs.h"
#include "messages.h"
#include "auxiliars.h"


///Starting game menu
void startGameMenu(void)
{
    int contador=0;
    char ch;
    FILE *arq;

    arq = fopen("data/pacmanStart.txt", "r");

    gotoXY(26, contador+5);
    Sleep(50);
    while( (ch=fgetc(arq))!=EOF)   //Messsage printing
    {
        if(contador>20)
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
            gotoXY(26, contador+5);
            Sleep((contador-25)*-6); //Each passing line shows faster
        }
    }

    getch();
    system("cls");
    return;
}


///Starting game message
int startMessage(int flag, pacmanInfo pacman, char lab[HEIGHT][WIDTH])
{

    if (flag)
    {
        switch(pacman.lives)
        {
        case 2:
            textcolor(BRANCO);
            gotoXY(38,13);
            printf("/------------------------\\");
            gotoXY(38,14);
            printf("|                        |");
            gotoXY(38,15);
            printf("| Press any key to start |");
            gotoXY(38,16);
            printf("|                        |");
            gotoXY(38,17);
            printf("\\------------------------/");
            break;
        case 1:
            textcolor(BRANCO);
            gotoXY(38,13);
            printf("/--------------------------\\");
            gotoXY(38,14);
            printf("|                          |");
            gotoXY(38,15);
            printf("| You've died! Be careful! |");
            gotoXY(38,16);
            printf("| Press any key to restart |");
            gotoXY(38,17);
            printf("|                          |");
            gotoXY(38,18);
            printf("\\--------------------------/");
            break;
        case 0:
            textcolor(BRANCO);
            gotoXY(38,13);
            printf("/--------------------------\\");
            gotoXY(38,14);
            printf("|                          |");
            gotoXY(38,15);
            printf("| This is your LAST chance |");
            gotoXY(38,16);
            printf("| Press any key to restart |");
            gotoXY(38,17);
            printf("|                          |");
            gotoXY(38,18);
            printf("\\--------------------------/");
        }
    }
    else
    {
        reconstructMaze(12,19,37,68, lab, pacman);
        flag--;
    }

    return flag;
}


///HIGHSCORES
void highscores(int points, clock_t duration)
{
    SCORE Highscores[11];
    ACTUAL_GAME data= {{'a','\0'}, -50};
    FILE *arq;
    char url[25]= {"data/Highscores.txt"};

    // Opens the highscores file for reading
    arq = fopen(url, "rt");
    readHighscore(arq, Highscores);
    fclose(arq);

    system("cls");
    textcolor(BRANCO);
    gotoXY(1,1);
    if(points>Highscores[9].points) //Verifies if this match will be in the highscore
    {
        fflush(stdin);
        gotoXY(32,17);
        printf("CONGRATULATIONS, YOU ARE IN THE TOP10!\n");
        gotoXY(38,18);
        printf("You have made %d points!\n", points);
        gotoXY(35,19);
        printf("Please, insert your first name: ");
        printTop10();

        cursorType(2); //Makes cursor to appear
        textcolor(VERMELHO);
        gotoXY(47,20);
        gets(data.name); //Reads the name
        data.name[strlen(data.name)]='\n';
        data.name[strlen(data.name)+1]='\0';
        data.points=points;
        cursorType(CURSOR);
    }

    //Manipulation and printing of the game data
    dataManipulation(Highscores, data, duration);

    //Writing the already changed file
    arq = fopen(url, "wt");
    writeHighscore(arq, Highscores);
    fclose(arq);

    colorHeader(); //Makes the header colorful, and switching colors

    return;

}

///Reads the highscore file stored in the PC
void readHighscore(FILE* arq, SCORE Highscores[10])
{

    int i;
    char line[100];

    if (arq == NULL)  // If opening error
    {
        printf("Opening file error\n");
        return;
    }
    else //Reads file
    {
        for(i=0; i<10; i++)
        {
            // Reads a line ('\n' included)
            fgets(line, 10, arq);  // 'fgets' reads until 14 characters or a '\n'
            Highscores[i].position=atoi(line);

            fgets(line, 31, arq);  // 'fgets' reads until 30 characters or a '\n'
            strcpy(Highscores[i].name,line);

            fgets(line, 10, arq);
            Highscores[i].points=atoi(line);

            fgets(line, 10, arq);
            Highscores[i].duration=atoi(line)*CLOCKS_PER_SEC;

            fgets(line, 16, arq);
            strcpy(Highscores[i].dateStr,line);

            fgets(line, 16, arq);
            strcpy(Highscores[i].timeStr,line);

        }
    }
}


///Reads the new Highscore file, with the match data included in it
void writeHighscore(FILE* arq, SCORE Highscores[10])
{

    int i;

    if (arq == NULL) // If error on opening
    {
        printf("CREATION file problems\n");
        return;
    }
    else //Write the new data
    {
        for(i=0; i<10; i++)
        {

            fprintf(arq,"%d\n",Highscores[i].position);
            fprintf(arq,"%s",Highscores[i].name);
            fprintf(arq,"%d\n",Highscores[i].points);
            fprintf(arq,"%ld\n",Highscores[i].duration/CLOCKS_PER_SEC);
            fprintf(arq,"%s",Highscores[i].dateStr);
            fprintf(arq,"%s",Highscores[i].timeStr);

        }
    }

    return;
}


///Manipulates the data taken from the Highscores with the actual game data
void dataManipulation(SCORE Highscores[10], ACTUAL_GAME data, clock_t duration)
{

    int i,j, flagRankingPosition=1, flagColor=0;
    char dateStrTemp[9];

    cursorType(CURSOR);
    system("cls"); //Clear screen

    //Starting message
    textcolor(BRANCO);
    gotoXY(1,8);
    printf("      POSITION  NAME                           POINTS        TIME        DATE            HOUR");

    //Manipulação dos dados
    for(i=0; i<10; i++)
    {
        if(Highscores[i].points==data.points && Highscores[i].duration>duration && flagRankingPosition) //If same points, but a lower time
        {
            for(j=9; j>=i; j--) //Makes all the following position, go down one position
            {
                Highscores[j].points=Highscores[j-1].points;
                Highscores[j].duration=Highscores[j-1].duration;
                strcpy(Highscores[j].name, Highscores[j-1].name);
                strcpy(Highscores[j].dateStr, Highscores[j-1].dateStr);
                strcpy(Highscores[j].timeStr, Highscores[j-1].timeStr);
            }

            //Sets the actual position
            Highscores[i].points=data.points;
            strcpy(Highscores[i].name, data.name);
            Highscores[i].duration=duration;

            _strdate(Highscores[i].dateStr); //MMDDAA shape data

            strcpy(dateStrTemp, Highscores[i].dateStr); //Transformation to DDMMAA
            Highscores[i].dateStr[0]=dateStrTemp[3];
            Highscores[i].dateStr[1]=dateStrTemp[4];
            Highscores[i].dateStr[3]=dateStrTemp[0];
            Highscores[i].dateStr[4]=dateStrTemp[1];
            Highscores[i].dateStr[8]='\n';
            Highscores[i].dateStr[9]='\0';

            _strtime(Highscores[i].timeStr); //Time in the HHMMSS shape
            Highscores[i].timeStr[8]='\n';
            Highscores[i].timeStr[9]='\0';
            flagRankingPosition=0;
            flagColor=1;
        }
        else if(Highscores[i].points<data.points && flagRankingPosition) //If has more points than the actual position
        {
            for(j=9; j>=i; j--) //Makes all the following position, go down one position
            {
                Highscores[j].points=Highscores[j-1].points;
                Highscores[j].duration=Highscores[j-1].duration;
                strcpy(Highscores[j].name, Highscores[j-1].name);
                strcpy(Highscores[j].dateStr, Highscores[j-1].dateStr);
                strcpy(Highscores[j].timeStr, Highscores[j-1].timeStr);
            }

            //Sets the actual position
            Highscores[i].points=data.points;
            strcpy(Highscores[i].name, data.name);
            Highscores[i].duration=duration;

            _strdate(Highscores[i].dateStr); //MMDDAA shape data

            strcpy(dateStrTemp, Highscores[i].dateStr); //Transformation to DDMMAA
            Highscores[i].dateStr[0]=dateStrTemp[3];
            Highscores[i].dateStr[1]=dateStrTemp[4];
            Highscores[i].dateStr[3]=dateStrTemp[0];
            Highscores[i].dateStr[4]=dateStrTemp[1];
            Highscores[i].dateStr[8]='\n';
            Highscores[i].dateStr[9]='\0';

            _strtime(Highscores[i].timeStr); //Time in the HHMMSS shape
            Highscores[i].timeStr[8]='\n';
            Highscores[i].timeStr[9]='\0';
            flagRankingPosition=0;
            flagColor=1;
        }

        //Sets the position color
        if(flagColor)
        {
            textcolor(LILAS);
        }
        else
        {
            textcolor(BRANCO);
        }

        //Prints the position
        gotoXY(10, i+10);
        printf("%d",Highscores[i].position);
        gotoXY(17,i+10);
        printf("%s", Highscores[i].name);
        gotoXY(48,i+10);
        printf("%5d", Highscores[i].points);
        gotoXY(60,i+10);
        printf("%5ld",Highscores[i].duration/CLOCKS_PER_SEC);
        gotoXY(72,i+10);
        printf("%s", Highscores[i].dateStr);
        gotoXY(88,i+10);
        printf("%s", Highscores[i].timeStr);
        flagColor=0;
    }

    textcolor(BRANCO);
    gotoXY(32,20);
    printf("______________________________________");
    gotoXY(32,21);
    printf("Press any key to close the game window");

    return;
}


///Prints the TOP10 "image"
void printTop10(void)
{
    int i=3;
    char ch;
    FILE *arq;

    arq = fopen("data/top10.txt", "r");

    gotoXY(4, i);
    i++;
    while( (ch=fgetc(arq))!=EOF)   //TOP10 printing
    {
        textcolor(BRANCO);
        printf("%c", ch);

        if(ch=='\n')
        {
            gotoXY(4,i);
            i++;
        }
    }

    return;

}


///Makes theHighscore header to blink
void colorHeader(void)
{

    int k=AZUL, flag=1;

    while(flag)
    {
        textcolor(k);
        gotoXY(1,2);
        printf("   ##############################################################################################\n");
        printf("  ##########################################               #######################################\n");
        printf(" ##########################################                 #######################################\n");
        printf("  ##########################################               #######################################\n");
        printf("   ##############################################################################################\n");
        k++;

        textcolor(BRANCO);
        gotoXY(48,4);
        printf("HIGHSCORE");

        if(k>AMARELO)
        {
            k=AZUL;
        }
        if(kbhit())
        {
            flag=0;
        }

        Sleep(150);
    }
}
