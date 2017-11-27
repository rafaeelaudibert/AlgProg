#ifndef OBJECTS_FUNCTIONS_H
#define OBJECTS_FUNCTIONS_H

void checkPacDots(int*, int*, char[HEIGHT][WIDTH], pacmanInfo); ///Check the pacdots collision with the pacman
void checkPowerPellets(int*, char[HEIGHT][WIDTH], pacmanInfo*, ghosts*, int); ///Check the power pellets collision with the pacman

#endif // OBJECTS_FUNCTIONS_H
