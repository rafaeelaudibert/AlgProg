//Headers includes
#include "main.h"
#include "structs.h"
#include "auxiliars.h"
#include "menu.h"


int menu(void)
{

    int menuFlag=1, opcaoMenu=1, endGame=3; //Flags
    int colorCounter=3500, color=VERMELHO; //Colors
    char key; //Key pressed

    printsHeader(); //Prints the menu message

    while(menuFlag)
    {
        if(kbhit()) //When a key is stroked
        {
            key=tolower(detectKeyMenu()); //Detects pressed key
            getch(); //Just one 'stroke' of the key each time

            switch(key) //Verifies if we should go up or down in the options
            {
            case 'w':
                opcaoMenu--;
                colorCounter=3500; //The next options starts red
                printsHeader();
                break;
            case 'x':
                opcaoMenu++;
                colorCounter=3500;
                printsHeader();
                break;
            }

            if (opcaoMenu>4) //Limits
            {
                opcaoMenu=1;
            }
            else if(opcaoMenu<1)
            {
                opcaoMenu=4;
            }
        }

        if(colorCounter%7000>3500)  //Makes color "blink"
        {
            color=VERMELHO;
        }
        else
        {
            color=BRANCO;
        }

        switch(opcaoMenu) //The actual selection
        {
        case 1:
            gotoXY(46,13);
            textcolor(color);
            printf(">    PLAY");
            break;
        case 2:
            gotoXY(46,17);
            textcolor(color);
            printf(">   OPTIONS");
            break;
        case 3:
            gotoXY(46,19);
            textcolor(color);
            printf(">   CREDITS");
            break;
        case 4:
            gotoXY(46,21);
            textcolor(color);
            printf(">    EXIT");
            break;

        }

        if(key=='e') //If 'enter' pressed, verifies what function must be called
        {
            switch(opcaoMenu)
            {
            case 1:
                menuFlag=0; //Closes the menu and enters in the game
                break;
            case 2:
                opcoes(); //Menu Options
                break;
            case 3:
                creditos(); //Menu Credits
                break;
            case 4:
                menuFlag=0; //Closes the menu
                endGame=-2; //Ends the game
                break;
            }

            colorCounter=3500;
            key='m';
            opcaoMenu=1;
        }

        colorCounter++;
    }

    return endGame;

}

char detectKeyMenu(void)
{
    char key;

    if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) //Tecla para cima ou tecla 'W'
    {
        key = 'w';
    }
    else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x58)) //Tecla para baixo ou tecla 'X'
    {
        key = 'x';
    }
    else if(GetAsyncKeyState(VK_RETURN)) //Tecla Enter
    {
        key='e';
    }
    else //Senão retorna uma letra que não significa nada
    {
        key='z';
    }

    return key;

}

void opcoes(void)
{

    int difficulty=10, speed=115, map=1;
    char up='w', down='x', right='d', left='a', stop='s';

    cursorType(2);
    textcolor(BRANCO);
    system("cls");
    do
    {
        printf("Difficulty (Range: 0-10  -+---+-  Default: 6): "); //Difficulty configuration
        scanf("%d", &difficulty);

        if(difficulty<0 || difficulty>10)  //Checking compatibility
        {
            printf("Invalid difficulty\n");
        }

    }
    while(difficulty<0 || difficulty>10);

    do
    {
        printf("Speed (Range: 20-150  -+---+-  Default: 115): "); //Speed configuration
        scanf("%d", &speed);

        if(speed<20 || speed>150) //Checking compatibility
        {
            printf("Invalid speed\n");
        }

    }
    while(speed<20 || speed>150);

    do
    {
        printf("Map Selector (Range: 1-%d  -+---+-  Default: 1): ", QT_MAP); //Map configuration
        scanf("%d", &map);

        if(map<1 || map>QT_MAP)  //Checking compatibility
        {
            printf("Invalid map\n");
        }

    }
    while(map<1 || map>QT_MAP);

    printf("\nUp key (Default: 'W'): "); //Up key configuration
    scanf(" %c", &up);
    up=tolower(up);

    printf("Left key (Default: 'A'): "); //Left key configuration
    scanf(" %c", &left);
    left=tolower(left);

    printf("Down key (Default: 'X'): "); //Down key configuration
    scanf(" %c", &down);
    down=tolower(down);

    printf("Right key (Default: 'D'): "); //Right key configuration
    scanf(" %c", &right);
    right=tolower(right);

    printf("Stop key (Default: 'S'): "); //Stop key configuration
    scanf(" %c", &stop);
    stop=tolower(stop);


    cursorType(CURSOR);
    printf("\n\nSAVED CONFIGURATIONS!");
    printf("\n____________________________________");
    printf("\nPress any key to go back to the menu");
    getch();
    system("cls");
    printsHeader();
    return;

}

void creditos(void)
{

    system("cls");
    printsHeader();
    return;

}

void printsHeader(void)
{

    textcolor(BRANCO);
    gotoXY(1,11);
    printf("                                    ################################\n\n");
    printf("                                                  PLAY               \n\n\n\n");
    printf("                                                 OPTIONS              \n\n");
    printf("                                                 CREDITS              \n\n");
    printf("                                                  EXIT               \n\n");
    printf("                                    ################################\n\n");
    return;
}
