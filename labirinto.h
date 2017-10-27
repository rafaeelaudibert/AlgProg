#ifndef COMMANDS_H_
#define COMMANDS_H_

// cabeçalho das funções
// função que lê o labirinto do endereco e coloca na matriz lab
int readLab(char endereco[], char lab[30][100]);
// função que imprime o lab original
void showLab(char lab[30][100]);
// carrega o labirinto e carrega as structs
int iniciaLabirinto(char lab[30][100], char endereco[]);
// apenas para testar as funções aqui criadas
int main();
// função para testar apenas leitura do arquivo
void testaLabirinto();

#endif
