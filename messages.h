#ifndef MESSAGES_FUNCTIONS_H
#define MESSAGES_FUNCTIONS_H

void startMenu(void); ///Starting game menu
int startMessage(int, pacmanInfo, char[HEIGHT][WIDTH]); ///Starting game message
void highscores(int, clock_t); ///Highscores table
void printTop10(void); ///Top10 "image"
void readHighscore(FILE*, SCORE[]); ///Reads the highscore file
void writeHighscore(FILE*, SCORE[]); ///Writes in the highscore file
void dataManipulation(SCORE[], ACTUAL_GAME, clock_t); ///Manipulates the highscore data
void colorHeader(void); ///Makes the highscore header to blink

#endif // MESSAGES_FUNCTIONS_H
