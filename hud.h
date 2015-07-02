#ifndef HUD_H
#define HUD_H

#include "config.h"

struct Hud {
	ALLEGRO_FONT *fonte=NULL;
	ALLEGRO_BITMAP *life=NULL;
	ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
	FILE *arquivo_high_score;
	int score, score_posicao_x;
	int lives, lives_posicao_x;

	int altura_sprite_vida, largura_sprite_vida;

	int posicao_y;

	char nome_score[3];

	char nomes_high_score[10][3];
	int scores_high_score[10];
};

void inicializa_hud (Hud *hud);

void inicializa_score (Hud* hud);

void inicializa_lives (Hud* hud);

void desenha_hud (Hud* hud);

void desenha_score (Hud* hud);

void desenha_lives (Hud* hud);

void ler_high_score(Hud *hud);

void iniciar_salvar_score(Hud *hud);

void inserir_caracter(Hud* hud, char letra);

void apagar_caracter(Hud* hud);

void permutar_strings(char *nome1, char *nome2);

void inicializar_high_score(Hud *hud);

void enviar_score(Hud *hud);

void ordenar_vetor(int *vetor, char nomes[][3], int n_elementos);

#endif
