#ifndef COMMANDS_H_
#define COMMANDS_H_

// Cabeçalho das funções

// Função que lê o labirinto do endereco e coloca na matriz lab
int readLab(char lab[30][100]);

// Função que imprime o lab original
int showLab(char lab[30][100]);

// Carrega o labirinto e carrega as structs
void iniciaLabirinto(char lab[30][100]);

//Funções que retornam posição do pacman para as funções do arquivo main.c (Bugadas no momento)
int retornaXPacman(void);
int retornaYPacman(void);

#endif
