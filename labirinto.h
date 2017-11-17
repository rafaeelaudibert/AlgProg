#ifndef COMMANDS_H_
#define COMMANDS_H_


struct // salva uma posição x,y
{
    int x;
    int y;
} typedef coord;

struct // fantasmas
{
    int x;
    int y;
    coord mov; // para onde o ghost está indo
    // UP, RIGHT, DOWN, LEFT
    int alive;
} typedef ghost;

struct // conjunto dos fantasmas
{
    int quant;
    ghost unid[5];
} typedef ghosts;


//Structs
struct //Stores axis and direction in it
{
    char coordinates; //'x', 'y' ou 's'
    int aumenta_diminui; //1 - Aumenta; -1 - Diminui; 0 - parado
} typedef directions;

struct //Stores pacman position, and its information
{
    int x; //x-axis position
    int y; //y-axis position
    int lives; //l=Lives remaining
    int pacDotActive; //SuperPower
    directions next; //Next direction to walk
    directions last; //Last direction walked
} typedef pacmanInfo;

// Cabeçalho das funções

// Função que lê o labirinto do endereco e coloca na matriz lab
int readLab(char lab[30][100]);

// Função que imprime o lab original
int showLab(char lab[30][100], int*, int*, int*);

// Carrega o labirinto e carrega as structs
// void iniciaLabirinto(char lab[30][100]);

// Fun��es dos fantasmas
void setupDir(); // seta as direcoes para o fantasma
void shuffleDir(); // random Sort of the dir[]
int iniciaLabirinto(char [30][100], int*, pacmanInfo); // seta as posições iniciais de tudo
void moveGhost(pacmanInfo, char[30][100]); // movimenta os fantasmas
void showGhosts(pacmanInfo, char[30][100]); // show the labirinto
void escolheDirecao(pacmanInfo, ghost*, char[30][100]); // dire��o v�lida aleat�ria do fantasma
int mudarDirecao(ghost, char[30][100]);

// altera o array dir, colocando as direções em ordem crescente de distância do ghost
void perseguePacman(pacmanInfo, ghost, char[30][100]);

// devolve para quantos lados o pacman pode ir
int ladosLivres(ghost, char[30][100]);
// se está fora dos limites do mapa
int testaLimites(ghost *g);

//Fun��es que retornam posi��o do pacman para as fun��es do arquivo main.c (Bugadas no momento)
int retornaXPacman(void);
int retornaYPacman(void);

// interações do pacman com os ghots
int checkGhostCollision(pacmanInfo, int*);

// dist�ncia cartesiana entre 2 pontos xy
//int distancia_cart(coord, coord);

// 'conio'
void gotoxy2(int, int);

#endif
