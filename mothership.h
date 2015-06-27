#ifndef MOTHERSHIP_H
#define MOTHERSHIP_H

struct Jogo;

struct Mothership{
	ALLEGRO_BITMAP *imagem=NULL;
	ALLEGRO_FONT *fonte=NULL;
	ALLEGRO_TIMER *tempo=NULL;
	ALLEGRO_EVENT_QUEUE *fila_evento = NULL;
	int segundos;
	int posicao_x;
	int posicao_y;
	int velocidade;
	int frequencia;
	int status;
	int altura_sprite;
	int largura_sprite;
	int largura_tela;
};

//desenha a mothership de acordo com as coordenadas
//apenas essa aqui será usada no loop do jogo
void desenha_mothership(Mothership*);

// por enquanto apenas coloca posicao_x=0 e posicao_y=0[depois faz mais]
// AINDA PRECISA REINICIAR O CONTADOR AQUI
void reinicia_mothership(Mothership*);

// responsável pela mudança nas coordenadas
void movimenta_mothership(Mothership*);

// carrega a imagem em mothership->imagem, reinicia as posições e define a velocidade
//utilizar antes da inicialização do loop do jogo
void inicializa_mothership(Mothership*, Jogo*);

//função que controla se a mothership mae aparece ou n
int autoriza_mothership(Mothership*);

void inicializar_timer_mothership(Mothership*);

void disparar_timer_mothership(Mothership*);


//destroi a mothership mae
void finaliza_mothership(Mothership*);

#endif
