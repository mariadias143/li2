#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "cgi.h"
#include "estado.h"
#include "escrever.c"


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


ESTADO inicializar(int x) {
	ESTADO e = {{0}};
	e.jog.x = random() % TAM;
	e.jog.y = random() % TAM;
	e.hp = 100;
	e.score = x;
	e.porta.x = random() % TAM;
	e.porta.y = random() % TAM;
	e = inicializar_inimigos(e, 12);
	e = inicializar_obstaculos(e, 20);
	return e;
}



int returninimigo (ESTADO e, int x,int y){
	int i;
	for (i=0; i < e.num_inimigos; i++){
			if (e.inimigo[i].x == x && e.inimigo[i].y == y)
				return i;
	}
	return -1;
}


ESTADO ataca_inimigo (ESTADO e, int x, int y){
	int i;
	i = returninimigo(e,x,y);
	for (i; i < e.num_inimigos; i++){
			e.inimigo[i] = e.inimigo[i+1];
	}
	e.num_inimigos--;
	e.score+=10;
	return e;

}

int calculadist (ESTADO e, int dx, int dy){
	int x = e.jog.x;
	int y = e.jog.y;
	int dist;

	dist = ((dx - x)^2) + ((dy - y)^2);

	return dist;
}



// funçao que faz array com posiçoes para as quais os inimigos podem ir

ESTADO pospossiveis (ESTADO e,int i){
	int dx, dy;
	int x, y;
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
				if(!tem_inimigo(e, a, b) &&	!tem_obstaculo(e, a, b) &&
				!tem_porta(e, a, b) &&	posicao_valida(a, b) &&		!tem_jogador(e, a, b))
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


ESTADO loopmove (ESTADO e) {
	int i;
	for (i = 0; i < e.num_inimigos; i++)
		e = pospossiveis(e,i);
	return e;
}

ESTADO matar_jogador (ESTADO e) {
	if (e.hp <= 0) {
		e.hp = 0;
		GAME_OVER;
		}
	return e;
}

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
			sprintf(link, "http://localhost/cgi-bin/exemplo?atacar_%i_%i",x,y);
			ABRIR_LINK(link);
			imprime_casa_transparente(x, y);
			FECHAR_LINK;
			return;

		}
	sprintf(link, "http://localhost/cgi-bin/exemplo?andar_%i_%i",x,y);
	ABRIR_LINK(link);
	imprime_casa_transparente(x, y);
	FECHAR_LINK;
}

void imprime_movimentos(ESTADO e) {
	int dx, dy;
	for (dx = -1; dx <= 1; dx++)
		for (dy = -1; dy <= 1; dy++)
			if (dx != 0 || dy != 0)
			imprime_movimento(e, dx, dy);
}

void imprime_jogador(ESTADO e) {
	IMAGEM(e.jog.x, e.jog.y, ESCALA, "foguetao.png");
	imprime_movimentos(e);
}

void imprime_porta (ESTADO e) {
	IMAGEM(e.porta.x, e.porta.y, ESCALA, "earth.png");
}
int comparastrg(char * s, char * c){
	int i;
	for(i=0;s[i] != '\0' && c[i] != '\0'; i++){
		if (s[i] != c[i]) return 0;
	}
	return 1;
}


ESTADO decode(ESTADO e, char * args){
	if( comparastrg(args, "andar")){
		int x,y;
			x=args[6] - '0';
			y=args[8] - '0';
			e.jog.x = x;
			e.jog.y = y;
			if (e.jog.x == e.porta.x && e.jog.y == e.porta.y){
				e.score+=100;
				e = inicializar(e.score);
			}
				else{
					e = loopmove(e);
				}
	}
	if( comparastrg(args, "atacar")){
		int x,y;
			x=args[7] - '0';
			y=args[9] - '0';
	    e = ataca_inimigo (e,x,y);
			e.jog.x = x;
			e.jog.y = y;
			e = loopmove(e);

		}
return e;
}


ESTADO ler_estado(char *args) {
	FILE * gameinfo;
	ESTADO e;
	if(strlen(args) == 0){
		e = inicializar(0);
	}

	else{

	gameinfo = fopen("/tmp/gameinfo", "r");
	fread(&e, sizeof(ESTADO), 1,gameinfo);
		e = decode( e, args );
		fclose(gameinfo);
		}

	gameinfo = fopen("/tmp/gameinfo", "w+");
	fwrite(	&e, sizeof(ESTADO), 1, gameinfo);
	fclose(gameinfo);
	return e;

}


void imprime_inimigos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_inimigos; i++)
		IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "alien1.png");
}

void imprime_obstaculos(ESTADO e) {
	int i;
	for(i = 0; i < e.num_obstaculos; i++)
		IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "rock1.png");
}

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
			    while(fgets(ficheiro,sizeof ficheiro,pontuacao)!= NULL){}
					sscanf(ficheiro, "%d_%d_%d_", &a,&b,&c);
					pontuacao = fopen ( "/tmp/pontuacao" , "w+" ) ;


					if (e.score > a) {
						fprintf(pontuacao, "%i_%i_%i_", e.score,a,b);
						printf("<text x=%d y=%d>\n%s\n</text>\n",550,50,"O seu score");

					}else if (e.score > b) {
						fprintf(pontuacao, "%i_%i_%i_", a,e.score,b);
						printf("<text x=%d y=%d>\n%s\n</text>\n",550,100,"O seu score");

					}else if (e.score > c) {
						fprintf(pontuacao, "%i_%i_%i_", a,b,e.score);
						printf("<text x=%d y=%d>\n%s\n</text>\n",550,150,"O seu score");

					}else
						fprintf(pontuacao, "%i_%i_%i_", a,b,c);
			}

	fclose(pontuacao);

}

int main() {
	srandom (time(NULL));
	int x, y;
	ESTADO e = ler_estado(getenv("QUERY_STRING"));

	COMECAR_HTML;
	ABRIR_SVG(700, 700);

	if (e.hp <= 0) {
		FILE * pontuacao;
		e.hp = 0;
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
				    while(fgets(ficheiro,sizeof ficheiro,pontuacao)!= NULL){}
						sscanf(ficheiro, "%d_%d_%d_", &a,&b,&c);
						printf("<text x=%d y=%d>\n%i\n</text>\n",500,50,a);
						printf("<text x=%d y=%d>\n%i\n</text>\n",500,100,b);
						printf("<text x=%d y=%d>\n%i\n</text>\n",500,150,c);


						fclose(pontuacao);

		}
	else{
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


	sprintf(linha, "%s %d HP", e.nome, e.hp);
	sprintf(linha2, "%s %d SCORE", e.nome, e.score);

	TEXTO(11, 1, ESCALA, linha);
	TEXTO(11, 3, ESCALA, linha2);

}

	FECHAR_SVG;

	return 0;
}
