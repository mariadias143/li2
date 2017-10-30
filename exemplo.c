#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "cgi.h"
#include "estado.h"
//#include "escrever.c"


#define MAX_BUFFER		10240
#define WIDTH   		10
#define HEIGHT      14
#define ESCALA			40

/**
@file exemplo.c
*/

/**
\brief Função que verifica se a posiçao (x,y) é uma posição válida no mapa.
*/
int posicao_valida(int x, int y) {
	return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT;
}

/**
\brief Função que verifica se as coordenadas de duas posições são iguais.
*/
int posicao_igual(POSICAO p, int x, int y) {
	return p.x == x && p.y == y;
}

/**
\brief Fu.nção que verifica se existe jogador na posiçao (x,y)
*/
int tem_jogador (ESTADO e, int x, int y){
	return posicao_igual(e.jog, x, y);
}

/**
\brief Função que verifica se existe um inimigo na posiçao (x,y)
*/
int tem_inimigo (ESTADO e, int x, int y) {
	int i;
	for(i=0; i < e.num_inimigos; i++)
		if  (posicao_igual(e.inimigo[i], x, y))
			return 1;
		return 0;
}

/**
\brief Função que verifica se existe um obstáculo na posiçao (x,y)
*/
int tem_obstaculo (ESTADO e, int x, int y) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		if  (posicao_igual(e.obstaculo[i], x, y))
			return 1;
		return 0;
}

/**
\brief Função que verifica se existe uma porta na posiçao (x,y)
*/
int tem_porta (ESTADO e, int x, int y){
	return posicao_igual(e.porta, x, y);
}

/**
\brief Função que verifica se a posição (x,y) está ocupada
*/
int posicao_ocupada (ESTADO e, int x, int y){
	return tem_jogador(e, x, y) || tem_inimigo(e, x, y) || tem_obstaculo(e, x, y)|| tem_porta(e, x, y);
}
/**
\brief Função que imprime uma casa transparente na posição (x,y)
*/
void imprime_casa_transparente(int x, int y) {
	QUADRADO_TRANSP(x, y,ESCALA);
}

/**
\brief Função que atribui coordenadas aleatórias para um inimigo
*/
ESTADO inicializar_inimigo (ESTADO e) {
    int x, y;

    do {
   		x = random() % WIDTH;
    	y = random() % HEIGHT;
    } while (posicao_ocupada (e, x, y));

    e.inimigo[(int)e.num_inimigos].x = x;
    e.inimigo[(int)e.num_inimigos].y = y;
    e.num_inimigos++;
	return e;
}

/**
\brief Função que aplica inicializar_inimigo a todos os inimigos
*/
ESTADO inicializar_inimigos (ESTADO e, int num) {
	int i;
	for (i = 0; i < num; i++)
		e = inicializar_inimigo (e);
	return e;
}

/**
\brief Função que atribui coordenadas aleatórias para um obstáculo
*/
ESTADO inicializar_obstaculo (ESTADO e) {
    int x, y;
    do {
   		x = random() % WIDTH;
    	y = random() % HEIGHT;
    } while (posicao_ocupada (e, x, y));
    e.obstaculo[(int)e.num_obstaculos].x = x;
    e.obstaculo[(int)e.num_obstaculos].y = y;
    e.num_obstaculos++;
	return e;
}

/**
\brief Função que aplica inicializar_obstaculo a todos os obstaculos
*/
ESTADO inicializar_obstaculos (ESTADO e, int num){
	int i;
	for (i = 0; i < num; i++)
		e = inicializar_obstaculo (e);
	return e;
}

/**
\brief Função que inicializa o jogo
@param x Score atual do jogador
*/
ESTADO inicializar(int x) {
	ESTADO e = {{0}};
	e.jog.x = random() % WIDTH;
	e.jog.y = random() % HEIGHT;
	e.hp = 100;
	e.score = x;
	e.porta.x = random() % WIDTH;
	e.porta.y = random() % HEIGHT;
	e = inicializar_inimigos(e, 12);
	e = inicializar_obstaculos(e, 20);
	return e;
}


/**
\brief Função que verifica se existe um inimigo na posição (x,y) e dá-nos o índice desse inimigo
*/
int returninimigo (ESTADO e, int x,int y){
	int i;
	for (i=0; i < e.num_inimigos; i++){
			if (e.inimigo[i].x == x && e.inimigo[i].y == y)
				return i;
	}
	return -1;
}

/**
\brief Função que elimina o inimigo do mapa caso o jogador o ataque, aumentando o score do jogador em 10
*/
ESTADO ataca_inimigo (ESTADO e, int x, int y){
	int i;
	i = returninimigo(e,x,y);
	for (; i < e.num_inimigos; i++){
			e.inimigo[i] = e.inimigo[i+1];
	}
	e.num_inimigos--;
	e.score+=10;
	return e;

}
/**
\brief Calcula a distância entre dois pontos

@param e O estado
@param dx Ordenada x da segunda posição
@param dy Ordenada y da segunda posição
*/
int calculadist (ESTADO e, int dx, int dy){
	int x = e.jog.x;
	int y = e.jog.y;
	int dist;

	dist = ((dx - x)^2) + ((dy - y)^2);

	return dist;
}



/**
\brief Função que determina as posiçoẽs para qual o inimigo pode ir e escolhe a que está mais perto do jogador
@param e O estado
@param i índice do inimigo
*/
ESTADO pospossiveis (ESTADO e, int i){
	int dx, dy;
	int x = e.inimigo[i].x, y = e.inimigo[i].y;
	int a, b;
	int dist = 300;
	int dist2;
	for (dx = -1; dx <= 1; dx++)
		for(dy = -1; dy <= 1; dy++) {
				if(dx != 0 || dy != 0) {
					a = e.inimigo[i].x + dx;
					b = e.inimigo[i].y + dy;
				if (tem_jogador(e, a, b)){
					e.hp -= 10;
					return e;
				}
				else {
				if(!posicao_ocupada(e, a, b) &&	posicao_valida(a, b))
					{ dist2 = calculadist(e, a, b);
						if (dist2 <= dist){
							dist = dist2;
							x = a;
							y = b;
						}
				}
			}
		}
	}
	e.inimigo[i].x = x;
	e.inimigo[i].y = y;
	return e;
}

/**
\brief Função que aplica a função pospossiveis a todos os inimigos
*/
ESTADO loopmove (ESTADO e) {
	int i;
	for (i = 0; i < e.num_inimigos; i++)
		e = pospossiveis(e,i);
	return e;
}

/**
\brief Função que mata o jogador, igualando a sua vida a 0 e mostrando a imagem do game over
*/
ESTADO matar_jogador (ESTADO e) {
	if (e.hp <= 0) {
		e.hp = 0;
		GAME_OVER;
		}
	return e;
}

/**
\brief Função que imprime o movimento do jogador e coloca na barra de endereços a ação, andar ou atacar, que este realizou
@param e O estado
@param dx Deslocamento, entre -1 e 1, da ordenada x
@param dy Deslocamento, entre -1 e 1, da ordenada y
*/
void imprime_movimento(ESTADO e, int dx, int dy) {
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	char link[MAX_BUFFER];
	if(!posicao_valida(x, y))
		return;
	if(tem_obstaculo(e, x, y) && !tem_porta(e, x, y))
		return;
	if(tem_inimigo (e, x ,y))
		{
			sprintf(link, "http://localhost/cgi-bin/exemplo?atacar_%i_%i", x, y);
			ABRIR_LINK(link);
			imprime_casa_transparente(x, y);
			FECHAR_LINK;
			return;

		}
	sprintf(link, "http://localhost/cgi-bin/exemplo?andar_%i_%i", x, y);
	ABRIR_LINK(link);
	imprime_casa_transparente(x, y);
	FECHAR_LINK;
}

/**
\brief Função que aplica a função imprime_movimento consoante for o deslocamento/movimento realizado??????????ta mal
*/
void imprime_movimentos(ESTADO e) {
	int dx, dy;
	for (dx = -1; dx <= 1; dx++)
		for (dy = -1; dy <= 1; dy++)
			if (dx != 0 || dy != 0)
			imprime_movimento(e, dx, dy);
}

/**
\brief Função que imprime a imagem do jogador
*/
void imprime_jogador(ESTADO e) {
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "foguetao.png");
	imprime_movimentos(e);
}


/**
\brief Função que imprime a porta
*/
void imprime_porta (ESTADO e) {
	IMAGEM(e.porta.x, e.porta.y, ESCALA, "earth.png");
}

/**
\brief Função que compara duas strings
*/
int comparastrg(char * s, char * c){
	int i;
	for (i = 0; s[i] != '\0' && c[i] != '\0'; i++){
		if (s[i] != c[i]) return 0;
	}
	return 1;
}

/**
\brief Função que transforma a string da barra de endereços na ação, andar ou atacar, que o jogador realizou e nas coordenadas onde o fez
@param e O estado
@param args String que contém a informação sobre o estado
*/
ESTADO decode(ESTADO e, char * args){
	if(comparastrg(args, "andar")){
		int x,y;
			x = args[6] - '0';
			y = args[8] - '0';
			e.jog.x = x;
			e.jog.y = y;
			if (e.jog.x == e.porta.x && e.jog.y == e.porta.y){
				e.score += 100;
				e = inicializar(e.score);
			}
				else{
					e = loopmove(e);
				}
	}
	if(comparastrg(args, "atacar")){
		int x,y;
			x = args[7] - '0';
			y = args[9] - '0';
	    e = ataca_inimigo (e,x,y);
			e.jog.x = x;
			e.jog.y = y;
			e = loopmove(e);

		}
	return e;
}

/**
\brief Função que lê a string do estado e cria ficheiros para o guardar
@param args String que contém a informação do estado
*/
ESTADO ler_estado(char *args) {
	FILE * gameinfo;
	ESTADO e;
	if(strlen(args) == 0){
		e = inicializar(0);
	}

	else{

	gameinfo = fopen("/tmp/gameinfo", "r");
	fread(&e, sizeof(ESTADO), 1,gameinfo);
		e = decode(e, args);
		fclose(gameinfo);
		}

	gameinfo = fopen("/tmp/gameinfo", "w+");
	fwrite(&e, sizeof(ESTADO), 1, gameinfo);
	fclose(gameinfo);
	return e;

}

/**
\brief Função que imprime a imagem dos inimigos
*/
void imprime_inimigos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_inimigos; i++)
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "alien1.png");
}

/**
\brief Função que imprime a imagem dos obstáculos
*/
void imprime_obstaculos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "rock1.png");
}


/**
\brief Função que cria o ficheiro que guarda o score e imprime a tabela dos highscores ao lado do mapa
*/
void guardascore(ESTADO e ){
	FILE * pontuacao;
	pontuacao = fopen ( "/tmp/pontuacao" , "r" ) ;
		if (!pontuacao) {
			pontuacao = fopen ( "/tmp/pontuacao" , "w+" ) ;
			fprintf(pontuacao, "%i_%i_%i_", e.score,0,0);
		}
		else {
			int a,b,c;
			char ficheiro[500];
			    while(fgets(ficheiro,sizeof ficheiro, pontuacao) != NULL){
					sscanf(ficheiro, "%d_%d_%d_", &a,&b,&c);
					pontuacao = fopen ( "/tmp/pontuacao" , "w+" ) ;


					if (e.score > a) {
						fprintf(pontuacao, "%i_%i_%i_", e.score,a,b);
						printf("<text x=%d y=%d>\n%s\n</text>\n",410,50,"YOUR SCORE:");

					}else if (e.score > b) {
						fprintf(pontuacao, "%i_%i_%i_", a,e.score,b);
						printf("<text x=%d y=%d>\n%s\n</text>\n",410,100,"YOUR SCORE:");

					}else if (e.score > c) {
						fprintf(pontuacao, "%i_%i_%i_", a,b,e.score);
						printf("<text x=%d y=%d>\n%s\n</text>\n",410,150,"YOUR SCORE:");

					}else
						fprintf(pontuacao, "%i_%i_%i_", a,b,c);
				}
			}

	fclose(pontuacao);
}

/**
\brief Main, 'nuff said
*/
int main() {
	srandom (time(NULL));
	int x, y;
	ESTADO e = ler_estado(getenv("QUERY_STRING"));

	COMECAR_HTML;
	ABRIR_SVG(700, 1000);

	if (e.hp <= 0) {
		FILE * pontuacao;
		e.hp = 0;
		// PIC(420, 0, 300, 200, "score.png");
		guardascore(e);

		GAME_OVER;

		char link[MAX_BUFFER];

		sprintf(link, "http://localhost/cgi-bin/exemplo?");

		ABRIR_LINK(link);

		REPLAY;

		FECHAR_LINK;

		pontuacao = fopen ( "/tmp/pontuacao" , "r" ) ;
				int a,b,c;
				char ficheiro[500];
				    while(fgets(ficheiro,sizeof ficheiro,pontuacao)!= NULL){
						sscanf(ficheiro, "%d_%d_%d_", &a,&b,&c);
						printf("<text x=%d y=%d>\n%s\n</text>\n",560,20,"HIGH SCORES");
						printf("<text x=%d y=%d>\n%i\n</text>\n",600,50,a);
						printf("<text x=%d y=%d>\n%i\n</text>\n",600,100,b);
						printf("<text x=%d y=%d>\n%i\n</text>\n",600,150,c);
					}

						fclose(pontuacao);

			}
	else {
    BACKGROUND;

	for(y = 0; y < 10; y++)
		for(x = 0; x < 10; x++)
			imprime_casa_transparente(x, y);

	imprime_inimigos(e);
	imprime_obstaculos(e);
    imprime_porta(e);
    imprime_jogador(e);

    char linha[1024] = {0};
		char linha2[1024] = {0};


	sprintf(linha, "%s HP: %d", e.nome, e.hp);
	sprintf(linha2, "%s SCORE: %d", e.nome, e.score);

  HP_BAR(10, 610);
	ICON(10, 610, "heart.jpg");

	TEXTO(35, 630, linha);

	SCORE_BAR(180, 610);
	ICON(180, 610, "estrelinha.png");

	TEXTO(215, 630, linha2);

	}

	FECHAR_SVG;

	return 0;
}
