/**
@file cgi.h
Macros úteis para gerar CGIs
*/

#include <stdio.h>

/**
* Caminho para as imagens
*/
#define IMAGE_PATH							"http://localhost/images/"

/**
\brief Macro para começar o html
*/
#define COMECAR_HTML						printf("Content-Type: text/html\n\n")

/**
\brief Macro para abrir um svg
@param tamx O comprimento do svg
@param tamy A altura do svg
*/
#define ABRIR_SVG(tamx, tamy)				printf("<svg width=%d height=%d>\n", tamx, tamy)

/**
\brief Macro para criar a imagem de fundo
*/
#define BACKGROUND             printf("<image width=400 height=650 xlink:href=http://localhost/images/sky.jpg />\n")

/**
\brief Macro para fechar um svg
*/
#define FECHAR_SVG							printf("</svg>\n")


/**
\brief Macro para criar uma imagem
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala da imagem
@param FICHEIRO O caminho para o link do ficheiro
*/
#define IMAGEM(X, Y, ESCALA, FICHEIRO)		printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
												ESCALA * X, ESCALA* Y, ESCALA, ESCALA, IMAGE_PATH FICHEIRO)
/**
\brief Macro para escrever texto
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala da imagem
@param TXT Texto a inserir
*/
#define TEXTO(X, Y, TXT)						printf("<text x=%d y=%d>\n%s\n</text>\n", \
															X, Y, TXT)
/**
\brief Macro para criar a imagem de game over
*/
#define GAME_OVER							printf("<image width=400 height=650 xlink:href=http://localhost/images/abducted.jpg />\n")

/**
\brief Macro para criar a imagem para reiniciar o jogo
*/
#define REPLAY						printf("<circle cx=210 cy=355 r=60 opacity=0 xlink:href=http://localhost/images/circulo.png />\n")

/**
\brief Macro para criar um quadrado
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala do quadrado
@param COR A cor de preenchimento do quadrado
*/

#define QUADRADO_TRANSP(X, Y, ESCALA)			printf("<rect x=%d y=%d width=%d height=%d opacity=0 />\n", \
												ESCALA * X, ESCALA* Y, ESCALA, ESCALA)


#define HP_BAR(X, Y) 				printf("<rect x=%d y=%d width=%d height=%d style=fill:red />\n", \
																		X, Y, 150, 25);

#define SCORE_BAR(X, Y)			printf("<rect x=%d y=%d width=%d height=%d style=fill:green />\n", \
																		X, Y, 150, 25);

#define ICON(X, Y, FICHEIRO)	printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
																					X, Y, 25, 25, IMAGE_PATH FICHEIRO);

#define PIC(X, Y, W, H, FICHEIRO) 	printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
																					X, Y, W, H, IMAGE_PATH FICHEIRO);

/**
\brief Macro para abrir um link
@param link O caminho para o link
*/
#define ABRIR_LINK(link)					printf("<a xlink:href=%s>\n", link)

/**
\brief Macro para fechar o link
*/
#define FECHAR_LINK							printf("</a>\n")
