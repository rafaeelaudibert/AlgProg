#ifndef STRUCTS_H_
#define STRUCTS_H_

struct
{
    int x;
    int y;
} typedef coord; //Stores a coordinate (x,y)

struct
{
    coord pos;
    int alive;
    coord mov; //Where the ghost is going -> UP, RIGHT, DOWN, LEFT
} typedef ghost; //Stores ONE ghost Information

struct
{
    int quant;
    ghost unid[5];
} typedef ghosts; //Ghosts Group

struct
{
    char coordinates; //'x', 'y' ou 's'
    int aumenta_diminui; //1 - Aumenta; -1 - Diminui; 0 - parado
} typedef directions; //Stores axis and direction in it

struct
{
    coord pos;
    coord origin;
    int lives; //lives remaining
    int pacDotActive; //SuperPower
    directions next; //Next direction to walk
    directions last; //Last direction walked
} typedef pacmanInfo; //Stores pacman information

struct
{
    int posicao;
    char nome[30];
    int pontos;
    char dateStr[15]; //Date
    char timeStr[15]; //Time
} typedef PLACAR; //Struct que guarda valores do HighScore

struct
{
    char nome[30];
    int pontos;
} typedef JOGO_ATUAL; //Struct que guarda nome do jogador atual e sua pontuação

#endif //STRUCTS_H_
