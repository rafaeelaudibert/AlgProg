#ifndef GHOSTS_FUNCTIONS_H
#define GHOSTS_FUNCTIONS_H

int ghostsControl(int*, pacmanInfo, clock_t*, char[30][100], ghosts*); // controla o tempo de movimento dos fantasmas
void setupDir(); //Seta as direcoes para o fantasma
void shuffleDir(); //Random Sort of the dir[]
void moveGhost(pacmanInfo, char[30][100], ghosts*); //Movimenta os fantasmas
void showGhosts(pacmanInfo, char[30][100], ghosts*); // show the labirinto
void escolheDirecao(pacmanInfo, ghost*, char[30][100]); //Direção válida aleatória do fantasma
int mudarDirecao(ghost, char[30][100]); //Muda a direção do fantasma
void perseguePacman(pacmanInfo, ghost, char[30][100]); //Altera o array dir, colocando as direções em ordem crescente de distância do ghost
int ladosLivres(ghost, char[30][100]); //Retorna a quantidade de lados que o pacman tem disponivel para andar
int testaLimites(ghost*); // Testa se o fantasma está fora dos limites do mapa
int checkGhostCollision(pacmanInfo, int*, ghosts*); //Interações de colisão entre o pacman e os ghosts
void reviveGhosts(ghosts *ghosts); // revive um fantasma dentre todos da struct ghosts

#endif // GHOSTS_FUNCTIONS_H
