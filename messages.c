//Headers includes
#include "main.h"
#include "structs.h"
#include "labirinto.h"
#include "pacman.h"
#include "ghosts.h"
#include "objects.h"
#include "messages.h"
#include "auxiliars.h"

void highscores(int points, clock_t duracao)
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

            fgets(Linha, 10, arq);
            Highscores[i].duracao=atoi(Linha)*CLOCKS_PER_SEC;

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
    printf("      POSICAO   NOME                         PONTUACAO       TEMPO       DATA            HORA");


    //Manipulação dos dados, e printagem do Ranking
    for(i=0; i<10; i++)
    {
        if(Highscores[i].pontos==dados.pontos && Highscores[i].duracao>duracao && flag)
        {
            for(j=9; j>=i; j--)
            {
                Highscores[j].pontos=Highscores[j-1].pontos;
                Highscores[j].duracao=Highscores[j-1].duracao;
                strcpy(Highscores[j].nome, Highscores[j-1].nome);
                strcpy(Highscores[j].dateStr, Highscores[j-1].dateStr);
                strcpy(Highscores[j].timeStr, Highscores[j-1].timeStr);
            }
            Highscores[i].pontos=dados.pontos;
            strcpy(Highscores[i].nome, dados.nome);
            Highscores[i].duracao=duracao;

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
        }else if(Highscores[i].pontos<dados.pontos && flag){
            for(j=9; j>=i; j--)
            {
                Highscores[j].pontos=Highscores[j-1].pontos;
                Highscores[j].duracao=Highscores[j-1].duracao;
                strcpy(Highscores[j].nome, Highscores[j-1].nome);
                strcpy(Highscores[j].dateStr, Highscores[j-1].dateStr);
                strcpy(Highscores[j].timeStr, Highscores[j-1].timeStr);
            }
            Highscores[i].pontos=dados.pontos;
            strcpy(Highscores[i].nome, dados.nome);
            Highscores[i].duracao=duracao;

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
        gotoXY(10, i+10);
        printf("%d",Highscores[i].posicao);
        gotoXY(17,i+10);
        printf("%s", Highscores[i].nome);
        gotoXY(48,i+10);
        printf("%5d", Highscores[i].pontos);
        gotoXY(60,i+10);
        printf("%5ld",Highscores[i].duracao/CLOCKS_PER_SEC);
        gotoXY(72,i+10);
        printf("%s", Highscores[i].dateStr);
        gotoXY(88,i+10);
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
            fprintf(arq,"%ld\n",Highscores[i].duracao/CLOCKS_PER_SEC);
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
int startMessage(int flag, pacmanInfo pacman, char lab[HEIGHT][WIDTH])
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
        reconstructMaze(13,17,38,70, lab, pacman);
        flag--;
    }

    return flag;
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
