#ifndef MENU_FUCNTIONS_H_
#define MENU_FUNCTIONS_H_

int menu(int*, int*, int*, char*, char*, char*, char*, char*); ///Game Menu
void options(int*, int*, int*, char*, char*, char*, char*, char*); ///Options menu
void credits(void); ///Credits menu
void pacMusic(void); ///Pac-music played in credits
void printsHeader(void); ///Game menu header printing
void printsOptions(int*, int*, int*, char*, char*, char*, char*, char*); ///Game menu options printing
void optionsCanvas(void); ///Options menu printing
char detectKeyMenu(void); ///Detects the actual pressed key
#endif // MENU_FUCNTIONS_H_
