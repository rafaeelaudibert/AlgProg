#ifndef GHOSTS_FUNCTIONS_H
#define GHOSTS_FUNCTIONS_H

int ghostsControl(int*, pacmanInfo, clock_t*, char[30][100], ghosts*, int, int); ///Controls ghost movement
void shuffleDir(); ///Random Sort of the dir[]
void moveGhost(pacmanInfo, char[30][100], ghosts*, int); ///Move the ghosts
void showGhosts(pacmanInfo, char[30][100], ghosts*, int); ///Print the ghosts
void chooseDirection(pacmanInfo, ghost*, char[30][100]); ///Ghost random valid direction
int changeDirection(ghost, char[30][100]); ///Changes ghost direction
void chasePacman(pacmanInfo, ghost, char[30][100]); ///Sorts dir[] array, according to the cartesian distance from the ghost to the pacman
int freeSides(ghost, char[30][100]); ///Sums the sides with no walls, around the ghost
int testGhostLimits(ghost*); ///Check if the ghost is outside of the map limits
int checkGhostCollision(pacmanInfo, int*, ghosts*); ///Collision between ghosts and the pacman
void reviveGhosts(ghosts *ghosts, int); ///Revives a ghost

#endif // GHOSTS_FUNCTIONS_H
