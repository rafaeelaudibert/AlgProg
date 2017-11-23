#ifndef MESSAGES_FUNCTIONS_H
#define MESSAGES_FUNCTIONS_H

void highscores(int, clock_t);
void startMenu(void);
int startMessage(int, pacmanInfo, char[HEIGHT][WIDTH]);
void printTop10(void);
void gravacaoHighscore(FILE*, PLACAR[]);
void colorHeader(void);
void leituraArquivo(FILE*, PLACAR[]);
void manipulacaoDados(PLACAR[], JOGO_ATUAL, clock_t);

#endif // MESSAGES_FUNCTIONS_H
