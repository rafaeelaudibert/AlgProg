//Headers includes
#include "main.h"
#include "structs.h"
#include "labirinto.h"
#include "pacman.h"
#include "ghosts.h"
#include "objects.h"
#include "messages.h"
#include "auxiliars.h"

//Variaveis globais

coord dir[4]; //Possíveis posições -> UP, RIGHT, DOWN, LEFT

//Ghost Controller
int ghostsControl(int *points, pacmanInfo pacman, clock_t *ghostsTime, char lab[HEIGHT][WIDTH], ghosts *fantasmas)
{

    float correcaoVelocidade = pacman.last.coordinates=='y' ? 1.4 : 1;
    clock_t actualTime=clock();

    if((actualTime - *ghostsTime) > ( pacman.pacDotActive ? SLOW_SPEED*correcaoVelocidade : NORMAL_SPEED*correcaoVelocidade))
    {
        *ghostsTime = actualTime;
        moveGhost(pacman, lab, fantasmas); // update of the moviment of the ghosts
        showGhosts(pacman, lab, fantasmas); // update and show the position of the ghosts

        if(checkGhostCollision(pacman, points, fantasmas) && !pacman.pacDotActive)
        {
            return 0;
        }
    }
    return 1;
}

// inicializa os elementos do array global dir
void setupDir()
{
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
void shuffleDir()
{
    coord t; // auxiliar
    int i, j=0;
    for(i=0; i<4; i++)
    {
        j = rand() % 4;
        t = dir[j];
        dir[j] = dir[i];
        dir[i] = t;
    }
}

// algoritmo pra movimentar cada um dos fantasmas
void moveGhost(pacmanInfo pac, char lab[30][100], ghosts *fantasmas)
{

    int i, q = fantasmas->quant; //fantasmas.quant;
    // passa por cada um dos fantasmas
    for(i=0; i<q; i++)
    {
        // fantasma do teste atual
        ghost g = fantasmas->unid[i];

        // chance do fantasma ir atr�s do pacman
        int chance = rand() % MAX_RANDOM;

        // se o fantasma precisa decidir se muda de dire��o
        if(mudarDirecao(g, lab) == 1)
        {
            // se o valor de perseguir pacman est� dentro da chance_persegui��o, persegue
            if(chance < abs(CHASE_CHANCE-10))
            {

                // testa se chegou em um canto com apenas uma sa�da
                if(ladosLivres(g, lab) == 1)
                {
                    fantasmas->unid[i].mov.y = fantasmas->unid[i].mov.y * -1;
                    fantasmas->unid[i].mov.x = fantasmas->unid[i].mov.x * -1;
                }
                else
                {
                    // escolhe uma dire��o diferente da que veio, aleatoriamente
                    // mistura aleatoriamente o vetor de poss�veis dire��es
                    shuffleDir();
                }
            }
            else
            {
                // escolhe o caminho que mais diminui a dist�ncia em rela��o ao pacman
                // ordena o vetor das possiveis dire��es de acordo com a dist�ncia
                // cartesiana do ghost para o pacman
                perseguePacman(pac, g, lab);
            }
            escolheDirecao(pac, &(fantasmas->unid[i]), lab);
        }
    }
}

// mostra os fantasma na tela
void showGhosts(pacmanInfo pac, char lab[30][100], ghosts *fantasmas)
{
    int i;
    for(i=0; i<fantasmas->quant; i++)
    {
        // reprinta a última posicao com o que continha
        coord posg;
        posg.x = fantasmas->unid[i].pos.x;
        posg.y = fantasmas->unid[i].pos.y;
        gotoXY(posg.x+1, posg.y+1);
        // cores para impresão
        if(lab[posg.y][posg.x] =='o')
        {
            textcolor(AMARELO);
        }
        else
        {
            textcolor(VERMELHO);
        }
        printf("%c", lab[posg.y][posg.x]);

        // atualiza a posicao
        fantasmas->unid[i].pos.x += fantasmas->unid[i].mov.x;
        fantasmas->unid[i].pos.y += fantasmas->unid[i].mov.y;

        testaLimites(&fantasmas->unid[i]);

        if(fantasmas->unid[i].alive)
        {
            // print na tela a nova posicao
            if(pac.pacDotActive>(2000/NORMAL_SPEED))
            {
                textcolor(VERDE_AGUA);
            }
            else
            {
                if(pac.pacDotActive%2)
                {
                    textcolor(VERDE_AGUA);
                }
                else
                {
                    textcolor(LILAS);
                }
            }
            gotoXY(fantasmas->unid[i].pos.x+1, fantasmas->unid[i].pos.y+1);
            printf("W");
        }
    }
    // volta a cor normal
    textcolor(BRANCO);
}

// verifica quais são as possíveis direções que o fantasma pode ir
// e escolhe uma de acordo com a ordem de prefêrencia no array dir[].
void escolheDirecao(pacmanInfo pac, ghost *pg, char lab[30][100])
{
    int i, d; // índice, limite, incremento
    ghost g = *pg;

    // passa por cada uma das possiveis direcoes que ele pode ir
    for(i=0; i<4; i++)
    {
        // por qual lado deve ser a leitura do array
        // crescente se o fantasma estiver perseguindo o pacman, drescente se estiver fugindo
        if(pac.pacDotActive != 0)
        {
            d = i;
        }
        else
        {
            d = 3 - i;
        }
        // verifica para quais lados nao sao paredes e se ele nao passa do limite do mapa com a nova dir
        if( lab[ (g.pos.y + dir[d].y) ][ (g.pos.x + dir[d].x) ] != '#')
        {
            // verifica se a nova direcao nao eh a que ele vinha, para nao retornar pelo mesmo caminho
            if( dir[d].x != (g.mov.x * -1) ||
                    dir[d].y != (g.mov.y * -1) )
            {
                // modifica direto no fantasma a nova dire��o e sai do la�o
                pg->mov = dir[d];
                d = 5;
            }
        }
    }
}

// verifica se o fantasma está em um local que é uma bifurcacao ou um canto
// se precisa mudar sua direcao
// retornos:  1 : se estiver e precisar decidir se muda de direcao ou se continua
//            0 : nao muda de direcao
int mudarDirecao(ghost g, char lab[30][100])
{
    // verifica em qual dire��o o fantasma est� indo
    if( g.mov.x != 0)
    {
        // dire��o na horizontal, verifica se pode ir pra cima ou pra baixo
        if( lab[g.pos.y+1][g.pos.x] != '#' || lab[g.pos.y-1][g.pos.x] != '#') return 1;
    }
    else
    {
        // dire��o na vertical, verifica se pode ir pra esquerda ou direita
        if( lab[g.pos.y][g.pos.x+1] != '#' || lab[g.pos.y][g.pos.x-1] != '#') return 1;
    }
    return 0;
}

// devolve um array de posiçoes ordenados pela distancia cartesiana
// crescente do fantasma em direcao ao Pacman.
void perseguePacman(pacmanInfo pac, ghost g, char lab[30][100])
{
    int d, i;

    // vetor das dist�ncias em cada uma das novas posi��es poss�veis
    int dists[4];
    for(i=0; i<4; i++)
    {
        // calculo da distância cartesiana
        dists[i] = pow( pac.pos.y - g.pos.y + dir[i].y, 2) + pow(pac.pos.x - g.pos.x + dir[i].x, 2);
    }

    // ordena o vetor das distancias e o das direcoes de acordo com o das distancias
    for(d=0; d<4; d++)
    {
        for(i=0; i<4; i++)
        {
            if(d != i)
            {
                if(dists[d] > dists[i])
                {
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

// retorna quantos lados est�o livres
int ladosLivres(ghost g, char lab[30][100])
{
    int soma = 0;
    // testa cada uma das dire��es se est� livre
    if( lab[g.pos.y][ g.pos.x + 1] != '#') soma++;
    if( lab[g.pos.y][ g.pos.x - 1] != '#') soma++;
    if( lab[ g.pos.y + 1][g.pos.x] != '#') soma++;
    if( lab[ g.pos.y - 1][g.pos.x] != '#') soma++;
    return soma;
}

// dentro dos limites do mapa, retorna 1, se não retorna 0;
int testaLimites(ghost *g)
{
    if( g->pos.x <= 0 || g->pos.x >= (WIDTH-1) )
    {
        g->pos.x = WIDTH - g->pos.x -1;
    }
    else if(g->pos.y <= 0 || g->pos.y >= (HEIGHT-1) )
    {
        g->pos.y = HEIGHT - g->pos.y -1;
    }
    else
    {
        return 0;
    }
    return 1;
}

int checkGhostCollision(pacmanInfo pac, int *points, ghosts *fantasmas)
{
    int i;

    for(i=0; i<fantasmas->quant; i++)
    {
        ghost g = fantasmas->unid[i];
        if(g.alive && pac.pos.x-1 == g.pos.x && pac.pos.y-1 == g.pos.y)
        {
            if(pac.pacDotActive)
            {
                fantasmas->unid[i].alive = 0;
                (*points)+=200;

                textcolor(BRANCO);
                gotoXY(36, 32);
                printf("Points: %5d", *points);
            }
            return 1;
        }
    }
    return 0;
}
