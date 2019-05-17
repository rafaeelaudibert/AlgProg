# AlgProg

## JOGO NO CMD: PACMAN

Jogo feito em C, conforme requisitos do trabalho final da cadeira INF01202 – Algoritmos e Programação no INF/UFRGS (Universidade Federal do Rio Grande do Sul).

O jogo está organizado em 8 arquivos `*.c`, com seus respectivos headers, além de um header responsável por armazenas as estruturas.

Para executar o programa somente é necessário rodar o executável e seguir os menus. Para percorrer o menu verticalmente são utilizadas as teclas `W` e `X`, e para entrar na função do Menu é utilizada a tecla `D`, exceto dentro do menu `Opções`, onde é utilizado o botão Enter quando selecionado `Save` para salvar o jogo e retornar para o menu principal, e quando selecionado `Default` para fazer os valores das opções retornarem ao padrão do jogo.
	
Dentro do jogo, as teclas de movimentação padrão são:
- CIMA: `W` ou Seta para cima;
- BAIXO: `X` ou Seta para baixo;
- ESQUERDA: `A` ou Seta para esquerda;
- DIREITA: `D` ou Seta para direita;
- PARAR: `S`;
- PAUSE: `P`;
- DESPAUSE: `R`;
- FINALIZAR JOGO: `Esc` ou Espaço;
- FUNÇÃO “ATIVA CHEAT”:	`F1`;
- FUNÇÃO “DESATIVA CHEAT”: `F2`;
	
As teclas de movimentação, assim como a dificuldade, a velocidade e o labirinto podem ser alterados no Menu Opções acessável no início do jogo. Essas escolhas perduram através de jogos, e podem ser resetadas no menu Opções, como já informado.

A pontuação é dada por: 10 pontos para as pastilhas comuns, 50 pontos para as super-pastilhas e 200 pontos para fantasmas (5000 pontos para as paredes, quando utilizando o cheat).
	
Os fantasmas, além de terem sua velocidade reduzida em 30%, podem ser comidos por 5 segundos após o Pac-man ter comido uma super-pastilha. Caso o Pac-man encoste em um fantasma fora desse tempo de 5 segundos, o Pacman perde uma das 3 vidas que ele possui.
	
O jogo acaba quando o Pac-man perder todas as suas vidas, ou quando ele comer todas as pastilhas disponíveis no labirinto. Caso o jogo acabe com a vitória por ter comido todas as pastilhas, é adicionada a pontuação o valor de 150 pontos para cada vida restante.


### DESCRIÇÃO TÉCNICA DO JOGO:

#### Estruturas utilizadas:

- Estrutura que guarda uma posição `x, y` do plano cartesiano:
```C
struct Coord
{
	int x;
	int y;
}
```

- Estrutura que guarda as informações referentes a UM fantasma
```C
struct Ghost
{
	Coord pos;			// Posição atual do fantasma
	Coord origin;			// Posição de renascimento do fantasma
	Coord mov;			// Incremento da posição atual do fantasma
	char key;			// ‘W’ ou ‘w’
	int alive;			// Fantasma vivo ou não
	int reviveTime;			// Utilizando durante “renascimento” do fantasma
	clock_t deathTime;		// “Hora” da morte do fantasma
}
```		

- Estrutura que guarda TODOS os fantasmas assim como a quantidade existente deles
```C
struct Ghosts
{
	int quant;
	Ghost unid[QTD_FANTASMAS];
}
```

- Estrutura que guarda eixo e direção no eixo de um movimento
```C
struct Directions
{
	char coordinates;		\\ 'x', 'y' ou 's'
	int up_down;			\\ '1' ou '-1'
}
```

- Estrutura que guarda as informações do Pacman
```C
struct PacmanInfo
{
	Coord pos;			\\ Posição atual do Pacman
	Coord origin;			\\ Posição de renascimento do Pacman
	int lives;			\\ Vidas do pacman
	int pacDotActive;		\\ Tempo de “super-poder” do Pacman
	Directions next;		\\ Proxima direção a ser andada
	Directions last;		\\ Última direção andada
}
```

- Estrutura que guarda informação do Highscore
```C
struct Score
{
	int position;			
	char name[30];			\\ Nome do jogador nessa posição
	int points;			\\ Pontuação
	clock_t duration;		\\ Duração do jogo
	char dateStr[15];		\\ Data do jogo
	char timeStr[15];		\\ Hora do jogo
}
```

•Estrutura que guarda as informações da partida atual
```C
struct Actual_Game
{
	char name[30];			\\ Nome do jogador jogando
	int points;			\\ Pontuação do jogo
}
```

#### O jogo:
	
No Menu Inicial, podemos tanto escolher jogar o jogo como mexer em algumas opções.  Para percorrer o menu verticalmente são utilizadas as teclas `W` e `X`, e para entrar na função do Menu é utilizada a tecla `D`, exceto dentro do menu Opções, onde é utilizado o botão Enter quando selecionado `Save` para salvar o jogo e retornar para o menu principal, e quando selecionado `Default` para fazer os valores das opções retornarem ao padrão do jogo estabelecido pelos programadores.

Na aba `Créditos`, temos os produtores do game, e na aba `Sair`, fechamos o jogo. Na aba `Opções` somos capazes de alterar várias coisas do jogo, como sua dificuldade, sua velocidade, o labirinto que será exibido assim como as principais teclas de movimentação. As setinhas direcionais continuarão funcionando ingame, não importa quais sejam as escolhas feitas no menu `Opções`.

Após o menu inicial, temos o jogo em si.
	
O programa utiliza um arquivo guardado em `\data\labirinto.txt` como mapa/labirinto, contendo 30 linhas x 100 colunas, lendo dele as informações sobre todos os objetos presentes na partida (Pacman, fantasmas, pastilhas, super-pastilhas, paredes), guardando em suas devidas estruturas além de guardar o próprio labirinto em uma matriz. O pacman sempre começará com 3 vidas, perdendo uma a cada morte.

O clock do jogo funciona iterando o loop do jogo a cada `speed`ms (variável definida no menu Opções), que é conferido através de variáveis que armazenam valores de tempo do jogo. É possível pausar o jogo utilizando a tecla `P`, que chama a função `gamePause()` e a deixa em loop até ser pressionada a tecla `R` para sairmos do pause.

As teclas de movimentação são detectadas através da função `GetAsyncKeyState()`, que retorna `true` caso o parâmetro passado a ela seja a tecla pressionada no momento, portanto ela fica sempre iterando verificando qual foi a última tecla pressionada, para assim realizar a sua movimentação. Não é possível atravessar paredes já que em todo laço de movimentação temos a função `testCollision()` que impede o pacman de entrar em alguma parede. Através ainda da função `testLimits()` podemos fazer o Pacman "atravessar" o mapa, caso haja um espaço livre diretamente oposto a ele.

Os fantasmas se movimentam da seguinte maneira (Dificuldade é definido no menu Opções):

- `(Dificuldade * 10)%` de chance de perseguir o pacman ao chegar em uma bifurcação, detectando entre os possíveis caminhos qual é o que possui a menor distância euclidiana até o Pacman;
		
- `((10-Dificuldade) * 10)%` de chance de escolher aleatoriamente um dos caminhos possíveis, quando em uma bifurcação
	
Ao comer uma pastilha ela é retirada da matriz do labirinto, e é incrementado seus pontos (10). Ao comer uma super-pastilha, além de ser retirada da matriz e incrementado seus 50 pontos no contador, é setado um timer que dura 5 segundos, durante os quais os fantasmas podem ser devorados pelo pacman, rendendo 200 pontos, além de terem sua velocidade reduzida em 30% (speed*1,3). Após 8 segundos mortos, os fantasmas renascem, ficando “intocáveis” por um dado período de tempo, durante o qual não podem nem matar o Pacman, nem morrer.

A colisão entre Pacman e Fantasma é verificada a cada movimentação do Fantasma e/ou do Pacman, verificando se a posição entre o pacman e algum dos fantasmas coincide. Caso seja constata a colisão é verificado se o Pacman está com os super poderes ativos ou não. Caso não esteja, é descontada uma vida do mesmo. Caso o Pacman ainda tenha vidas sobrando, o jogo é reiniciado recarregando tanto o Pacman quanto o fantasma em suas posições originais. Caso contrário o jogo é encerrado como derrota.

Para o jogador ganhar, é necessário que todas as pastilhas COMUNS tenham sido comidas, não precisando que o jogador coma as super-pastilhas. Caso o jogador ganhe, a pontuação é incrementada em `((Vidas Restantes + 1) * 150)`. Esse incremento de 1, serve para contar a vida ATUAL do Pacman.

Ao final do jogo temos uma tela de Highscore, e após a mesma o jogo retornamos para o menu inicial
	

#### Extras:
	
Foi utilizada uma função extra para reconstruir uma parte do labirinto embaixo das mensagens, para não precisar reconstruir todo o labirinto, já que computadores mais lentos não conseguem imprimir o labirinto instantaneamente.

Também foi utilizada uma função para poder colorir os diferentes elementos do jogo, além de possuir uma constante no arquivo `main.h` onde podemos alterar os caracteres a serem printados para os variados elementos do jogo, podendo colocar qualquer elemento da tabela ASCII para ser exibido no CMD do jogo.

Para os efeitos sonoros, foi utilizada a função `Beep()` que utiliza o buzzer da placa-mãe para emitir som. 

Foi implementado a função Highscore que verifica em um arquivo guardado em `\data\Highscores.txt` as últimas pontuações do jogo, e verifica se a pontuação da partida atual é suficiente para poder entrar no Highscores ou não. Caso seja faz toda uma manipulação de dados, para poder adicionar o nome, a pontuação, o tempo de duração da partida e a data e a hora de finalização da partida. Após isso, o arquivo é novamente acessado para podermos guardar nele a pontuação atual, caso ela seja merecedora de estar no Top 10.

