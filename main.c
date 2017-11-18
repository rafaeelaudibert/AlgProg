//Libraries inclusion
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //Detect keystrokes
#include <ctype.h> //To use tolower();
#include <time.h> //To use clock();
#include <conio.h> //To use textcolor() & kbhit();
#include <string.h> //To use gets/puts;

//Local headers inclusions
#include "labirinto.h"

//Function Prototypes
void gameStart(void);
int pacmanControl(int*, int*);

void gamePause(void);
void gameEnd(void);
void gameWin(int);
void gameLost(void);
void highscores(int);
char detectKey(void);

void movePacman(void);
void checkPacDots(int*, int*);
void checkPowerPellets(int*);
void testLimits(void);
void testColision(void);
void setDirection(char, int*);

void startMenu(void);
int startMessage(int);
void printTop10(void);
void reconstructMaze(int, int, int, int);
void gotoXY(int, int);
void cursorType(int);
void textcolor(); //conio.c

int ghostsControl(); // controla o tempo de movimento dos fantasmas

//Constants
int  const  TOP = 1, //Top map limit (Never less than 1)
            LEFT = 1, //Left map limit (Never less than 1)
            HEIGHT = 30, //Bottom map limit
            WIDTH = 100, //Right map limit
            CURSOR = 0, // 0 - no cursor; 1 - box cursor; 2 - normal cursor
            SLOW_SPEED=157, //Ghost's speed while under effect of PowerPellets (30% less than the normal speed)
            NORMAL_SPEED=115; //Default game's speed


//Enumerações
enum cores
{
    PRETO=0,
    CINZA=8,
    AZUL, //8+1
    VERDE,
    VERDE_AGUA,
    VERMELHO,
    LILAS,
    AMARELO,
    BRANCO //15
};


//Variaveis Globais
pacmanInfo pacman; //Struct whose stores the pacman
char lab[30][100]; //Variable whose stores the maze
int speed; //Variable whose stores the actual speed of the game
clock_t pacStartTimer, pacEndTimer, ghostsTime; //Timers do jogo
int points=0; //Points counter
int totalPacDots, eatenPacDots=0; //PacDots quantities

//Pac-man Main
int main()
{
    system("mode 100, 45"); //Defines CMD's screen size
    system("title Pacman"); //Defines CMD's title
    cursorType(CURSOR); //Sets the cursor according to a value declared in the constant 'CURSOR'
    srand( time(NULL) ); // alimenta a semente de rand com um n�mero do tempo atual

    //Setting some variables
    pacman.lives=3; //Sets the initial number of lives of the pacman
    speed=NORMAL_SPEED; //Sets the initial speed of the game
    pacman.pacDotActive=0; //Sets pacman "not powered"
    ghostsTime = clock(); //initial time of the ghosts

    iniciaLabirinto(lab, &totalPacDots, pacman); // Set the initial positions of the game
    startMenu(); //Start message

    //THE GAME
    while(pacman.lives>=0) //While pacman still has lives, keeps playing the game
    {
        pacman.lives--; //When starts the game, takes one live out of pacman
        if(pacman.lives>=0)
        {
            gameStart(); //The Game 'per se'
            while(kbhit())
            {
                getch();
            }
        }
    }

    if(pacman.lives==-1) //Se acabar com 0 vidas, mostra que pacman perdeu
    {
        gameLost();
    }

    if(pacman.lives!=-2)
    {
        highscores(points); //Tabela de Highscores
    }


    gotoXY(1,40);
    textcolor(PRETO);
    return EXIT_SUCCESS; //End of the program
}



//Start of the game
void gameStart(void)
{
    int showStartMessage=1; //Starting Message Flag
    int continueGame=1;//Game loop flag
    char key='j'; //Stroked key with a non useful, but known value

    system("cls");
    if(showLab(lab, &totalPacDots, &pacman.x, &pacman.y))  //Loads the maze, pac's, pacDots's, powerPellets' & ghost's coordinates in the memory
    {
        printf("ERROR!");
        system("Pause");
        return; //If there is an error at loading it, finishes the game
    }

    //Exit message
    textcolor(BRANCO);
    gotoXY(36, 31);
    printf("Press 'Space' or 'ESC' to quit");
    gotoXY(36, 32);
    printf("Points: %5d         Lives: %d", points, pacman.lives);

    //Starts timer, and makes the game to start
    pacStartTimer=clock();
    while(continueGame)
    {
        if(showStartMessage>-1)
        {
            showStartMessage=startMessage(showStartMessage); //Shows start message when necessary
        }

        if(kbhit()) //When a key is stroked
        {
            //Flag to make the start message not appear in the next iteration
            //First, it makes not to print the initial message anymore, after rebuilds the maze under it
            if(showStartMessage>-1)
            {
                showStartMessage--;
            }

            key=tolower(detectKey()); //Detects pressed key
            setDirection(key, &continueGame); //Decodifies pressed key
        }


        if(!pacmanControl(&eatenPacDots, &points))  //Controls pacman
        {
            pacman.next.coordinates='s';
            pacman.next.aumenta_diminui='0';
            return;  //Ends the game, if there is a collision between the ghost and the pacman

        }

        if(key!='j' && !ghostsControl(&points)) //Controls ghosts
        {
            pacman.next.coordinates='s';
            pacman.next.aumenta_diminui='0';
            return; //Ends the game, if there is a collision between the ghost and the pacman
        }

        if(eatenPacDots==totalPacDots) //If all pacDots have been eaten, ends the game
        {
            points+=((pacman.lives+1)*150); //150 points, each lasting life
            gameWin(points);
            return;
        }

    } //WHILE END

    gameEnd(); //Ends the game
    pacman.lives=-2; //This way, the Highscore menu is not shown

    return;
}

//Pacman Controller
int pacmanControl(int* qtde_pacdots, int* points)
{
    float correcaoVelocidade;

    if (pacman.last.coordinates=='y')
    {
        correcaoVelocidade=1.4; //Correção da distorção das letras
    }
    else
    {
        correcaoVelocidade=1;
    }

    pacEndTimer=clock(); //Seta para verificar tempo atual do sistema
    if((pacEndTimer-pacStartTimer)>speed*correcaoVelocidade) //Ao ter passado tempo igual a velocidade, executa o loop
    {
        pacStartTimer=pacEndTimer; //"Zera" o contador inicio
        movePacman();
        checkPacDots(qtde_pacdots, points);
        checkPowerPellets(points);

        if(checkGhostCollision(pacman, points) && !pacman.pacDotActive)
        {
            return 0;
        }

        gotoxy2(0,0);
    }

    return 1;

}

int ghostsControl(int *points)
{

    float correcaoVelocidade;

    if (pacman.last.coordinates=='y')
    {
        correcaoVelocidade=1.4; //Correção da distorção das letras
    }
    else
    {
        correcaoVelocidade=1;
    }

    clock_t atualTime = clock();

    if((atualTime - ghostsTime) > ( pacman.pacDotActive ? SLOW_SPEED*correcaoVelocidade : NORMAL_SPEED*correcaoVelocidade))
    {
        ghostsTime = atualTime;
        moveGhost(pacman, lab); // update of the moviment of the ghosts
        showGhosts(pacman, lab); // update and show the position of the ghosts

        if(checkGhostCollision(pacman, points) && !pacman.pacDotActive)
        {
            return 0;
        }
    }
    return 1;
}

//Pausa o jogo
void gamePause(void)
{
    char key='m'; //Inicializa com qualquer outro valor, para nao cair no While
    int count;

    textcolor(BRANCO);
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
        textcolor(BRANCO);
        gotoXY(45, 16);
        printf("%d seconds...", count);
        Sleep(1000);
    }

    reconstructMaze(12,18,40,62);

    return;
}

//Mensagem de término de jogo, caso jogador simplesmente desista
void gameEnd(void)
{
    textcolor(BRANCO);
    gotoXY(36,31);
    printf("The program will be finished!  ");
    gotoXY(32,32);
    printf("Press any key twice to close the game");
    gotoXY(29,33);
    printf("                                              ");

    textcolor(PRETO); //Deixa texto em preto, tornando-o invisivel
    gotoXY(1,33);
    system("pause");
    textcolor(BRANCO);

}

//Mensagem de término de jogo, caso player ganhe o jogo
void gameWin(int points)
{
    int contador=0;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/pacmanWin.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    system("cls");
    gotoXY(26, contador+6);
    Sleep(50);
    while( (ch=fgetc(arq))!=EOF)   //Impressão do PacMan
    {
        if(contador>19)
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
            gotoXY(26, contador+6);
            Sleep(10); //Imprime uma linha por vez
        }
    }

    getch();
    printf("\n\n");
    textcolor(PRETO);
    system("pause");
    pacman.lives=-3;
    return;
}

//Mensagem de término de jogo, caso player perca o jogo
void gameLost(void)
{
    int contador=0;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/pacmanLost.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    system("cls");
    gotoXY(26, contador+6);
    Sleep(50);
    while( (ch=fgetc(arq))!=EOF)   //Impressão do PacMan
    {
        if(contador>19)
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
            gotoXY(26, contador+6);
            Sleep(10); //Imprime uma linha por vez
        }
    }

    getch();
    printf("\n\n");
    textcolor(PRETO);
    system("pause");
    return;
}

void highscores(int points)
{
    PLACAR Highscores[11];
    JOGO_ATUAL dados= {{'a','\0'}, -50};
    FILE *arq;
    char Linha[100], dateStrTemp[9], url[25]= {"data/Highscores.txt"};
    int i, j, k=AZUL;
    int flag=1, flag2=0, flag3=1;

    system("cls");

    textcolor(BRANCO);
    gotoXY(1,1);
    // Abre um arquivo TEXTO para LEITURA
    arq = fopen(url, "rt");
    if (arq == NULL)  // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return;
    }
    else //Lê arquivo
    {
        for(i=0; i<10; i++)
        {
            // Lê uma linha (inclusive com o '\n')
            fgets(Linha, 10, arq);  // o 'fgets' lê até 14 caracteres ou até o '\n'
            Highscores[i].posicao=atoi(Linha);

            fgets(Linha, 31, arq); // o 'fgets' lê até 30 caracteres ou até o '\n'
            strcpy(Highscores[i].nome,Linha);

            fgets(Linha, 10, arq);
            Highscores[i].pontos=atoi(Linha);

            fgets(Linha, 16, arq);
            strcpy(Highscores[i].dateStr,Linha);

            fgets(Linha, 16, arq);
            strcpy(Highscores[i].timeStr,Linha);

        }
    }
    fclose(arq);

    if(points>Highscores[9].pontos) //Verifica se essa partida entrará para o placar
    {
        //MENSAGEM
        fflush(stdin);
        gotoXY(32,17);
        printf("CONGRATULATIONS, YOU ARE IN THE TOP10!\n");
        gotoXY(38,18);
        printf("You have made %d points!\n", points);
        gotoXY(35,19);
        printf("Please, insert your first name: ");
        printTop10();
        cursorType(2); //Faz cursor aparecer
        textcolor(VERMELHO);
        gotoXY(47,20);

        //Operações para conseguir nome e pontos caso esteja no TOP10
        gets(dados.nome);
        dados.nome[strlen(dados.nome)]='\n';
        dados.nome[strlen(dados.nome)+1]='\0';
        dados.pontos=points;
    }

    //MANIPULAÇÃO E PRINTAGEM DOS DADOS DO PLACAR
    cursorType(CURSOR); //Faz cursor desaparecer novamente
    system("cls");
    //Mensagem inicial
    textcolor(BRANCO);
    gotoXY(1,8);
    printf("        POSICAO   NOME                         PONTUACAO         DATA            HORA");


    //Manipulação dos dados, e printagem do Ranking
    for(i=0; i<10; i++)
    {
        if(Highscores[i].pontos<dados.pontos && flag)
        {
            for(j=9; j>=i; j--)
            {
                Highscores[j].pontos=Highscores[j-1].pontos;
                strcpy(Highscores[j].nome, Highscores[j-1].nome);
                strcpy(Highscores[j].dateStr, Highscores[j-1].dateStr);
                strcpy(Highscores[j].timeStr, Highscores[j-1].timeStr);
            }
            Highscores[i].pontos=dados.pontos;
            strcpy(Highscores[i].nome, dados.nome);

            _strdate(Highscores[i].dateStr); //Data em formato MMDDAA
            strcpy(dateStrTemp, Highscores[i].dateStr); //Transformação para DDMMAA
            Highscores[i].dateStr[0]=dateStrTemp[3];
            Highscores[i].dateStr[1]=dateStrTemp[4];
            Highscores[i].dateStr[3]=dateStrTemp[0];
            Highscores[i].dateStr[4]=dateStrTemp[1];
            Highscores[i].dateStr[8]='\n';
            Highscores[i].dateStr[9]='\0';

            _strtime(Highscores[i].timeStr); //Hora no formato HHMMSS
            Highscores[i].timeStr[8]='\n';
            Highscores[i].timeStr[9]='\0';
            flag=0;
            flag2=1;
        }

        if(flag2)
        {
            textcolor(LILAS);
        }
        else
        {
            textcolor(BRANCO);
        }
        gotoXY(12, i+10);
        printf("%d",Highscores[i].posicao);
        gotoXY(19,i+10);
        printf("%s", Highscores[i].nome);
        gotoXY(50,i+10);
        printf("%5d", Highscores[i].pontos);
        gotoXY(64,i+10);
        printf("%s", Highscores[i].dateStr);
        gotoXY(80,i+10);
        printf("%s", Highscores[i].timeStr);
        flag2=0;
    }
    textcolor(BRANCO);
    gotoXY(32,20);
    printf("______________________________________");
    gotoXY(32,21);
    printf("Press any key to close the game window");

    //ABRE arquivo para GRAVAÇÃO
    arq = fopen(url, "wt");
    if (arq == NULL) // Se nào conseguiu criar
    {
        printf("Problemas na CRIACAO do arquivo\n");
        return;
    }
    else //Grava os novos dados no arquivo
    {
        for(i=0; i<10; i++)
        {

            fprintf(arq,"%d\n",Highscores[i].posicao);
            fprintf(arq,"%s",Highscores[i].nome);
            fprintf(arq,"%d\n",Highscores[i].pontos);
            fprintf(arq,"%s",Highscores[i].dateStr);
            fprintf(arq,"%s",Highscores[i].timeStr);


        }
    }
    fclose(arq);

    while(flag3) //While para ficar trocando as cores do banner
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
            flag3=0;
        }

        Sleep(150);
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



//Movimentação e impressão do PacMan na posição correta
void movePacman(void)
{

    gotoXY(pacman.x, pacman.y); //Apaga a posição atual do pacman
    printf(" ");

    switch(pacman.next.coordinates) //Calcula a proxima posição do pacman
    {
    case 'y':
        pacman.y+=pacman.next.aumenta_diminui;
        break;
    case 'x':
        pacman.x+=pacman.next.aumenta_diminui;
        break;
    }

    testColision(); //Caso comando coloque o pacman dentro de uma parede, tira ele de lá
    testLimits(); //Caso tenha chegado nos limites do mapa, coloca pacman no outro lado

    //Imprime a nova posição do pacman
    textcolor(AMARELO);
    gotoXY(pacman.x,pacman.y);
    printf("C");

    return;
}

//Checagem das pastilhas
void checkPacDots(int* pacdots, int* points)
{

    if(lab[pacman.y-1][pacman.x-1]=='o')
    {
        lab[pacman.y-1][pacman.x-1]=' ';
        *points+=10; //Aumenta contador de pontos
        *pacdots+=1; //Incrementa 1 no contador de pastilhas comidas
        //Beep(500, 30); //- Comentado até sabermos se utilizaremos isso ou não


        textcolor(BRANCO);
        gotoXY(36, 32);
        printf("Points: %5d", *points);
    }

}

//Checagem das super-pastilhas
void checkPowerPellets(int* points)
{

    if(lab[pacman.y-1][pacman.x-1]=='*')
    {
        lab[pacman.y-1][pacman.x-1]=' ';
        *points+=50;
        Beep(1000, 50);
        pacman.pacDotActive=5000/NORMAL_SPEED; //Fica mais rapido pela quantidade de clocks possiveis em 5 segundos

        textcolor(BRANCO);
        gotoXY(36, 32);
        printf("Points: %5d", *points);
    }

    //Alteração da velocidade, e timer da duração do super-poder
    if(pacman.pacDotActive>0)
    {
        pacman.pacDotActive--;
        textcolor(BRANCO);
        gotoXY(29,33);
        printf("Tempo restante de invulnerabilidade: %4dms", pacman.pacDotActive*NORMAL_SPEED);
    }
    else
    {
        gotoXY(29,33);
        printf("                                             ");
    }
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
void testColision(void)
{

    if(lab[pacman.y-1][pacman.x-1]=='#' && (pacman.y>TOP-1 && pacman.y<HEIGHT+1) && (pacman.x>LEFT-1 && pacman.x<WIDTH+1)) //Caso esteja dentro de um campo 'parede' E dentro do mapa
    {
        switch(pacman.next.coordinates) //Volta a ultima posição andada
        {
        case 'y':
            pacman.y-=pacman.next.aumenta_diminui;
            break;
        case 'x':
            pacman.x-=pacman.next.aumenta_diminui;
            break;
        }

        //Faz pacman continuar andando
        switch(pacman.last.coordinates)
        {
        case 'y':
            pacman.y+=pacman.last.aumenta_diminui;
            break;
        case 'x':
            pacman.x+=pacman.last.aumenta_diminui;
            break;
        }


        if(lab[pacman.y-1][pacman.x-1]=='#') //Caso ja esteja em uma esquina, e seja forçado a entrar na parede, faz não entrar nela
        {
            switch(pacman.last.coordinates)
            {
            case 'y':
                pacman.y-=pacman.last.aumenta_diminui;
                break;
            case 'x':
                pacman.x-=pacman.last.aumenta_diminui;
                break;
            }
        }
    }

    else //Senão, confirma que ocorreu um movimento valido, e seta a ultima posição para ser igual a atual, para funcionar a proxima iteração
    {
        pacman.last.coordinates=pacman.next.coordinates;
        pacman.last.aumenta_diminui=pacman.next.aumenta_diminui;
    }

    return;
}

//Seta direção que o PacMan irá seguir
void setDirection(char key, int* continua)
{

    switch(key) //Verifica para onde será a nova direção
    {
    case 'w':
        pacman.next.coordinates='y'; //Seta direção no eixo y
        pacman.next.aumenta_diminui=-1; //Seta direção negativa
        break;
    case 'x':
        pacman.next.coordinates='y';
        pacman.next.aumenta_diminui=1; //Seta direção positiva
        break;
    case 'a':
        pacman.next.coordinates='x'; //Seta direção no eixo x
        pacman.next.aumenta_diminui=-1;
        break;
    case 'd':
        pacman.next.coordinates='x';
        pacman.next.aumenta_diminui=1;
        break;
    case 's':
        pacman.next.coordinates='s';
        pacman.next.aumenta_diminui=0;
        break;
    case 'p': //Pausa o jogo ao pressionar 'P'
        gamePause();
        break;
    case ' ':
        *continua=0; //Irá fazer o programa terminar sua execução
        pacman.next.coordinates='s'; //Faz pacman parar sua movimentação
        pacman.next.aumenta_diminui=0;
        break;
    }

}



//Apresenta menu de inicio do jogo
void startMenu(void)
{
    int contador=0;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/pacmanStart.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    gotoXY(26, contador+5);
    Sleep(50);
    while( (ch=fgetc(arq))!=EOF)   //Impressão do PacMan
    {
        if(contador>19)
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
            Sleep((contador-25)*-6); //Acelera a impressão de cada linha aos poucos
        }
    }

    getch();
    system("cls");
    return;
}

//Mensagem que aparece no inicio do jogo
int startMessage(int flag)
{

    if (flag)  //Mensagem de inicio
    {
        switch(pacman.lives)
        {
        case 2:
            textcolor(BRANCO);
            gotoXY(39,14);
            printf("                         ");
            gotoXY(39,15);
            printf(" Press any key to start  ");
            gotoXY(39,16);
            printf("                         ");
            break;
        case 1:
            textcolor(BRANCO);
            gotoXY(39,14);
            printf("                          ");
            gotoXY(39,15);
            printf(" You've died! Be careful! ");
            gotoXY(39,16);
            printf(" Press any key to restart ");
            gotoXY(39,17);
            printf("                          ");
            break;
        case 0:
            textcolor(BRANCO);
            gotoXY(39,14);
            printf("                          ");
            gotoXY(39,15);
            printf(" This is your LAST chance ");
            gotoXY(39,16);
            printf(" Press any key to restart ");
            gotoXY(39,17);
            printf("                          ");
            break;
        }
    }
    else
    {
        reconstructMaze(13,17,38,70);
        flag--;
    }

    return flag;
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
                textcolor(BRANCO);
                printf("%c", lab[contador][count]);
            }
            else if(lab[contador][count]=='o')
            {
                textcolor(AMARELO);
                printf("%c", lab[contador][count]);
            }
            else
            {
                printf(" ");
            }

        }
    }

    textcolor(AMARELO);
    gotoXY(pacman.x, pacman.y);
    printf("C");

    return;
}

void printTop10(void)
{
    int i=3;
    char ch;
    FILE *arq; //Cria um ponteiro para um tipo arquivo

    arq = fopen("data/top10.txt", "r"); //Abre arquivo pacman.txt onde temos a imagem do PacMan

    gotoXY(4, i);
    i++;
    while( (ch=fgetc(arq))!=EOF)   //Impressão do TOP10
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

// Função gotoXY, tem limite igual a: system("mode 'x-1', 'y-1'");
// índice inicia em 0
void gotoXY(int x, int y)
{
    COORD coord;
    coord.X = x-1;
    coord.Y = y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Selects cursor appearance.
// Sets the cursor type to
//
// _NOCURSOR	Turns off the cursor
// _NORMALCURSOR	Normal underscore cursor
// _SOLIDCURSOR	Solid block cursor
void cursorType(int cursor)
{
    CONSOLE_CURSOR_INFO cursorInfo;

    switch(cursor)
    {
    case 0:
        cursorInfo.dwSize = 100;
        cursorInfo.bVisible = FALSE;
        break;
    case 1:
        cursorInfo.dwSize = 100;
        cursorInfo.bVisible = TRUE;
        break;
    case 2:
        cursorInfo.dwSize = 10;
        cursorInfo.bVisible = TRUE;
        break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}




/*
TODO LIST:

• CMD
  - COMPLETO!!!

• PACMAN
  - COMPLETO!!

• FANTASMAS
  - Timer para ressuscitar
  - Trocar 1500/115 por 1500/NORMAL_SPEED quando conseguir colocar NORMAL_SPEED constante global

• PASTILHAS
  - COMPLETO!!!

• SUPER-PASTILHAS
  - COMPLETO!!!

• EXTRAS:
  Dijkshtra - Sistema de detecção de caminho dos fantasmas mais inteligente, utilizando grafos
  HIGHSCORES -> FEITO
  EFEITOS SONOROS -> Super-pastilhas OK
                     Morte PACMAN
                     Vitoria PACMAN
  Adicionar Cheat no F9 (Desativa deteccção de colisão com as paredes - Paredes valem 10000 pontos)
  Menu inicial, com seleções de mapas e/ou dificuldades + créditos

  FIM DO ARQUIVO
*/
