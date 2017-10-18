#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

int const   TOP = 2,
            LEFT = 1;
            HEIGHT = 50,
            WIDTH = 100;

int main()
{
    int x=LEFT, y=TOP;
    int flag = 1;
    char key = 'c';
    while(flag){
        clrscr();
        gotoxy(1, 1);
        printf("Press 'space' to end");
        textcolor(300);
        if(kbhit()){
            key = getch();
            switch(key){
                case 'w':
                    y--;
                    break;
                case 's':
                    y++;
                    break;
                case 'a':
                   x--;
                    break;
                case 'd':
                   x++;
                    break;
                case ' ':
                    flag = 0;
                    break;
            }
            if(y < TOP) y=TOP;
            else
            if(y > HEIGHT) y = HEIGHT;

            if(x < LEFT)x=LEFT;
            else
            if(x > WIDTH)x = WIDTH;
        }
        gotoxy(x, y);
        printf("%c", 'C');
        gotoxy(x, y);
        Sleep(100);
    }
    return 0;
}
