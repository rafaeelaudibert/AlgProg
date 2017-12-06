//Headers includes
#include "main.h"
#include "structs.h"
#include "auxiliars.h"
#include "messages.h"
#include "menu.h"

///Main Menu
int menu(int *difficulty, int *speed, int *map, char *up, char *down, char *right, char *left, char *stop)
{

    int menuFlag=1, menuOption=1, endGame=3; //Flags
    int colorCounter=3500, color=VERMELHO; //Colors
    char key; //Key pressed

    fflush(stdin);
    printsHeader(); //Prints the menu message

    while(menuFlag)
    {
        if(kbhit()) //When a key is stroked
        {
            key=tolower(detectKeyMenu()); //Detects pressed key

            switch(key) //Verifies if we should go up or down in the options
            {
            case 'w':
                menuOption--;
                colorCounter=3500; //The next options starts red
                printsHeader();
                break;
            case 'x':
                menuOption++;
                colorCounter=3500;
                printsHeader();
                break;
            }

            if (menuOption>5) //Limits
            {
                menuOption=1;
            }
            else if(menuOption<1)
            {
                menuOption=5;
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

        switch(menuOption) //The actual selection
        {
        case 1:
            gotoXY(45,15);
            textcolor(color);
            printf(">>    PLAY");
            break;
        case 2:
            gotoXY(45,19);
            textcolor(color);
            printf(">>   OPTIONS");
            break;
        case 3:
            gotoXY(45,21);
            textcolor(color);
            printf(">>   CREDITS");
            break;
        case 4:
            gotoXY(45,23);
            textcolor(color);
            printf(">>  HIGHSCORES");
            break;
        case 5:
            gotoXY(45,27);
            textcolor(color);
            printf(">>    EXIT");
            break;

        }

        if(key=='d' || key=='e') //If 'd' was pressed, verifies what function must be called
        {
            switch(menuOption)
            {
            case 1:
                menuFlag=0; //Closes the menu and enters in the game
                if(key=='e'){ //If enter is pressed, we need to take the second press of it
                    getch();
                }
                break;
            case 2:
                options(difficulty, speed, map, up, down, right, left, stop); //Menu Options
                break;
            case 3:
                credits(); //Call Credits
                break;
            case 4:
                if(key=='e'){ //If enter is pressed, we need to take the second press of it
                    getch();
                }
                highscores(0, 0); //Highscores, with no values
                printsHeader();
                break;
            case 5:
                menuFlag=0; //Closes the menu
                endGame=-2; //Ends the game
                break;
            }

            colorCounter=3500;
            key='m';
            menuOption=1;
        }

        colorCounter++;
    }

    return endGame;

}


///Options Menu
void options(int *difficulty, int *speed, int *map, char *up, char *down, char *right, char *left, char *stop)
{

    //Default values of the game
    int dfDifficulty=6, dfSpeed=115, dfMap=1;
    char dfUp='W', dfDown='X', dfRight='D', dfLeft='A', dfStop='S';

    int menuFlag=1, menuOption=1; //Flags
    int colorCounter=3500, color=VERMELHO; //Colors
    char key; //Pressed key
    char temporary='m'; //Temporary key, used to change values, initialized with a random value

    printsOptions(difficulty, speed, map, up, down, right, left, stop);

    //Loop of iterations of the menu
    while(menuFlag)
    {
        if(kbhit()) //When a key is stroked
        {
            key=tolower(detectKeyMenu()); //Detects pressed key

            switch(key) //Verifies if we should go up or down in the options
            {
            case 'w':
                menuOption--;
                colorCounter=3500; //The next options starts red
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                break;
            case 'x':
                menuOption++;
                colorCounter=3500;
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                break;
            }

            if (menuOption>10) //Limits of the menu
            {
                menuOption=1;
            }
            else if(menuOption<1)
            {
                menuOption=10;
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

        switch(menuOption) //The actual selection
        {
        case 1:
            gotoXY(36,6);
            textcolor(color);
            printf(">> DIFFICULTY:              %3d\n", *difficulty);
            break;
        case 2:
            gotoXY(36,7);
            textcolor(color);
            printf(">> DELAY:                   %3d\n", *speed);
            break;
        case 3:
            gotoXY(36,8);
            textcolor(color);
            printf(">> MAP:                     %3d\n", *map);
            break;
        case 4:
            gotoXY(36,17);
            textcolor(color);
            printf(">> UP:                      %3c\n", *up);
            break;
        case 5:
            gotoXY(36,18);
            textcolor(color);
            printf(">> DOWN:                    %3c\n", *down);
            break;
        case 6:
            gotoXY(36,19);
            textcolor(color);
            printf(">> LEFT:                    %3c\n", *left);
            break;
        case 7:
            gotoXY(36,20);
            textcolor(color);
            printf(">> RIGHT:                   %3c\n", *right);
            break;
        case 8:
            gotoXY(36,21);
            textcolor(color);
            printf(">> STOP:                    %3c\n", *stop);
            break;
        case 9:
            gotoXY(36,25);
            textcolor(color);
            printf(">> SAVE");
            break;
        case 10:
            gotoXY(57,25);
            textcolor(color);
            printf(">> DEFAULT");
            break;
        }

        if(key=='d') //Changes the values
        {
            cursorType(2);
            switch(menuOption)
            {
            case 1: //SET DIFFICULTY
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    optionsCanvas();

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

                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 2:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    optionsCanvas();

                    gotoXY(50,31);
                    printf("DELAY");
                    gotoXY(44,32);
                    printf("Between 20 and 180");
                    gotoXY(43,34);
                    printf("The lower the delay");
                    gotoXY(43,35);
                    printf("The faster the game");

                    if(*speed<20 || *speed>180) //Invalid value
                    {
                        gotoXY(41,34);
                        printf("                         ");
                        gotoXY(41,35);
                        printf("     Invalid value       ");
                    }

                    gotoXY(52,33);
                    scanf("%d", speed);
                }
                while(*speed<20 || *speed>180); //Compatibility check

                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 3:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    optionsCanvas();

                    if(*map<1 || *map>QT_MAP) //Invalid value
                    {
                        gotoXY(46,35);
                        printf("Invalid value");
                    }

                    gotoXY(51,31);
                    printf("MAP");
                    gotoXY(45,32);
                    printf("Between 1 and %d", QT_MAP);
                    gotoXY(52,34);
                    scanf("%d", map);
                }
                while(*map<0 || *map>QT_MAP); //Compatibility check

                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 4:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    optionsCanvas();

                    if(temporary<65 || (temporary>90 && temporary<97) || temporary>122) //Invalid value
                    {
                        gotoXY(46,34);
                        printf("Invalid value");
                    }

                    gotoXY(52,31);
                    printf("UP");
                    gotoXY(44,32);
                    printf("Between 'A' and 'Z'");
                    gotoXY(52,33);
                    scanf("%c", &temporary);//Stores the value temporarily
                    temporary=toupper(temporary);

                    //Changing values
                    if(temporary==*down)
                    {
                        *down=*up;
                    }
                    else if(temporary==*left)
                    {
                        *left=*up;
                    }
                    else if(temporary==*right)
                    {
                        *right=*up;
                    }
                    else if(temporary==*stop)
                    {
                        *stop=*up;
                    }

                }
                while(temporary<65 || (temporary>90 && temporary<97) || temporary>122); //Compatibility check
                *up=toupper(temporary); //Assigning the right value

                fflush(stdin);
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 5:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    optionsCanvas();

                    if(temporary<65 || (temporary>90 && temporary<97) || temporary>122) //Invalid value
                    {
                        gotoXY(46,34);
                        printf("Invalid value");
                    }

                    gotoXY(51,31);
                    printf("DOWN");
                    gotoXY(44,32);
                    printf("Between 'A' and 'Z'");
                    gotoXY(52,33);
                    scanf("%c", &temporary);//Stores the value temporarily
                    temporary=toupper(temporary);

                    //Changing values
                    if(temporary==*up)
                    {
                        *up=*down;
                    }
                    else if(temporary==*left)
                    {
                        *left=*down;
                    }
                    else if(temporary==*right)
                    {
                        *right=*down;
                    }
                    else if(temporary==*stop)
                    {
                        *stop=*down;
                    }

                }
                while(temporary<65 || (temporary>90 && temporary<97) || temporary>122); //Compatibility check
                *down=toupper(temporary); //Assigning the right value

                fflush(stdin);
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 6:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    optionsCanvas();

                    if(temporary<65 || (temporary>90 && temporary<97) || temporary>122) //Invalid value
                    {
                        gotoXY(46,34);
                        printf("Invalid value");
                    }

                    gotoXY(51,31);
                    printf("LEFT");
                    gotoXY(44,32);
                    printf("Between 'A' and 'Z'");
                    gotoXY(52,33);
                    scanf("%c", &temporary);//Stores the value temporarily
                    temporary=toupper(temporary);

                    //Changing values
                    if(temporary==*up)
                    {
                        *up=*left;
                    }
                    else if(temporary==*down)
                    {
                        *down=*left;
                    }
                    else if(temporary==*right)
                    {
                        *right=*left;
                    }
                    else if(temporary==*stop)
                    {
                        *stop=*left;
                    }

                }
                while(temporary<65 || (temporary>90 && temporary<97) || temporary>122); //Compatibility check
                *left=toupper(temporary); //Assigning the right value

                fflush(stdin);
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 7:
                do
                {
                    fflush(stdin);
                    textcolor(BRANCO);
                    optionsCanvas();

                    if(temporary<65 || (temporary>90 && temporary<97) || temporary>122) //Invalid value
                    {
                        gotoXY(46,34);
                        printf("Invalid value");
                    }

                    gotoXY(51,31);
                    printf("RIGHT");
                    gotoXY(44,32);
                    printf("Between 'A' and 'Z'");
                    gotoXY(52,33);
                    scanf("%c", &temporary);//Stores the value temporarily
                    temporary=toupper(temporary);

                    //Changing values
                    if(temporary==*up)
                    {
                        *up=*right;
                    }
                    else if(temporary==*down)
                    {
                        *down=*right;
                    }
                    else if(temporary==*left)
                    {
                        *left=*right;
                    }
                    else if(temporary==*stop)
                    {
                        *stop=*right;
                    }

                }
                while(temporary<65 || (temporary>90 && temporary<97) || temporary>122); //Compatibility check
                *right=toupper(temporary); //Assigning the right value

                fflush(stdin);
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 8:
                do
                {
                    textcolor(BRANCO);
                    optionsCanvas();

                    if(temporary<65 || (temporary>90 && temporary<97) || temporary>122) //Invalid value
                    {
                        gotoXY(46,34);
                        printf("Invalid value");
                    }

                    gotoXY(51,31);
                    printf("STOP");
                    gotoXY(44,32);
                    printf("Between 'A' and 'Z'");
                    gotoXY(52,33);
                    scanf("%c", &temporary);//Stores the value temporarily
                    temporary=toupper(temporary);

                    //Changing values
                    if(temporary==*up)
                    {
                        *up=*stop;
                    }
                    else if(temporary==*down)
                    {
                        *down=*stop;
                    }
                    else if(temporary==*right)
                    {
                        *right=*stop;
                    }
                    else if(temporary==*left)
                    {
                        *left=*stop;
                    }

                }
                while(temporary<65 || (temporary>90 && temporary<97) || temporary>122); //Compatibility check
                *stop=toupper(temporary); //Assigning the right value

                fflush(stdin);
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 9:
                menuOption++; //Goes to the default button
                colorCounter=3500;
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 10:
                menuOption--; //Goes to the save button
                colorCounter=3500;
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;

            }
            cursorType(CURSOR);
        }
        else if(key=='a') //Used just on Save or Default button
        {
            switch(menuOption)
            {
            case 9:
                menuOption++; //Goes to default button
                colorCounter=3500;
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            case 10:
                menuOption--; //Goes to save button
                colorCounter=3500;
                printsOptions(difficulty, speed, map, up, down, right, left, stop);
                key='z';
                break;
            }
        }
        else if(key=='e') //If enter was pressed (works in Save and Default button)
        {
            switch(menuOption)
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

    //Save the values in the file, and shows the saved message
    writeSettings(*difficulty, *speed, *map, *up, *down, *right, *left, *stop);
    getch(); //Receives the enter
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


///Credits Menu
void credits(void)
{

    int i, scrollCounter, counter;
    char text[13][45];
    FILE* arq;

    system("cls");
    arq=fopen("data/credits.txt", "r"); //Opens the file
    if(!arq)
    {
        printf("File opening ERROR");
        system("pause");
        return;
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

        for(counter=0, i=11; counter<=12; counter++, i--)  //Prints each line, scrolling it through the screen
        {
            if(scrollCounter>counter && scrollCounter<counter+38)
            {
                gotoXY(30,scrollCounter-counter);
                printf("%s", text[i]);
            }
        }

        if(scrollCounter>12 && scrollCounter<51)  //Cleans the upper line
        {
            gotoXY(30,scrollCounter-12);
            printf("                                                     ");
        }

        scrollCounter==24 ? pacMusic() : Sleep(200); //Pauses the game for a longer time when needed

    }

    system("cls");
    printsHeader();
    return;

}


///Pacman theme
void pacMusic(void)
{

    Beep(493.88,100); Sleep(50);
    Beep(987.77,100); Sleep(50);
    Beep(739.99,100); Sleep(50);
    Beep(622.25,100); Sleep(50);
    Beep(987.77,100); Sleep(50);
    Beep(739.99,100); Sleep(50);
    Beep(622.25,200); Sleep(50);
    Beep(523.25,100); Sleep(50);
    Sleep(70);
    Beep(1046.5,100); Sleep(50);
    Beep(783.99,100); Sleep(50);
    Beep(659.25,100); Sleep(50);
    Beep(1046.5,100); Sleep(50);
    Beep(793.99,100); Sleep(50);
    Beep(659.25,200); Sleep(50);
    Beep(493.88,100); Sleep(50);
    Sleep(70);
    Beep(987.77,100); Sleep(50);
    Beep(739.99,100); Sleep(50);
    Beep(622.25,100); Sleep(50);
    Beep(987.77,100); Sleep(50);
    Beep(739.99,100); Sleep(50);
    Beep(622.25,200); Sleep(50);
    Sleep(100);
    Beep(622.25,70);  Sleep(20);
    Beep(659.25,70);  Sleep(20);
    Beep(698.46,80);  Sleep(20);
    Beep(698.46,70);  Sleep(20);
    Beep(739.99,70);  Sleep(20);
    Beep(793.99,80);  Sleep(20);
    Beep(793.99,70);  Sleep(20);
    Beep(830.61,70);  Sleep(20);
    Beep(880.23,80);  Sleep(20);
    Sleep(50);
    Beep(987.7,100);  Sleep(20);
    Sleep(500);

    return;
}


///Prints the header of the Main Menu
void printsHeader(void)
{

    system("cls");
    textcolor(BRANCO);

    gotoXY(31,8);
    printf(" _ __   __ _  ___ _ __ ___   __ _ _ __  ");
    gotoXY(31,9);
    printf("| '_ \\ / _` |/ __| '_ ` _ \\ / _` | '_  \\ ");
    gotoXY(31,10);
    printf("| |_) | (_| | (__| | | | | | (_| | | | |");
    gotoXY(31, 11);
    printf("| .__/ \\__,_|\\___|_| |_| |_|\\__,_|_| |_|");
    gotoXY(31,12);
    printf("| |");
    gotoXY(31,13);
    printf("|_|");
    gotoXY(37,13);
    printf("################################");
    gotoXY(37,15);
    printf("              PLAY               ");
    gotoXY(37,19);
    printf("             OPTIONS              ");
    gotoXY(37,21);
    printf("             CREDITS              ");
    gotoXY(37,23);
    printf("            HIGHSCORES             ");
    gotoXY(37,27);
    printf("              EXIT               ");
    gotoXY(37,29);
    printf("################################");
    return;
}


///Prints the Options Menu
void printsOptions(int *difficulty, int *speed, int *map, char *up, char *down, char *right, char *left, char *stop)
{

    system("cls");
    textcolor(BRANCO);
    gotoXY(1,1);
    printf("\n                                    ################################\n");
    printf("                                               MAP OPTIONS\n");
    printf("                                    ################################\n\n");
    printf("                                      DIFFICULTY:              %3d\n", *difficulty);
    printf("                                      DELAY:                   %3d\n", *speed);
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


///Prints the canvas of the Option Selection
void optionsCanvas(void)
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


///Detects the pressed key
char detectKeyMenu(void)
{
    char key;

    if (GetAsyncKeyState(0x57)) //'W' key
    {
        getch();
        key = 'w';
    }
    else if(GetAsyncKeyState(VK_UP))
    {
        key = 'w';
        Sleep(150);
    }
    else if (GetAsyncKeyState(0x58)) //'X' key
    {
        getch();
        key = 'x';
    }
    else if(GetAsyncKeyState(VK_DOWN))
    {
        key = 'x';
        Sleep(150);
    }
    else if (GetAsyncKeyState(0x44)) //'D' key
    {
        getch();
        key = 'd';
    }
    else if(GetAsyncKeyState(VK_RIGHT))
    {
        key = 'd';
        Sleep(150);
    }
    else if (GetAsyncKeyState(0x41)) //'A' key
    {
        getch();
        key = 'a';
    }
    else if(GetAsyncKeyState(VK_LEFT))
    {
        key = 'a';
        Sleep(150);
    }
    else if(GetAsyncKeyState(VK_RETURN)) //'Return' key
    {
        key='e';
    }
    else //Else, random key, with no meaning
    {
        key='z';
    }

    return key;

}
