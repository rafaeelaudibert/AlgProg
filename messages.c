//Headers includes
#include "main.h"
#include "structs.h"
#include "messages.h"
#include "auxiliars.h"

void highscores(int points, clock_t duracao)
{
    PLACAR Highscores[11];
    JOGO_ATUAL dados= {{'a','\0'}, -50};
    FILE *arq;
    char url[25]= {"data/Highscores.txt"};

    // Abre um arquivo TEXTO para LEITURA
    arq = fopen(url, "rt");
    leituraArquivo(arq, Highscores);
    fclose(arq);

    system("cls");
    textcolor(BRANCO);
    gotoXY(1,1);
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
    manipulacaoDados(Highscores, dados, duracao);

    //GRAVAÇÃO do ARQUIVO já editado
    arq = fopen(url, "wt");
    gravacaoHighscore(arq, Highscores);
    fclose(arq);

    colorHeader(); //Makes the header colorful, and switching colors

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
            gotoXY(37,13);
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

void gravacaoHighscore(FILE* arq, PLACAR Highscores[10])
{

    int i;

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

    return;
}

void colorHeader(void)
{


    int k=AZUL, flag=1;

    while(flag) //While para ficar trocando as cores do banner
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

void leituraArquivo(FILE* arq, PLACAR Highscores[10])
{

    int i;
    char Linha[100];

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
}

void manipulacaoDados(PLACAR Highscores[10], JOGO_ATUAL dados, clock_t duracao)
{

    int i,j, flagRankingPosition=1, flagColor=0;
    char dateStrTemp[9];

    cursorType(CURSOR); //Faz cursor desaparecer novamente
    system("cls"); //Limpa a tela

    //Mensagem inicial
    textcolor(BRANCO);
    gotoXY(1,8);
    printf("      POSICAO   NOME                         PONTUACAO       TEMPO       DATA            HORA");

    //Manipulação dos dados
    for(i=0; i<10; i++)
    {
        if(Highscores[i].pontos==dados.pontos && Highscores[i].duracao>duracao && flagRankingPosition)
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
            flagRankingPosition=0;
            flagColor=1;
        }
        else if(Highscores[i].pontos<dados.pontos && flagRankingPosition)
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
            flagRankingPosition=0;
            flagColor=1;
        }

        if(flagColor)
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
        flagColor=0;
    }

    textcolor(BRANCO);
    gotoXY(32,20);
    printf("______________________________________");
    gotoXY(32,21);
    printf("Press any key to close the game window");

    return;
}
