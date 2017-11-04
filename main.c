//Includes das Libs
#include <stdio.h>
#include <stdlib.h>
#include <conio.h> //Para utilizar funções de movimentações do jogo
#include <windows.h> //Para detectar keystrokes
#include <ctype.h> //Para utilizar o tolower();
#include <time.h> //Para utilizar função clock();

//Includes de headers locais
#include "labirinto.h"


//Structs
struct //Armazena eixo andado, e direção no eixo
{
    char coordinates; //'x', 'y' ou 's'
    int aumenta_diminui; //1 - Aumenta; -1 - Diminui; 0 - parado
} typedef directions;

struct //Armazena posição do pacman, e suas informações
{
    int x; //x-axis position
    int y; //y-axis position
    int lives; //lives remaining
} typedef pacmanPosition;



//Inclusão dos protópipos das funções da conio.c
void clrscr(void); //Função que limpa a tela
void _setcursortype(int); //Função para modificar o tipo de cursor
void textcolor(int); //Função que controla cor do texto

//Protótipo das funções locais
void pacmanStart(void);
void pacmanPause(void);
void pacmanEnd(void);

void startGameMenu(void);
int startMessage(int);
void checkPacDots(int*);
void checkPowerPellets(void);

void pacmanControl(directions*, directions*, int*);
void setDirection(char, int*, directions*);
void movePacman(directions*, directions*);
void testLimits(void);
void testColision(directions*, directions*);

char detectKey(void);
void gotoXY(int, int);
void reconstructMaze(int, int, int, int);


//Constantes
int  const  TOP = 1, //Limite superior do mapa (Nunca menor que 0)
            LEFT = 1, //Limite esquerdo do mapa (Nunca menor que 0)
            HEIGHT = 30, //Limite inferior do mapa
            WIDTH = 100, //Limite direito do mapa
            CURSOR = 0; // 0 para sem cursor; 1 para cursor de caixa; 2 para cursor normal

//Variaveis Globais
pacmanPosition pacman; //Struct responsavel pelo pacman
char lab[30][100]; //Variavel responsavel por armazenar o labirinto
int points=0; //Contador de pontos
clock_t pacmanStartTimer, pacmanEndTimer; //Timers do jogo
int pacDotActive, speed; //Variaveis responsaveis por guardar estado de poder do pacman, e sua velocidade



//Pac-man Main
int main()
{
    system("mode 100, 37"); //Defines screen size
    _setcursortype(CURSOR); //Sets the cursor according to a value declared in the constant 'CURSOR'

    startGameMenu(); //Start message

    pacman.lives=3; //Sets the initial number of lives of the pacman
    speed=100; //Sets the initial speed of the game
    pacDotActive=0;
    pacmanStart(); //The Game 'per se'

    return EXIT_SUCCESS; //End of the program
}



//Funções Locais

//Jogo em si
void pacmanStart(void)
{
    int mostraStart=1; //Flag para mensagem inicial
    int continuaJogo=1, *point_continua=&continuaJogo; //Flag para loop do jogo
    int qtdePastilhasTotal, *pQtdePastilhasTotal=&qtdePastilhasTotal; //Qtde de pastilhas no tabuleiro
    int qtdePastilhasComidas=0, *pQtdePastilhasComidas=&qtdePastilhasComidas; //Quantidade de pastilhas comidas nessa rodada
    char key; //Tecla pressionada

    //Structs
    directions nextDirection, *pNextDirection=&nextDirection; //Struct que armazena dados da posição a ser andada
    directions lastDirection, *pLastDirection=&lastDirection; //Struct que armazena dados da ultima posição andada

    pacman.lives--; //Ao iniciar o jogo, diminui uma vida do pacman
    if(!pacman.lives)  //Se acabarem as vidas do pacman, finaliza o jogo
    {
        return;
    }

    if(showLab(lab, pQtdePastilhasTotal, &pacman.x, &pacman.y))  //Carrega o labirinto, a posição do pacman, das pastilhas, das super-pastilhas e dos fantasmas na memória
    {
        printf("ERROR!");
        system("Pause");
        return; //Se der erro ao carregar, encerra o programa
    }

    //Mensagem de saida
    gotoXY(36, 31);
    printf("Press 'Space' or 'ESC' to quit");
    gotoXY(46, 32);
    printf("Pontos: %d", points);

    //Starta o timer e roda o jogo
    pacmanStartTimer=clock();
    while(continuaJogo)
    {
        if(mostraStart>-1)
        {
            mostraStart=startMessage(mostraStart); //Mostra mensagem de inicio enquanto necessario
        }

        if(kbhit()) //Ao pressionar uma tecla
        {
            //Flag para fazer não aparecer mais a mensagem inicial
            //Primeiro faz não imprimir mais a mensagem inicial, depois reconstroi o labirinto embaixo dela
            if(mostraStart>-1)
            {
                mostraStart--;
            }

            key=tolower(detectKey()); //Detecta tecla pressionada
            setDirection(key, point_continua, pNextDirection); //Decodifica tecla pressionada
        }

        pacmanControl(pNextDirection, pLastDirection, pQtdePastilhasComidas); //Controle do pacman

        if(qtdePastilhasComidas==qtdePastilhasTotal) //Ao comer todas pastilhas, termina o jogo
        {
            pacmanEnd();
            return;
        }

    } //FIM DO WHILE

    pacmanEnd(); //Finaliza o jogo

    return;
}

//Pausa o jogo
void pacmanPause(void)
{
    char key='m'; //Inicializa com qualquer outro valor, para nao cair no While
    int count;

    gotoXY(41,13);
    printf("                     ");
    gotoXY(41,14);
    printf("    Game paused!     ");
    gotoXY(41,15);
    printf(" Press 'R' to resume ");
    gotoXY(41,16);
    printf("                     ");
    gotoXY(41,17);
    printf("                     ");


    while(key!='r')
    {
        if(kbhit())
        {
            if(GetAsyncKeyState(0x52))  //Ao clicar no R sairá desse loop
            {
                key='r';
            }
        }
    }

    for(count=3; count>0; count--)  //Contagem regressiva ao voltar para o jogo
    {
        gotoXY(45, 16);
        printf("%d seconds...", count);
        Sleep(1000);
    }

    reconstructMaze(12,18,40,62);

    return;
}

//Mensagem de término de jogo
void pacmanEnd(void)
{
    gotoXY(36,31);
    printf("The program will be finished!  ");
    gotoXY(35,32);
    printf("Press any key to close the game");
    gotoXY(29,33);
    printf("                                   ");

    textcolor(0); //Deixa texto em preto, tornando-o invisivel
    gotoXY(1,33);
    system("pause");
    textcolor(15);

    return;
}


//Apresenta menu de inicio do jogo
void startGameMenu(void)
{
    int contador=0;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/pacman.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    gotoXY(20, contador+1);
    Sleep(50);
    while( (ch=fgetc(arq))!=EOF)   //Impressão do PacMan
    {
        if(contador>19)
        {
            textcolor(15);
        }
        else
        {
            textcolor(14);
        }

        printf("%c", ch);

        if(ch=='\n' && contador<25)
        {
            contador++;
            gotoXY(20, contador+1);
            Sleep((contador-25)*-6); //Acelera a impressão de cada linha aos poucos
        }
    }

    getch();
    clrscr();
    return;
}

//Mensagem que aparece no inicio do jogo
int startMessage(int flag)
{

    if (flag==1)  //Mensagem de inicio
    {
        gotoXY(39, 14);
        printf("                         ");
        gotoXY(39,15);
        printf(" Press any key to start  ");
        gotoXY(39, 16);
        printf("                         ");
    }
    else if(flag==0)
    {
        reconstructMaze(13,15,38,65);
        flag--;
    }

    return flag;
}

//Checagem das pastilhas
void checkPacDots(int* pacdots)
{

    if(lab[pacman.y-1][pacman.x-1]=='o')
    {
        lab[pacman.y-1][pacman.x-1]=' ';
        points+=10; //Aumenta contador de pontos
        *pacdots+=1; //Incrementa 1 no contador de pastilhas comidas
        //Beep(500, 30); //- Comentado até sabermos se utilizaremos isso ou não


        gotoXY(46, 32);
        printf("Pontos: %d", points);
    }

}

//Checagem das super-pastilhas
void checkPowerPellets(void)
{

    if(lab[pacman.y-1][pacman.x-1]=='*')
    {
        lab[pacman.y-1][pacman.x-1]=' ';
        points+=50;
        Beep(1000, 50);
        pacDotActive=71; //≈≈5 segundos(4970ms), divididos em clocks de 70 milissegundos

        gotoXY(46, 32);
        printf("Pontos: %d", points);
    }

    //Alteração da velocidade, e timer da duração do super-poder
    if(pacDotActive>0)
    {
        speed=70;
        pacDotActive--;
        gotoXY(29,33);
        printf("Tempo restante de invulnerabilidade: %4dms", pacDotActive*70);
    }
    else
    {
        speed=100;
        gotoXY(29,33);
        printf("                                             ");
    }
}



//Controla pacman
void pacmanControl(directions* next, directions* last, int* qtde_pacdots)
{
    float velocidade;

    if ((*last).coordinates=='y')
    {
        velocidade=speed*1.4; //Correção da distorção das letras
    }
    else
    {
        velocidade=speed;
    }

    pacmanEndTimer=clock(); //Seta para verificar tempo atual do sistema
    if((pacmanEndTimer-pacmanStartTimer)>velocidade) //Ao ter passado tempo igual a velocidade, executa o loop
    {
        pacmanStartTimer=pacmanEndTimer; //"Zera" o contador inicio
        movePacman(next, last);
        checkPacDots(qtde_pacdots);
        checkPowerPellets();
    }
}

//Seta direção que o PacMan irá seguir
void setDirection(char key, int* pointer, directions* next)
{

    switch(key) //Verifica para onde será a nova direção
    {
    case 'w':
        (*next).coordinates='y'; //Seta direção no eixo y
        (*next).aumenta_diminui=-1; //Seta direção negativa
        break;
    case 'x':
        (*next).coordinates='y';
        (*next).aumenta_diminui=1; //Seta direção positiva
        break;
    case 'a':
        (*next).coordinates='x'; //Seta direção no eixo x
        (*next).aumenta_diminui=-1;
        break;
    case 'd':
        (*next).coordinates='x';
        (*next).aumenta_diminui=1;
        break;
    case 's':
        (*next).coordinates='s';
        (*next).aumenta_diminui=0;
        break;
    case 'p': //Pausa o jogo ao pressionar 'P'
        pacmanPause();
        break;
    case ' ':
        *pointer=0; //Irá fazer o programa terminar sua execução
        (*next).coordinates='s'; //Faz pacman parar sua movimentação
        (*next).aumenta_diminui=0;
        break;
    }

}

//Movimentação e impressão do PacMan na posição correta
void movePacman(directions* next, directions* last)
{

    gotoXY(pacman.x, pacman.y); //Apaga a posição atual do pacman
    printf(" ");

    switch((*next).coordinates) //Calcula a proxima posição do pacman
    {
    case 'y':
        pacman.y+=(*next).aumenta_diminui;
        break;
    case 'x':
        pacman.x+=(*next).aumenta_diminui;
        break;
    }

    testLimits(); //Caso tenha chegado nos limites do mapa, coloca pacman no outro lado
    testColision(next, last); //Caso comando coloque o pacman dentro de uma parede, tira ele de lá

    //Imprime a nova posição do pacman
    gotoXY(pacman.x,pacman.y);
    printf("C");

    return;
}

//Testa se pacman chegou no limite do mapa, mandando ele para o outro lado
void testLimits(void)
{

    if(pacman.y<TOP)
    {
        pacman.y=HEIGHT;
    }
    else if(pacman.y>HEIGHT)
    {
        pacman.y=TOP;
    }
    else if(pacman.x<LEFT)
    {
        pacman.x=WIDTH;
    }
    else if(pacman.x>WIDTH)
    {
        pacman.x=LEFT;
    }

    return;
}

//Testa colisão do pacman com as paredes
void testColision(directions* next, directions* last)
{

    if(lab[pacman.y-1][pacman.x-1]=='#') //Caso esteja dentro de um campo 'parede'
    {
        switch((*next).coordinates) //Volta a ultima posição andada
        {
        case 'y':
            pacman.y-=(*next).aumenta_diminui;
            break;
        case 'x':
            pacman.x-=(*next).aumenta_diminui;
            break;
        }

        if((*next).coordinates!=(*last).coordinates) //Faz continuar andando na ultima posição andada nessa iteração
        {
            switch((*last).coordinates)
            {
            case 'y':
                pacman.y+=(*last).aumenta_diminui;
                break;
            case 'x':
                pacman.x+=(*last).aumenta_diminui;
                break;
            }
        }
        //Faz programa continuar correndo na ultima direção andada na proxima iteração
        (*next).aumenta_diminui=(*last).aumenta_diminui;
        (*next).coordinates=(*last).coordinates;

        if(lab[pacman.y-1][pacman.x-1]=='#') //Caso ja esteja em uma esquina, e seja forçado a entrar na parede, faz não entrar nela
        {
            switch((*next).coordinates)
            {
            case 'y':
                pacman.y-=(*next).aumenta_diminui;
                break;
            case 'x':
                pacman.x-=(*next).aumenta_diminui;
                break;
            }
        }

    }
    else //Senão, confirma que ocorreu um movimento valido, e seta a ultima posição para ser igual a atual, para funcionar a proxima iteração
    {
        (*last).coordinates=(*next).coordinates;
        (*last).aumenta_diminui=(*next).aumenta_diminui;
    }

    return;
}


//Detecta a tecla pressionada
char detectKey(void)
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
    else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) //Tecla para esquerda ou tecla 'A'
    {
        key = 'a';
    }
    else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) //Tecla para direita ou tecla 'D'
    {
        key = 'd';
    }
    else if (GetAsyncKeyState(0x53)) //Tecla 'S'
    {
        key = 's';
    }
    else if (GetAsyncKeyState(0x50)) //Tecla 'P'
    {
        key = 'p';
    }
    else if (GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_SPACE)) //Tecla 'ESC' ou tecla 'Espaço'
    {
        key = ' ';
    }
    else //Senão retorna uma letra que não significa nenhuma direção, assim será utilizada a ultima direção andada
    {
        key='m';
    }

    return key;

}


// Função gotoXY, tem limite igual a: system("mode 'x-1', 'y-1'");
// índice inicia em 0
// vai até system(mode x-1, y-1)
void gotoXY(int x, int y)
{
    COORD coord;
    coord.X = x-1;
    coord.Y = y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Função que reconstroi parte do labirinto conforme parametros passados
void reconstructMaze(int y_inicial, int y_final, int x_inicial, int x_final)
{

    int contador, count;
    for(contador=y_inicial; contador<=y_final; contador++)
    {
        for (count=x_inicial; count<=x_final; count++)
        {
            gotoXY(count+1,contador+1);
            if(lab[contador][count]=='#')
            {
                textcolor(15);
                printf("%c", lab[contador][count]);
            }
            else if(lab[contador][count]=='o')
            {
                textcolor(14);
                printf("%c", lab[contador][count]);
            }
            else
            {
                printf(" ");
            }

        }
    }

    gotoXY(pacman.x, pacman.y);
    printf("C");

    return;
}


//TODO LIST:

//CMD
// Criar 3 tipos diferentes de final de jogo: • Desistencia
//                                            • Vitoria
//                                            • Derrota

//PACMAN
//Detecção de colisão com os fantasmas, pacman.lives--, Respawn;

//FANTASMAS
//Movimentação dos fantasmas (Movimentação com SuperPastilha OFF/ON - Movimentação 30% menor)
//Timer para ressuscitar
//Na struct, adicionar parametro 'Alive' (0 - Morto, 1 - Vivo)

//PASTILHAS
//Criar função que detecta que comeu todas pastilhas -> EndGame - WIN

//SUPER-PASTILHAS
//Criar ligação entre super-pastilhas e fantasmas


//EXTRAS:
//Dijkshtra - Sistema de detecção de caminho dos fantasmas mais inteligente
//Highscores
//Adicionar Cheat no F9 (Desativa deteccção de colisão com as paredes - Paredes valem 10000 pontos)

//FIM DO ARQUIVO
