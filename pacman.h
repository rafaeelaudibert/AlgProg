#ifndef PACMAN_FUNCTIONS_H_
#define PACMAN_FUNCTIONS_H_

int pacmanControl(int*, int*, pacmanInfo*, clock_t*, char[HEIGHT][WIDTH], ghosts*, int); ///Pacman controller
void movePacman(pacmanInfo*, char[HEIGHT][WIDTH]); ///Moves the pacman
void testLimits(pacmanInfo*); ///Test map limits
void testColision(pacmanInfo*, char[HEIGHT][WIDTH]); ///Test collision with walls
void setDirection(char, int*, pacmanInfo*); ///Choose the right direction of the pacman

#endif // PACMAN_FUNCTIONS_H_
