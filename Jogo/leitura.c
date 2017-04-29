#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define str(s)						#s
#define rd_fld(fich, var, field)	if (fscanf (fich, str(field) ": %d ", &var.field) != 1){ \
										fprintf(stderr, "%s\n", "NÃO CONSEGUI LER O CAMPO" str(field"\n")); \
											exit(1); \
										}
	
									
#define rd_flds(fich, var, field)	fscanf (fich, str(field)": %s ", var.field)

typedef struct {
	int hp;
	char nome[20];
	int inimigos;
} ESTADO;

ESTADO set_hp (ESTADO e, char* valor){
	sscanf(valor, "%d", &e.hp);
	return e;
}

ESTADO set_inimigos (ESTADO e, char* valor){
	sscanf(valor, "%d", &e.inimigos);
	return e;
}

ESTADO set_nome (ESTADO e, char* valor){
	sscanf(valor, "%s", e.nome);
	return e;
}


ESTADO ler_valor (ESTADO e, char* campo, char* valor){
	struct {
		char* nome;
		ESTADO (*funcao)(ESTADO e, char* valor); 
	} Campos[] = {
		{"hp", set_hp},
		{"nome", set_nome},
		{"inimigos", set_inimigos},
		{NULL, NULL}
	};
	int i = 0;
	while (Campos[i].nome != NULL && strcmp(Campos[i].nome, campo) != 0) {
		i++;
	}
	if (Campos[i].nome == NULL) {
		fprintf(stderr, "Campo %s não existe\n", campo);
		exit(1);
	}
	return Campos[i].funcao(e, valor);
}


ESTADO ler_campo (ESTADO e, char* linha) {
	char nome [100];
	char resto [10240];
	if (sscanf (linha, "%[^:]: %[^\n]", nome, resto) != 2) {
		fprintf(stderr, "linha %s mal formada", linha);
		exit(1);
	}
	return ler_valor(e, nome, resto);
}


int main(){
	FILE* f;
	ESTADO e;
	char linha [10240];
	if ((f = fopen("config","r")) == NULL){
		perror("NÃO CONSEGUI ABRIR O FICHEIRO");
		exit(1);
	}
	while (fgets(linha, 10240, f) != NULL ) {
		e = ler_campo(e, linha);
	}
	fclose (f);
	printf("%d\n", e.hp);
	printf("%s\n", e.nome);
	printf("%d\n", e.inimigos);
}

