#ifndef LABIRINTO_FUNCTIONS_H_
#define LABIRINTO_FUNCTIONS_H_

int readLab(char[30][100], int); ///Reads the maze and puts it in the matrix lab
int startLab(char[30][100], int*, pacmanInfo*, ghostsInfo*, int); ///Sets initial positions
int showLab(char[30][100], pacmanInfo*, ghostsInfo*); ///Prints the original lab


#endif // LABIRINTO_FUNCTIONS_H_



