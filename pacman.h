#ifndef PACMAN_FUNCTIONS_H_
#define PACMAN_FUNCTIONS_H_

int pacmanControl(int*, int*, pacmanInfo*, clock_t*, char[HEIGHT][WIDTH], ghostsInfo*, int, int); ///Pacman controller
void movePacman(pacmanInfo*, char[HEIGHT][WIDTH], int*, int); ///Moves the pacman
void testLimits(pacmanInfo*); ///Test map limits
void testColision(pacmanInfo*, char[HEIGHT][WIDTH]); ///Test collision with walls
void testWalls(pacmanInfo*, char[HEIGHT][WIDTH], int*);
void setDirection(char, int*, pacmanInfo*, int*); ///Choose the right direction of the pacman

#endif // PACMAN_FUNCTIONS_H_
