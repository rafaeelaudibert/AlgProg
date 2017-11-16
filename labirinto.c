//Libraries inclusion
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <math.h>
#include <time.h>

// Local Headers Inclusion
#include "structs.h"
#include "labirinto.h"

void gotoxy(int, int);
void textcolor(int);

//Definição das estruturas

//instâncias globais dos objetos
ghosts fantasmas, ghosts_origin;
pacmanInfo pacman_origin;

extern int HEIGHT;
extern int WIDTH;

// tamanho do mapa
int const WIDTH_SCREEN = 100;
int const HEIGHT_SCREEN = 30;

// chance de fantasma perseguir o pacman
int const chance_perseguicao = 6;
// função aleatória, gera de 0 a max_random
int const max_random = 10;

// possíveis posições
coord dir[4]; // UP, RIGHT, DOWN, LEFT

// inicia e carrega todas as estruturas
int iniciaLabirinto(char lab[HEIGHT][WIDTH], int *qtdePastilhas, pacmanInfo pacman){
    int i, j; //Contador do laço da matriz
    int q_fantasmas=0, q_pastilhas=0; //Contagem de objetos

    // standard setup of the array dir[];
    setupDir();

    //Chamada das outras duas funções responsáveis pelo labirinto
    if(readLab(lab))
    {
        return 1;
    }

    //Printagem do labirinto na tela
    gotoxy2(0,0);
    for(i=0; i<HEIGHT; i++)
    {
        for(j=0; j<WIDTH; j++)
        {
            switch(lab[i][j])
            {
            case 'c':
            case 'C':
                // printf(" ");
                // *pacmanx=j+1;
                // *pacmany=i+1;
                pacman_origin.x = j+1;
                pacman_origin.y = i+1;
                lab[i][j] = ' ';
                break;
            case 'w':
            case 'W': // Fantasmas
                // printf(" ");
                fantasmas.unid[q_fantasmas].x = j;
                fantasmas.unid[q_fantasmas].y = i;
                fantasmas.unid[q_fantasmas].alive = 1; // seta a vida do ghost
                // movimentação inicial aleatória
                //shuffleDir();
                //escolheDirecao(&fantasmas.unid[q_fantasmas], lab);

                q_fantasmas++;
                lab[i][j] = ' ';
                break;
            case 'o':
                q_pastilhas++;
                break;
            }
        }
    }

    fantasmas.quant=q_fantasmas;
    // seta e guarda a posição inicial dos fantasmas
    ghosts_origin = fantasmas;

    // seta e guarda a posição inicial do Pacman
    pacman.x = pacman_origin.x;
    pacman.y = pacman_origin.y;

    *qtdePastilhas=q_pastilhas; //Seta a quantidade total de pastilhas

    return 0;
}


// mostra o labirinto na tela
int showLab(char lab[HEIGHT][WIDTH], int *qtdePastilhas, int *pacmanx, int *pacmany)
{

    int i, j; //Contador do laço da matriz
    int q_fantasmas=fantasmas.quant;

    //Printagem do labirinto na tela
    gotoxy2(0,0);
    // reprinta todo o labirinto
    for(i=0; i<HEIGHT; i++)
    {
        for(j=0; j<WIDTH; j++)
        {
            switch(lab[i][j])
            {
            case 'c':
            case 'C':
                break;
            case 'w':
            case 'W': // Fantasmas
                break;
            case '#':
                textcolor(15); //BRANCO
                printf("#");
                break;
            case 'o':
                textcolor(14); //AMARELO
                printf("o");
                // q_pastilhas++;
                break;
            case '*':
                textcolor(12); //ROSA
                printf("*");
                break;
            default:
                printf(" ");
                break;
            }
        }
        printf("\n");
    }

    // seta a posição inicial do pacman
    *pacmanx= pacman_origin.x;
    *pacmany= pacman_origin.y;

    // seta os fantasmas para as posições iniciais
    for(i=0; i<q_fantasmas; i++){
        fantasmas.unid[q_fantasmas].x = ghosts_origin.unid[q_fantasmas].x;
        fantasmas.unid[q_fantasmas].y = ghosts_origin.unid[q_fantasmas].y;
        fantasmas.unid[q_fantasmas].alive = 1; // seta a vida do ghost
    }

    return 0;
}

//Função que lê o arquivo labirinto.txt e configura a matriz do labirinto
//Retorna 1 se houve algum erro
int readLab(char lab[HEIGHT][WIDTH])
{
    char str[101];
    int i=0, j=0;
    char url[30]="data/labirinto.txt"; //labirinto.txt para jogo normal, labirintoOneDot.txt para testes rapidos
    FILE *arq;

    //Ponteiro para leitura do arquivo no 'endereco'
    arq = fopen(url, "r");

    //Se não conseguiu ler o arquivo, sai e retorna 1
    if(arq == NULL)
    {
        printf("ERROR: nao foi possivel abrir o arquivo\n");
        return 1;
    }


    //Lê cada uma das linhas do arquivo
    while(fgets(str, 101, arq) != NULL || i<30)
    {
        //Se a leitura retornar uma linha em branco, ignora
        if(str[0] != '\n')
        {
            //Senão, adiciona à matriz lab uma linha
            for(j=0; j<100; j++)
            {
                lab[i][j] = str[j];
            }

            i++;
        }
    }

    return 0;
}

int respawn(){

}
// novo código
// algoritmo pra movimentar cada um dos fantasmas
void moveGhost(pacmanInfo pac, char lab[30][100]){

    int i, q = fantasmas.quant; //fantasmas.quant;
    // passa por cada um dos fantasmas
    for(i=0; i<q; i++){
        // fantasma do teste atual
        ghost g = fantasmas.unid[i];

        // chance do fantasma ir atr�s do pacman
        int chance = rand() % max_random;

        // se o fantasma precisa decidir se muda de dire��o
        if(mudarDirecao(g, lab) == 1){
            // se o valor de perseguir pacman est� dentro da chance_persegui��o, persegue
            if(chance < chance_perseguicao){

                // testa se chegou em um canto com apenas uma sa�da
                if(ladosLivres(g, lab) == 1){
                    fantasmas.unid[i].mov.y = fantasmas.unid[i].mov.y * -1;
                    fantasmas.unid[i].mov.x = fantasmas.unid[i].mov.x * -1;
                } else {
                    // escolhe uma dire��o diferente da que veio, aleatoriamente
                    // mistura aleatoriamente o vetor de poss�veis dire��es
                    shuffleDir();
                }
            } else {
                // escolhe o caminho que mais diminui a dist�ncia em rela��o ao pacman
                // ordena o vetor das possiveis dire��es de acordo com a dist�ncia
                // cartesiana do ghost para o pacman
                perseguePacman(pac, g, lab);
            }
            escolheDirecao(pac, &fantasmas.unid[i], lab);
        }
    }
}


// verifica se o fantasma está em um local que é uma bifurcacao ou um canto
// se precisa mudar sua direcao
// retornos:  1 : se estiver e precisar decidir se muda de direcao ou se continua
//            0 : nao muda de direcao
int mudarDirecao(ghost g, char lab[30][100]){
    // verifica em qual dire��o o fantasma est� indo
    if( g.mov.x != 0){
        // dire��o na horizontal, verifica se pode ir pra cima ou pra baixo
        if( lab[g.y+1][g.x] != '#' || lab[g.y-1][g.x] != '#') return 1;
    } else {
        // dire��o na vertical, verifica se pode ir pra esquerda ou direita
        if( lab[g.y][g.x+1] != '#' || lab[g.y][g.x-1] != '#') return 1;
    }
    return 0;
}

// verifica quais são as possíveis direções que o fantasma pode ir
// e escolhe uma de acordo com a ordem de prefêrencia no array dir[].
void escolheDirecao(pacmanInfo pac, ghost *pg, char lab[30][100]){
    int i, d; // índice, limite, incremento
    ghost g = *pg;

    // passa por cada uma das possiveis direcoes que ele pode ir
    for(i=0; i<4; i++)
    {
        // por qual lado deve ser a leitura do array
        // crescente se o fantasma estiver perseguindo o pacman, drescente se estiver fugindo
        if(pac.pacDotActive != 0){
            d = i;
        } else {
            d = 3 - i;
        }
        // verifica para quais lados nao sao paredes e se ele nao passa do limite do mapa com a nova dir
        if( lab[ (g.y + dir[d].y) ][ (g.x + dir[d].x) ] != '#' &&
                testaLimites(g, dir[d]) == 1 )
        {
            // verifica se a nova direcao nao eh a que ele vinha, para nao retornar pelo mesmo caminho
            if( dir[d].x != (g.mov.x * -1) ||
                dir[d].y != (g.mov.y * -1) ){
                    // modifica direto no fantasma a nova dire��o e sai do la�o
                    pg->mov = dir[d];
                    d = 5;
                }
        }
    }
}

// devolve um array de posiçoes ordenados pela distancia cartesiana
// crescente do fantasma em direcao ao Pacman.
void perseguePacman(pacmanInfo pac, ghost g, char lab[30][100]){
    int d, i;

    // vetor das dist�ncias em cada uma das novas posi��es poss�veis
    int dists[4];
    for(i=0; i<4; i++){
        // calculo da distância cartesiana
        dists[i] = pow( pac.y - g.y + dir[i].y , 2) + pow(pac.x - g.x + dir[i].x, 2);
    }

    // ordena o vetor das distancias e o das direcoes de acordo com o das distancias
    for(d=0; d<4; d++){
        for(i=0; i<4; i++){
            if(d != i){
                if(dists[d] > dists[i]){
                    int t = dists[d];
                    dists[d] = dists[i];
                    dists[i] = t;

                    coord temp = dir[d];
                    dir[d] = dir[i];
                    dir[i] = temp;
                }
            }
        }
    }
}

// mostra os fantasma na tela
void showGhosts(pacmanInfo pac, char lab[30][100]){
    int i;
    for(i=0; i<fantasmas.quant; i++){
        // reprinta a última posicao com o que continha
        coord posg;
        posg.x = fantasmas.unid[i].x;
        posg.y = fantasmas.unid[i].y;
        gotoxy2(posg.x, posg.y);
        // cores para impresão
        if(lab[posg.y][posg.x] =='o'){
            textcolor(14); //AMARELO
        } else if(lab[posg.y][posg.x]){
            textcolor(12); //VERMELHO
        }
        printf("%c", lab[posg.y][posg.x]);

        // atualiza a posicao
        fantasmas.unid[i].x += fantasmas.unid[i].mov.x;
        fantasmas.unid[i].y += fantasmas.unid[i].mov.y;

        if(fantasmas.unid[i].alive){
            // print na tela a nova posicao
            if(pac.pacDotActive){
            textcolor(11);
            }
            else{
                textcolor(13);
            } // muda a cor para roxo
            gotoxy2(fantasmas.unid[i].x, fantasmas.unid[i].y);
            printf("W");
        }
    }
    // volta a cor normal
    textcolor(15);
}

// setup inicial of ghosts
void randomSetup(){

}

// inicializa os elementos do array global dir
void setupDir(){
    dir[0].x = 0; // UP
    dir[0].y = -1;
    dir[1].x = 1; // RIGHT
    dir[1].y = 0;
    dir[2].x = 0; // DOWN
    dir[2].y = 1;
    dir[3].x = -1; // LEFT
    dir[3].y = 0;
}

// embaralha o array das direções ( dir ), aleatoriamente
void shuffleDir(){
    coord t; // auxiliar
    int i, j=0;
    for(i=0; i<4; i++){
        j = rand() % 4;
        t = dir[j];
        dir[j] = dir[i];
        dir[i] = t;
    }
}

// dentro dos limites do mapa, retorna 1
int testaLimites(ghost g, coord d){
    if( g.x + d.x < WIDTH_SCREEN-1 &&
        g.x + d.x > 0 &&
        g.y + d.y < HEIGHT_SCREEN-1 &&
        g.y + d.y > 0){
        return 1;
    } else {
        return 0;
    }
};

// retorna quantos lados est�o livres
int ladosLivres(ghost g, char lab[30][100]){
    int soma = 0;
    // testa cada uma das dire��es se est� livre
    if( lab[g.y][ g.x + 1] != '#') soma++;
    if( lab[g.y][ g.x - 1] != '#') soma++;
    if( lab[ g.y + 1][g.x] != '#') soma++;
    if( lab[ g.y - 1][g.x] != '#') soma++;
    return soma;
}

// gotoxy2
// move the cursor to position (x, y)
// begin in 0.
void gotoxy2(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int checkGhostCollision(pacmanInfo pac){
    int i;

    for(i=0; i<fantasmas.quant; i++){
        ghost g = fantasmas.unid[i];
        if(g.alive && pac.x-1 == g.x && pac.y-1 == g.y){
            if(pac.pacDotActive){
                fantasmas.unid[i].alive = 0;
            } else {
                return 1;
            }

        }
    }
    return 0;
}

int eatGhost(pacmanInfo pac, int *points){
    int i;

    for(i=0; i<fantasmas.quant; i++){
        ghost g = fantasmas.unid[i];
        if(g.alive && pac.x-1 == g.x && pac.y-1 == g.y){
            fantasmas.unid[i].alive = 0;
            *points = *points + 100;
            return 1;
        }
    }
    return 0;
}
