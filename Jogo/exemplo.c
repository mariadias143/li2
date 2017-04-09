#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"

#define MAX_BUFFER		10240
#define TAM				10
#define ESCALA			50

int posicao_valida(int x, int y) {
	return x >= 0 && y >= 0 && x < TAM && y < TAM;
}
int posicao_igual(POSICAO p, int x, int y) {
	return p.x == x && p.y == y;
}

int tem_jogador (ESTADO e, int x, int y){
	return posicao_igual(e.jog, x, y);
}

int tem_inimigo (ESTADO e, int x, int y) {
	int i;
	for(i=0; i < e.num_inimigos; i++)
		if  (posicao_igual(e.inimigo[i], x, y))
			return 1;
		return 0;
}

int tem_obstaculo (ESTADO e, int x, int y) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		if  (posicao_igual(e.obstaculo[i], x, y))
			return 1;
		return 0;
}

int tem_porta (ESTADO e, int x, int y){
	return posicao_igual(e.porta, x, y);
}

int posicao_ocupada (ESTADO e, int x, int y){
	return tem_jogador(e, x, y) || tem_inimigo(e, x, y) || tem_obstaculo(e, x, y)|| tem_porta(e, x, y);
}

void imprime_casa(int x, int y) {
	char *cor[] = {"#132024", "#143024"};
	int idx = (x + y) % 2;
	QUADRADO(x, y,ESCALA, cor[idx]);
}

void imprime_casa_transparente(int x, int y) {
	QUADRADO_TRANSP(x, y,ESCALA);
}

ESTADO inicializar_inimigo (ESTADO e) {
    int x, y;

    do {
   		x = random() % TAM;
    	y = random() % TAM;
    } while (posicao_ocupada (e, x, y));

    e.inimigo[(int)e.num_inimigos].x = x;
    e.inimigo[(int)e.num_inimigos].y = y;
    e.num_inimigos++;
	return e;
}

ESTADO inicializar_inimigos (ESTADO e, int num) {
	int i;
	for (i = 0; i < num; i++)
		e = inicializar_inimigo (e);
	return e;
}

ESTADO inicializar_obstaculo (ESTADO e) {
    int x, y;
    do {
   		x = random() % TAM;
    	y = random() % TAM;
    } while (posicao_ocupada (e, x, y));
    e.obstaculo[(int)e.num_obstaculos].x = x;
    e.obstaculo[(int)e.num_obstaculos].y = y;
    e.num_obstaculos++;
	return e;
}

ESTADO inicializar_obstaculos (ESTADO e, int num){
	int i;
	for (i = 0; i < num; i++)
		e = inicializar_obstaculo (e);
	return e;
}


ESTADO inicializar() {
	ESTADO e = {{0}};
	e.jog.x = random() % TAM;
	e.jog.y = random() % TAM;
	e.porta.x = random() % TAM;
	e.porta.y = random() % TAM;
	e = inicializar_inimigos(e, 20);
	e = inicializar_obstaculos(e, 20);
	return e;
}

void imprime_movimento(ESTADO e, int dx, int dy) {
	ESTADO novo = e;
	int x = e.jog.x + dx;
	int y = e.jog.y + dy;
	char link[MAX_BUFFER];
	if(!posicao_valida(x, y))
		return;
	if(posicao_ocupada(e, x, y) && !tem_porta(e, x, y))
		return;
	novo.jog.x = x;
	novo.jog.y = y;
	sprintf(link, "http://localhost/cgi-bin/exemplo?%s", estado2str(novo));
	ABRIR_LINK(link);
	imprime_casa_transparente(x, y);
	FECHAR_LINK;
}

void imprime_movimentos(ESTADO e) {
	int dx, dy;
	for (dx = -1; dx <= 1; dx++)
		for (dy = -1; dy <= 1; dy++)
			imprime_movimento(e, dx, dy);
}

void imprime_jogador(ESTADO e) {
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "DwellerN_03.png");
	imprime_movimentos(e);
}

void imprime_porta (ESTADO e) {
	IMAGEM(e.porta.x, e.porta.y, ESCALA, "heart.png");
}


ESTADO ler_estado(char *args) {
	if(strlen(args) == 0)
		return inicializar();

	ESTADO e = str2estado(args);
	if (e.jog.x == e.porta.x && e.jog.y == e.porta.y)
		e = inicializar();

	return e;
}

void imprime_inimigos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_inimigos; i++)
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "Driders_04.png");
}

void imprime_obstaculos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "lava_pool1.png");
}


int main() {
	srandom (time(NULL));
	int x, y;
	ESTADO e = ler_estado(getenv("QUERY_STRING"));

	COMECAR_HTML;
	ABRIR_SVG(600, 600);
    	BACKGROUND;
	for(y = 0; y < 10; y++)
		for(x = 0; x < 10; x++)
			imprime_casa_transparente(x, y);

	imprime_inimigos(e);
	imprime_obstaculos(e);
    	imprime_porta(e);
    	imprime_jogador(e);
    

	FECHAR_SVG;

	return 0;
}
