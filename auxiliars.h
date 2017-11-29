#ifndef AUXILIARS_FUNCTIONS_H_
#define AUXILIARS_FUNCTIONS_H_

void readSettings(int*, int*, int*, char*, char*, char*, char*, char*); ///Read from a file, the gameSettings
void writeSettings(int, int, int, char, char, char, char, char); ///Prints in the file the gameSettings
void reconstructMaze(int, int, int, int, char[HEIGHT][WIDTH], pacmanInfo); ///Rebuilds a given part of the lab
void gotoXY(int, int); ///Goes to a given position
void cursorType(int); ///Sets the cursor type
void textcolor(); ///Conio.c Prototype

#endif // AUXILIARS_FUNCTIONS_H_
