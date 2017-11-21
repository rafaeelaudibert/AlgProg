//Headers includes
#include "main.h"
#include "structs.h"
#include "labirinto.h"
#include "pacman.h"
#include "ghosts.h"
#include "objects.h"
#include "messages.h"
#include "auxiliars.h"

//Pacman Controller
int pacmanControl(int* qtde_pacdots, int* points, pacmanInfo* pacman, clock_t* pacEndTimer, clock_t* pacStartTimer, int speed, char lab[HEIGHT][WIDTH], ghosts* fantasmas)
{
    float correcaoVelocidade= pacman->last.coordinates=='y' ? 1.4 : 1; //Correção de distorção das letras


    *pacEndTimer=clock(); //Seta para verificar tempo atual do sistema

    if(((*pacEndTimer)-(*pacStartTimer))>speed*correcaoVelocidade) //Ao ter passado tempo igual a velocidade, executa o loop
    {
        *pacStartTimer=*pacEndTimer; //"Zera" o contador inicio
        movePacman(pacman, lab);
        checkPacDots(qtde_pacdots, points, lab, *pacman);
        checkPowerPellets(points, lab, pacman);

        if(checkGhostCollision((*pacman), points, fantasmas) && !pacman->pacDotActive)
        {
            return 0;
        }

        gotoXY(1,1);
    }

    return 1;

}

//Movimentação e impressão do PacMan na posição correta
void movePacman(pacmanInfo *pacman, char lab[HEIGHT][WIDTH])
{

    gotoXY(pacman->x, pacman->y); //Apaga a posição atual do pacman
    printf(" ");

    switch(pacman->next.coordinates) //Calcula a proxima posição do pacman
    {
    case 'y':
        pacman->y+=pacman->next.aumenta_diminui;
        break;
    case 'x':
        pacman->x+=pacman->next.aumenta_diminui;
        break;
    }

    testColision(pacman, lab); //Caso comando coloque o pacman dentro de uma parede, tira ele de lá
    testLimits(pacman); //Caso tenha chegado nos limites do mapa, coloca pacman no outro lado

    //Imprime a nova posição do pacman
    textcolor(AMARELO);
    gotoXY(pacman->x,pacman->y);
    printf("C");

    return;
}

//Testa se pacman chegou no limite do mapa, mandando ele para o outro lado
void testLimits(pacmanInfo *pacman)
{

    if(pacman->y<TOP)
    {
        pacman->y=HEIGHT;
    }
    else if(pacman->y>HEIGHT)
    {
        pacman->y=TOP;
    }
    else if(pacman->x<LEFT)
    {
        pacman->x=WIDTH;
    }
    else if(pacman->x>WIDTH)
    {
        pacman->x=LEFT;
    }

    return;
}

//Testa colisão do pacman com as paredes
void testColision(pacmanInfo *pacman, char lab[HEIGHT][WIDTH])
{

    if(lab[pacman->y-1][pacman->x-1]=='#' && (pacman->y>TOP-1 && pacman->y<HEIGHT+1) && (pacman->x>LEFT-1 && pacman->x<WIDTH+1)) //Caso esteja dentro de um campo 'parede' E dentro do mapa
    {
        switch(pacman->next.coordinates) //Volta a ultima posição andada
        {
        case 'y':
            pacman->y-=pacman->next.aumenta_diminui;
            break;
        case 'x':
            pacman->x-=pacman->next.aumenta_diminui;
            break;
        }

        //Faz pacman continuar andando
        switch(pacman->last.coordinates)
        {
        case 'y':
            pacman->y+=pacman->last.aumenta_diminui;
            break;
        case 'x':
            pacman->x+=pacman->last.aumenta_diminui;
            break;
        }


        if(lab[pacman->y-1][pacman->x-1]=='#') //Caso ja esteja em uma esquina, e seja forçado a entrar na parede, faz não entrar nela
        {
            switch(pacman->last.coordinates)
            {
            case 'y':
                pacman->y-=pacman->last.aumenta_diminui;
                break;
            case 'x':
                pacman->x-=pacman->last.aumenta_diminui;
                break;
            }
        }
    }

    else //Senão, confirma que ocorreu um movimento valido, e seta a ultima posição para ser igual a atual, para funcionar a proxima iteração
    {
        pacman->last.coordinates=pacman->next.coordinates;
        pacman->last.aumenta_diminui=pacman->next.aumenta_diminui;
    }

    return;
}

//Seta direção que o PacMan irá seguir
void setDirection(char key, int* continua, pacmanInfo *pacman)
{

    switch(key) //Verifica para onde será a nova direção
    {
    case 'w':
        pacman->next.coordinates='y'; //Seta direção no eixo y
        pacman->next.aumenta_diminui=-1; //Seta direção negativa
        break;
    case 'x':
        pacman->next.coordinates='y';
        pacman->next.aumenta_diminui=1; //Seta direção positiva
        break;
    case 'a':
        pacman->next.coordinates='x'; //Seta direção no eixo x
        pacman->next.aumenta_diminui=-1;
        break;
    case 'd':
        pacman->next.coordinates='x';
        pacman->next.aumenta_diminui=1;
        break;
    case 's':
        pacman->next.coordinates='s';
        pacman->next.aumenta_diminui=0;
        break;
    case 'p': //Pausa o jogo ao pressionar 'P'
        gamePause();
        break;
    case ' ':
        *continua=0; //Irá fazer o programa terminar sua execução
        pacman->next.coordinates='s'; //Faz pacman parar sua movimentação
        pacman->next.aumenta_diminui=0;
        break;
    }

}
