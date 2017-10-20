#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <ctype.h> //Para utilizar o tolower();
#define SPEED 70 //Velocidade padrao do jogo

int const   TOP = 1,
            LEFT = 1;
            HEIGHT = 24,
            WIDTH = 50;

void inicioJogo(void){

    printf("          #####################################\n");
    printf("                       PAC-MAN                 \n");
    printf("          Pressione qualquer tecla para comecar\n");
    printf("          #####################################\n                          ");
    getch();

}

void movimentacao(void){

    int x=LEFT, y=TOP;
    int flag = 1, flag_2=1;
    char key = 'p', last='p';
    while(flag)
    {
        clrscr();
        gotoxy(15, 25);
        printf("Press 'space' to quit"); //Mensagem de saida

        if (flag_2){ //Mensagem de inicio
            gotoxy(7,12);
            printf("Pressione as teclas de movimentacao para comecar");
        }

        gotoxy(1,1);
        if(kbhit())
        {
            key = tolower(getch()); //Detecta tecla pressionada
            flag_2=0;

            switch(key) //Verifica para onde será a nova direção
            {
            case 'w':
                y--;
                last=key; //Faz podermos manter andando na mesma direcao
                break;
            case 'x':
                y++;
                last=key;
                break;
            case 'a':
                x--;
                last=key;
                break;
            case 'd':
                x++;
                last=key;
                break;
            case 's':
                last = key;
                break;
            case ' ':
                flag = 0;
                break;
            }
        }
        else
        {
            switch(last) //Verifica a ultima direção que foi solicitado para andar
            {
            case 'w':
                y--;
                break;
            case 'x':
                y++;
                break;
            case 'a':
                x--;
                break;
            case 'd':
                x++;
                break;
            case 's':
                break;
            default:
                break;
            }
        }

        if(y < TOP) //Ao chegar nos limites do mapa, vai para o outro lado
            {
                y= HEIGHT;
            }
            else if(y > HEIGHT)
            {
                y = TOP;
            }else if(x < LEFT)
            {
                x=WIDTH;
            }
            else if(x > WIDTH)
            {
                x = LEFT;
            }

        gotoxy(x, y);
        printf("%c", 'C');
        gotoxy(x, y);

        if (last=='d' || last=='a')
        {
            Sleep(SPEED);
        }
        else
        {
            Sleep(SPEED*1.6); //Corrige velocidade maior para baixo
        }
    }

    clrscr();
    gotoxy(1,1);
    printf("O programa sera encerrado!                     \n");
    system("pause");



;}



int main()
{

    inicioJogo(); //Mensagem de inicio do jogo

    movimentacao(); //Movimentação do PacMan

    return EXIT_SUCCESS;
}
