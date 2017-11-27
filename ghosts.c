//Headers includes
#include "main.h"
#include "structs.h"
#include "ghosts.h"
#include "objects.h"
#include "auxiliars.h"

//Variaveis globais

 //Available directions -> UP, RIGHT, DOWN, LEFT
coord dir[4]={{0,-1},{1,0},{0,1},{-1,0}};

///Ghost Controller
int ghostsControl(int *points, pacmanInfo pacman, clock_t *ghostsTimer, char lab[HEIGHT][WIDTH], ghosts *fantasmas, int speed, int chase_chance)
{

    float speedCorrection = pacman.last.coordinates=='y' ? 1.4 : 1; //Corrects the speed of the game, as the pacman's movement axis changes
    clock_t actualTime=clock();

    if((actualTime - *ghostsTimer) > ( pacman.pacDotActive ? speed*1.3*speedCorrection : speed*speedCorrection))
    {
        *ghostsTimer = actualTime;
        moveGhost(pacman, lab, fantasmas, chase_chance); // Updates the movement of the ghosts
        showGhosts(pacman, lab, fantasmas, speed); // Updates and shows the position of the ghosts

        if(checkGhostCollision(pacman, points, fantasmas) && !pacman.pacDotActive) //If there is collision between a ghost and the pacman
        {
            return 0;
        }
    }
    return 1;
}

///Unsorts the array of directions, randomically
void shuffleDir()
{
    coord t; //Auxiliar
    int i, j=0;
    for(i=0; i<4; i++)
    {
        j = rand() % 4;
        t = dir[j];
        dir[j] = dir[i];
        dir[i] = t;
    }
}

///Moves each one of the ghosts
void moveGhost(pacmanInfo pac, char lab[30][100], ghosts *fantasmas, int chase_chance)
{

    int i, q = fantasmas->quant; //fantasmas.quant;
    // Iterates on each ghost
    for(i=0; i<q; i++)
    {
        // Actual test ghost
        ghost g = fantasmas->unid[i];

        // Ghost's chance of stalking the Pacman
        int chance = rand() % MAX_RANDOM;

        //Decisions to change direction
        if(ladosLivres(g, lab) == 1) //Only one exit, inverts the movement direction
        {
            fantasmas->unid[i].mov.y = fantasmas->unid[i].mov.y * -1;
            fantasmas->unid[i].mov.x = fantasmas->unid[i].mov.x * -1;
        }
        else if(mudarDirecao(g, lab) == 1)
        {
            // se o valor de perseguir pacman est� dentro da chance_persegui��o, persegue
            if(chance < abs(chase_chance-10))
            {

                // escolhe uma dire��o diferente da que veio, aleatoriamente
                // mistura aleatoriamente o vetor de poss�veis dire��es
                shuffleDir();
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
void showGhosts(pacmanInfo pac, char lab[30][100], ghosts *fantasmas, int speed)
{
    int i;
    for(i=0; i < fantasmas->quant; i++)
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
            printf("%c", PACDOT);
        }
        else if(lab[posg.y][posg.x] =='*')
        {
            textcolor(VERMELHO);
            printf("%c", POWERPELLET);
        }else{
            printf(" ");
        }

        // atualiza a posicao
        if(fantasmas->unid[i].alive==1)
        {
            fantasmas->unid[i].pos.x += fantasmas->unid[i].mov.x;
            fantasmas->unid[i].pos.y += fantasmas->unid[i].mov.y;
        }

        testaLimites(&fantasmas->unid[i]);

        if(fantasmas->unid[i].alive==1)
        {
            // print na tela a nova posicao
            if(pac.pacDotActive>(2000/speed))
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
            printf("%c", fantasmas->unid[i].key);
        }
        else if(fantasmas->unid[i].alive==2)
        {
            if (!(fantasmas->unid[i].reviveTime%2))
            {
                textcolor(PRETO);
            }
            else
            {
                textcolor(LILAS);
            }
            fantasmas->unid[i].reviveTime--;

            if(!(fantasmas->unid[i].reviveTime))
            {
                fantasmas->unid[i].alive=1;
            }

            gotoXY(fantasmas->unid[i].pos.x+1, fantasmas->unid[i].pos.y+1);
            printf("%c", fantasmas->unid[i].key);
        }
    }
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
int testaLimites(ghost *fantasma)
{
    if (fantasma->pos.x < 0)
    {
        fantasma->pos.x = WIDTH-1;
    }
    else if(fantasma->pos.x > (WIDTH-1))
    {
        fantasma->pos.x=0;
    }
    else if(fantasma->pos.y < 0)
    {
        fantasma->pos.y = HEIGHT-1;
    }
    else if(fantasma->pos.y > (HEIGHT-1))
    {
        fantasma->pos.y = 0;
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
        if(g.alive==1 && pac.pos.x-1 == g.pos.x && pac.pos.y-1 == g.pos.y)
        {
            if(pac.pacDotActive)
            {
                fantasmas->unid[i].alive = 0;
                fantasmas->unid[i].deathTime = clock();
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

/// Revive o primeiro fantasma que encontrar morto
void reviveGhosts(ghosts *ghosts, int speed)
{

    int i;

    for(i=0; i < ghosts->quant; i++)
    {
        if( !ghosts->unid[i].alive && (( clock() - ghosts->unid[i].deathTime) > RESPAWN) )
        {
            ghosts->unid[i].pos = ghosts->unid[i].origin;
            ghosts->unid[i].reviveTime=4000/speed; //4 seconds blinking
            ghosts->unid[i].alive = 2;
            return;
        }
    }
    return;
}
