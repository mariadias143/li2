#include <stdio.h>
#include <stdlib.h>

#define str(s)						#s
#define prt_fld(fich, var, field)	fprintf(fich, str(field) ": %d\n", var.field)
#define prt_flds(fich, var, field)	fprintf(fich, str(field) ": %s\n", var.field)

typedef struct {
	int hp;
	char nome[20];
	int inimigos;
} ESTADO;


int main(){
	FILE* f;
	ESTADO e = {20,"Mercy",5};
	if ((f = fopen("config","w")) == NULL){
		perror("NÃO CONSEGUI ABRIR O FICHEIRO");
		exit(1);
	}
	prt_fld(f, e, hp);
	prt_flds(f, e, nome);
	prt_fld(f, e, inimigos);
	fclose (f);
}

