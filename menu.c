//Headers includes
#include "main.h"
#include "structs.h"
#include "auxiliars.h"
#include "menu.h"

//Main Menu
int menu(int *difficulty, int *speed, int *map, char *up, char *down, char *right, char *left, char *stop)
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
            gotoXY(46,15);
            textcolor(color);
            printf(">    PLAY");
            break;
        case 2:
            gotoXY(46,19);
            textcolor(color);
            printf(">   OPTIONS");
            break;
        case 3:
            gotoXY(46,21);
            textcolor(color);
            printf(">   CREDITS");
            break;
        case 4:
            gotoXY(46,23);
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
                options(difficulty, speed, map, up, down, right, left, stop); //Menu Options
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

//Options Menu
void options(int *difficulty, int *speed, int *map, char *up, char *down, char *right, char *left, char *stop)
{

    //Default values of the game
    int dfDifficulty=6, dfSpeed=115, dfMap=1;
    char dfUp='W', dfDown='X', dfRight='D', dfLeft='A', dfStop='S';

    int menuFlag=1, opcaoMenu=1; //Flags
    int colorCounter=3500, color=VERMELHO; //Colors
    char key; //Pressed key

    system("cls");
    printsOptions(difficulty, speed, map, up, down, right, left, stop);

    //Loop of iterations of the menu
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
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                break;
            case 'x':
                opcaoMenu++;
                colorCounter=3500;
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                break;
            }

            if (opcaoMenu>10) //Limits of the menu
            {
                opcaoMenu=1;
            }
            else if(opcaoMenu<1)
            {
                opcaoMenu=10;
            }
        }

        if(colorCounter%7000>3500)  //Makes color "blinks"
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
            gotoXY(37,6);
            textcolor(color);
            printf("> DIFFICULTY:              %3d\n", *difficulty);
            break;
        case 2:
            gotoXY(37,7);
            textcolor(color);
            printf("> SPEED:                   %3d\n", *speed);
            break;
        case 3:
            gotoXY(37,8);
            textcolor(color);
            printf("> MAP:                     %3d\n", *map);
            break;
        case 4:
            gotoXY(37,17);
            textcolor(color);
            printf("> UP:                      %3c\n", *up);
            break;
        case 5:
            gotoXY(37,18);
            textcolor(color);
            printf("> DOWN:                    %3c\n", *down);
            break;
        case 6:
            gotoXY(37,19);
            textcolor(color);
            printf("> LEFT:                    %3c\n", *left);
            break;
        case 7:
            gotoXY(37,20);
            textcolor(color);
            printf("> RIGHT:                   %3c\n", *right);
            break;
        case 8:
            gotoXY(37,21);
            textcolor(color);
            printf("> STOP:                    %3c\n", *stop);
            break;
        case 9:
            gotoXY(37,25);
            textcolor(color);
            printf("> SAVE");
            break;
        case 10:
            gotoXY(58,25);
            textcolor(color);
            printf("> DEFAULT");
            break;

        }

        if(key=='d') //Changes the values
        {
            cursorType(2);
            switch(opcaoMenu)
            {
            case 1: //SET DIFFICULTY
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    printsSelectOptions();

                    gotoXY(48,31);
                    printf("DIFFICULTY");
                    gotoXY(45,32);
                    printf("Between 0 and 10");
                    gotoXY(41,34);
                    printf("The lower the difficulty");
                    gotoXY(43,35);
                    printf("The easier the game");

                    if(*difficulty<0 || *difficulty>10) //Invalid value
                    {
                        gotoXY(41,34);
                        printf("                         ");
                        gotoXY(41,35);
                        printf("     Invalid value       ");
                    }

                    gotoXY(52,33);
                    scanf("%d", difficulty);
                }
                while(*difficulty<0 || *difficulty>10); //Compatibility check
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 2:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    printsSelectOptions();

                    gotoXY(50,31);
                    printf("SPEED");
                    gotoXY(44,32);
                    printf("Between 20 and 150");
                    gotoXY(43,34);
                    printf("The lower the speed");
                    gotoXY(43,35);
                    printf("The faster the game");

                    if(*speed<20 || *speed>150) //Invalid value
                    {
                        gotoXY(41,34);
                        printf("                         ");
                        gotoXY(41,35);
                        printf("     Invalid value       ");
                    }

                    gotoXY(52,33);
                    scanf("%d", speed);
                }
                while(*speed<20 || *speed>150); //Compatibility check
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 3:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    printsSelectOptions();

                    if(*map<1 || *map>3) //Invalid value
                    {
                        gotoXY(46,35);
                        printf("Invalid value");
                    }

                    gotoXY(51,31);
                    printf("MAP");
                    gotoXY(45,32);
                    printf("Between 1 and 3");
                    gotoXY(52,34);
                    scanf("%d", map);
                }
                while(*map<0 || *map>3); //Compatibility check
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 4:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    printsSelectOptions();

                    if(*up<65 || (*up>90 && *up<97) || *up>122) //Invalid value
                    {
                        gotoXY(46,34);
                        printf("Invalid value");
                    }

                    if(*up==*down || *up==*left || *up==*right || *up==*stop) //Shortcut already used
                    {
                        gotoXY(42,34);
                        printf("Shortcut already used");
                    }

                    gotoXY(52,31);
                    printf("UP");
                    gotoXY(44,32);
                    printf("Between 'A' and 'Z'");
                    gotoXY(52,33);
                    scanf("%c", up);
                    *up=toupper(*up);
                }
                while((*up<65 || (*up>90 && *up<97) || *up>122) || *up==*down || *up==*left || *up==*right || *up==*stop); //Compatibility check
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 5:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    printsSelectOptions();

                    if(*down<65 || (*down>90 && *down<97) || *down>122) //Invalid value
                    {
                        gotoXY(46,34);
                        printf("Invalid value");
                    }

                    if(*down==*up || *down==*left || *down==*right || *down==*stop) //Shortcut already used
                    {
                        gotoXY(42,34);
                        printf("Shortcut already used");
                    }

                    gotoXY(51,31);
                    printf("DOWN");
                    gotoXY(44,32);
                    printf("Between 'A' and 'Z'");
                    gotoXY(52,33);
                    scanf("%c", down);
                    *down=toupper(*down);
                }
                while(*down<65 || (*down>90 && *down<97) || *down>122 || *down==*up || *down==*left || *down==*right || *down==*stop); //Compatibility check
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 6:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    printsSelectOptions();

                    if(*left<65 || (*left>90 && *left<97) || *left>122) //Invalid value
                    {
                        gotoXY(46,34);
                        printf("Invalid value");
                    }

                    if(*left==*up || *left==*down || *left==*right || *left==*stop) //Shortcut already used
                    {
                        gotoXY(42,34);
                        printf("Shortcut already used");
                    }

                    gotoXY(51,31);
                    printf("LEFT");
                    gotoXY(44,32);
                    printf("Between 'A' and 'Z'");
                    gotoXY(52,33);
                    scanf("%c", left);
                    *left=toupper(*left);
                }
                while(*left<65 || (*left>90 && *left<97) || *left>122 || *left==*up || *left==*down || *left==*right || *left==*stop); //Compatibility check
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 7:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    printsSelectOptions();

                    if(*right<65 || (*right>90 && *right<97) || *right>122) //Invalid value
                    {
                        gotoXY(46,34);
                        printf("Invalid value");
                    }

                    if(*right==*up || *right==*down || *right==*left || *right==*stop) //Shortcut already used
                    {
                        gotoXY(42,34);
                        printf("Shortcut already used");
                    }

                    gotoXY(51,31);
                    printf("RIGHT");
                    gotoXY(44,32);
                    printf("Between 'A' and 'Z'");
                    gotoXY(52,33);
                    scanf("%c", right);
                    *right=toupper(*right);
                }
                while(*right<65 || (*right>90 && *right<97) || *right>122 || *right==*up || *right==*down || *right==*left || *right==*stop); //Compatibility check
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 8:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    printsSelectOptions();

                    if(*stop<65 || (*stop>90 && *stop<97) || *stop>122) //Invalid value
                    {
                        gotoXY(46,34);
                        printf("Invalid value");
                    }

                    if(*stop==*up || *stop==*down || *stop==*left || *stop==*right) //Shortcut already used
                    {
                        gotoXY(42,34);
                        printf("Shortcut already used");
                    }

                    gotoXY(51,31);
                    printf("STOP");
                    gotoXY(44,32);
                    printf("Between 'A' and 'Z'");
                    gotoXY(52,33);
                    scanf("%c", stop);
                    *stop=toupper(*stop);
                }
                while(*stop<65 || (*stop>90 && *stop<97) || *stop>122 || *stop==*up || *stop==*down || *stop==*left || *stop==*right); //Compatibility check
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 9:
                opcaoMenu++; //Goes to the default button
                colorCounter=3500;
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 10:
                opcaoMenu--; //Goes to the save button
                colorCounter=3500;
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;

            }
            cursorType(CURSOR);
        }
        else if(key=='a') //Used just on Save or Default button
        {
            switch(opcaoMenu)
            {
            case 9:
                opcaoMenu++; //Goes to default button
                colorCounter=3500;
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 10:
                opcaoMenu--; //Goes to save button
                colorCounter=3500;
                system("cls");
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            }
        }
        else if(key=='e') //If enter was pressed (works in Save and Default button)
        {
            switch(opcaoMenu)
            {
            case 9: //SAVE BUTTON
                menuFlag=0; //Saves and closes the option menu
                break;
            case 10: //DEFAULT
                //Sets  the variables to the default values
                *difficulty=dfDifficulty;
                *speed=dfSpeed;
                *map=dfMap;
                *up=dfUp;
                *down=dfDown;
                *left=dfLeft;
                *right=dfRight;
                *stop=dfStop;
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            }
        }

        colorCounter++; //Makes the color varies
    }

    writeSettings(*difficulty, *speed, *map, *up, *down, *right, *left, *stop);
    textcolor(BRANCO);
    gotoXY(43,29);
    printf("SAVED CONFIGURATIONS!");
    gotoXY(35,30);
    printf("____________________________________");
    gotoXY(35,31);
    printf("Press any key to go back to the menu");
    getch();
    system("cls");
    printsHeader();
    return;

}

//Credits Menu
void creditos(void)
{

    int i, scrollCounter, counter;
    char text[13][45];
    FILE* arq;

    system("cls");
    arq=fopen("data/creditos.txt", "r");
    if(!arq)
    {
        printf("Erro na abertura do arquivo");
    }
    else
    {
        for(i=0; i<13; i++)
        {
            fgets(text[i], 45, arq); //Reads the credits file lines
        }
    }
    fclose(arq);

    textcolor(BRANCO);
    for(scrollCounter=0; scrollCounter<51; scrollCounter++) //Prints the credits
    {

        for(counter=0, i=11; counter<=12; counter++, i--){ //Prints each line, scrolling it through the screen
            if(scrollCounter>counter && scrollCounter<counter+38){
                gotoXY(30,scrollCounter-counter);
                printf("%s", text[i]);
            }
        }

        if(scrollCounter>12 && scrollCounter<51)  //Cleans the upper line
        {
            gotoXY(30,scrollCounter-12);
            printf("                                                     ");
        }

        scrollCounter==24 ? Sleep(5000) : Sleep(250); //Pauses the game for a longer time when needed

    }

    system("cls");
    printsHeader();
    return;

}

//Prints the header of the Main Menu
void printsHeader(void)
{

    textcolor(BRANCO);
    gotoXY(1,10);
    printf("                                                 PACMAN\n\n\n");
    printf("                                    ################################\n\n");
    printf("                                                  PLAY               \n\n\n\n");
    printf("                                                 OPTIONS              \n\n");
    printf("                                                 CREDITS              \n\n");
    printf("                                                  EXIT               \n\n");
    printf("                                    ################################\n\n");
    return;
}

//Prints the Options Menu
void printsOptions(int *difficulty, int *speed, int *map, char *up, char *down, char *right, char *left, char *stop)
{

    textcolor(BRANCO);
    gotoXY(1,1);
    printf("\n                                    ################################\n");
    printf("                                               MAP OPTIONS\n");
    printf("                                    ################################\n\n");
    printf("                                      DIFFICULTY:              %3d\n", *difficulty);
    printf("                                      SPEED:                   %3d\n", *speed);
    printf("                                      MAP:                     %3d\n\n", *map);
    printf("                                    ################################\n\n");
    printf("\n");
    printf("                                    ################################\n");
    printf("                                                KEY CODES\n");
    printf("                                    ################################\n\n");
    printf("                                      UP:                      %3c\n", *up);
    printf("                                      DOWN:                    %3c\n", *down);
    printf("                                      LEFT:                    %3c\n", *left);
    printf("                                      RIGHT:                   %3c\n", *right);
    printf("                                      STOP:                    %3c\n\n", *stop);
    printf("                                    ################################\n\n");
    printf("                                      SAVE                 DEFAULT");


    return;
}

//Prints the canvas of the Option Selection
void printsSelectOptions(void)
{

    gotoXY(37,29);
    printf("################################");
    gotoXY(37,30);
    printf("#                              #");
    gotoXY(37,31);
    printf("#                              #");
    gotoXY(37,32);
    printf("#                              #");
    gotoXY(37,33);
    printf("#                              #");
    gotoXY(37,34);
    printf("#                              #");
    gotoXY(37,35);
    printf("#                              #");
    gotoXY(37,36);
    printf("#                              #");
    gotoXY(37,37);
    printf("################################");

    return;

}

//Detects the pressed key
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
    else if (GetAsyncKeyState(0x44)) //Tecla 'D'
    {
        key = 'd';
    }
    else if (GetAsyncKeyState(0x41)) //Tecla 'A'
    {
        key = 'a';
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
