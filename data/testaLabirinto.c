#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>

int const WIDTH_SCREEN = 100;
int const HEIGHT_SCREEN = 30;

//definição estruturas
struct {
    int x;
    int y;
} typedef ghost;

struct {
    int quant;
    ghost unid[5];
} typedef ghosts;

struct {
    int x;
    int y;
} typedef pacmanS;

struct {
    int x;
    int y;
} typedef coord;

//instâncias globais dos objetos
ghosts fantasmas;
pacmanS pacmanTeste;

coord pastilhas[100];
int q_pastilhas = 0;

coord superPastilhas[10];
int q_superPastilhas = 0;

// cabeçalho das funções
// função que lê o labirinto do endereco e coloca na matriz lab
int readLab(char endereco[], char lab[30][100]);
// função que imprime o lab original
void showLab(char lab[30][100]);
// carrega o labirinto e carrega as structs
int iniciaLabirinto(char lab[30][100], char endereco[]);
// apenas para testar as funções aqui criadas
int testeMain();
// função para testar apenas leitura do arquivo
int testaLabirinto();


// função que lê o arquivo labirinto.txt e configura todos os objetos lidos
// retorna 0 se houve algum erro
// retorna 1 se tudo ocorreu corretamente
int readLab(char endereco[], char lab[30][100]){
    char ch;
    FILE *arq;

    // ponteiro para leitura do arquivo no 'endereco'
    arq = fopen(endereco, "r");

    // se não conseguiu ler o arquivo, sai e retorna 0
    if(arq == NULL){
        printf("ERROR: não foi possível abrir o arquivo\n");
        return 0;
    }

    // i = linha, j = coluna
    int i=0, j=0;
    // passa por cada um dos caracters no arquivo
    while( (ch=fgetc(arq))!= EOF ){
        // se não for uma nova linha, adiciona a matriz lab, e configura as devidas structs
 	    if(ch != '\n'){
            lab[i][j] = tolower(ch);
            j++;
 	    } else {
 	        // verifica se há '\0\n' no fim da linha, se houver, pula para o início da próxima linha da matriz
            if((ch = fgetc(arq)) == '\n'){
                i++;
                j = 0;
            }
 	    }
     };

    return 1;
}

int carregado = 0;

void showLab(char lab[30][100]){
    clrscr();
    int w = 100;
    int h = 30;

    int i, j;
    textcolor(7);
    gotoxy(1,1);
    for(i=0; i<h; i++){
        for(j=0; j<w; j++){
            printf("%c", lab[i][j]);
        }
        printf("\n");
    }
    // se tiver carregado as structs no iniciaLabirinto(), mostra elas nos pontos e com outras cores
    // por enquanto meio bugado
    if(carregado){
        textcolor(2);
        gotoxy(1, 1);
        printf("R");
        textcolor(6);
        gotoxy(pacmanTeste.x, pacmanTeste.y);
        printf("C");
        printf("\nposx %d, posy %d\n", pacmanTeste.x, pacmanTeste.y);
    }
    textcolor(7);
}

// inicia e carrega as structs
int iniciaLabirinto(char lab[30][100], char endereco[]){
    fantasmas.quant = 0;
    int q;
    // mode já deve estar configurado
    if( !readLab(endereco, lab)){
        printf("Error: leitura incompleta!\n");
        return 0;
    }

    int i, j;
    showLab(lab);
    printf("\nCarregado puro\n");
    printf("Carregando ");
    for(i=0; i<5; i++){
        Sleep(500);
        printf(".");
    }

    printf("\n");
    // carrega cada um dos objectos e suas localizações originais
    for(i=0; i<30; i++){
        for(j=0; j<100; j++){
            switch(lab[i][j]){
            case 'c':
                // pacman
                pacmanTeste.x = j;
                pacmanTeste.y = i;
                lab[i][j] = ' ';
                printf("\nx: %d, y: %d\n", pacmanTeste.x, pacmanTeste.y);
                break;
            case 'w':
                // fantasmas
                q = fantasmas.quant;
                fantasmas.unid[ q ].x = j;
                fantasmas.unid[ q ].y = i;
                fantasmas.quant++;
                lab[i][j] = ' ';
                break;
            case '*':
                superPastilhas[ q_superPastilhas ].x = j;
                superPastilhas[ q_superPastilhas ].y = i;
                q_superPastilhas++;
                lab[i][j] = ' ';
                break;
            case 'o':
                pastilhas[ q_pastilhas ].x = j;
                pastilhas[ q_pastilhas ].y = i;
                q_pastilhas++;
                lab[i][j] = ' ';
                break;
            default:
                // nothing
                break;
            }
        }
    }
    // objetos setados e carregados, carregado = 1
    carregado = 0; // 0 enquanto estiver bugado
    showLab(lab);
    system("PAUSE");
    printf("\nCarregado após lido");
    return 1;
}

int main(){
    char lab[30][100];
    iniciaLabirinto(lab, "data/labirinto2.txt");
    return 0;
}


int testaLabirinto(){
    // [ linha ] [ coluna ]
    char lab[HEIGHT_SCREEN][WIDTH_SCREEN];
    // tamanho da tela do CMD, em caracteres
    system("mode 100, 34");
    // lê o arquivo e verifica se ocorreu algum erro
    if( ! readLab("data/labirinto2.txt", lab) ){
       printf("\nErro: leitura incompleta!\n");
    };

    //showLab(lab);
	printf("\n");
	system("pause");

	return 0;
}
