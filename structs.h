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
    coord origin;
    coord mov;
    char key;
    int alive;
    int reviveTime;
    clock_t deathTime;
}typedef ghost; //Stores ONE ghost Information

struct
{
    int quant;
    ghost unid[MAX_GHOSTS];
} typedef ghostsInfo; //Ghosts Group

struct
{
    char coordinates; //'x', 'y' ou 's'
    int up_down; //1 - Grows; -1 - Decreases; 0 - stop
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
    int position;
    char name[30];
    int points;
    clock_t duration;
    char dateStr[15]; //Date
    char timeStr[15]; //Time
} typedef SCORE; //Struct which stores highscore data

struct
{
    char name[30];
    int points;
} typedef ACTUAL_GAME; //Struct which stores actual player name, and its points

#endif //STRUCTS_H_
