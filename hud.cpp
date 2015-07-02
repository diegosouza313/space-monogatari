#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "hud.h"

void inicializa_hud (Hud *hud){
	hud->fonte = al_load_font("resources/acknowledge.ttf", (LARGURA_DISPLAY/32), 0);
	if (hud->fonte == NULL){
		puts("Erro ao carregar \"resources/acknowledge.ttf\"");
		exit(0);
	}

	hud->posicao_y = (ALTURA_DISPLAY/48);

	inicializa_score(hud);
	inicializa_lives(hud);
}

void inicializa_score (Hud *hud){
	hud->score = 0;
	hud->score_posicao_x = (LARGURA_DISPLAY/12);
}

void inicializa_lives (Hud *hud){
	hud->life = al_load_bitmap("resources/player4.png");
	if(hud->life == NULL){
		puts("Erro ao carregar \"resources/player4.png\"");
		exit(0);
	}

	hud->altura_sprite_vida = 24*(ALTURA_DISPLAY/480);
	hud->largura_sprite_vida = 24*(LARGURA_DISPLAY/640);

	hud->lives = NUMERO_VIDAS;
	hud->lives_posicao_x = (LARGURA_DISPLAY/12)*6;
}

void desenha_hud (Hud *hud){
	desenha_score(hud);
	desenha_lives(hud);
}

void desenha_score (Hud *hud){
	al_draw_text(hud->fonte,
				 CINZA,
				 hud->score_posicao_x, 
				 hud->posicao_y, 
				 0, 
				 "SCORE: ");

	al_draw_textf(hud->fonte, 
				  VERDE, 
				  hud->score_posicao_x + (LARGURA_DISPLAY/12)*1.5, 
				  hud->posicao_y, 
				  0, 
				  "%d", 
				  hud->score);
}

void desenha_lives (Hud *hud){
	int linha_vidas = 0;
	int coluna_vidas = 0;

	al_draw_text(hud->fonte, 
				 CINZA,
				 hud->lives_posicao_x,
				 hud->posicao_y,
				 0,
				 "LIVES:");

	for (int i = 0; i < hud->lives; i++) {
		if (hud->lives_posicao_x + (LARGURA_DISPLAY/12)*1.5 + hud->largura_sprite_vida*1.25*(coluna_vidas) > LARGURA_DISPLAY - hud->largura_sprite_vida) {
			linha_vidas++;
			coluna_vidas = 0;
		}

		al_draw_scaled_bitmap(hud->life,
							  0, 0,
							  al_get_bitmap_width(hud->life),
							  al_get_bitmap_height(hud->life),
							  hud->lives_posicao_x + (LARGURA_DISPLAY/12)*1.5 + hud->largura_sprite_vida*1.25*(coluna_vidas),
							  hud->posicao_y + linha_vidas*(hud->altura_sprite_vida*1.25),
							  hud->largura_sprite_vida, hud->altura_sprite_vida,
							  0);
		
		coluna_vidas++;
	}
}

bool verificar_letra_high_score(Hud* hud,char letra){
	if(((letra>='0' && letra <='9')  || ((letra>='A')&&(letra<='Z')) || ((letra>='a') && letra<='z')) && hud->nome_score[2]=='_' )
		return true;
	else if(int(letra)==8){
		apagar_caracter(hud);
		return false;
	}
	else
		return false;
}

void apagar_caracter(Hud* hud){
	for(int i=2;i>=0;i--){
		if(hud->nome_score[i]!='_'){
			hud->nome_score[i]='_';
			return;
		}
	}
}
void iniciar_salvar_score(Hud *hud){
	while(!al_is_event_queue_empty(hud->fila_eventos)){
		ALLEGRO_EVENT evento;
		al_wait_for_event(hud->fila_eventos, &evento);
		if(evento.type == ALLEGRO_EVENT_KEY_CHAR){
			char letra = evento.keyboard.unichar;
			inserir_caracter(hud, letra);
		}

	}
}
void inserir_caracter(Hud* hud, char letra){
	if(verificar_letra_high_score(hud,letra)){
		for(int i=0;i<3;i++){
			if(hud->nome_score[i]=='_'){
				hud->nome_score[i]=letra;
				return;
			}
		}
	}
}

void ler_high_score(Hud *hud){
	hud->arquivo_high_score = fopen("high_score.txt","r");
	if(hud->arquivo_high_score==NULL){
		puts("Erro ao abrir arquivo high_score.txt");
		exit(0);
	}
	for(int i=0; i<10;i++){
		fscanf(hud->arquivo_high_score," %c %c %c %d\n",&hud->nomes_high_score[i][0],&hud->nomes_high_score[i][1],&hud->nomes_high_score[i][2],&hud->scores_high_score[i]);
	}
	fclose(hud->arquivo_high_score);

}
void inicializar_high_score(Hud *hud){
    hud->fila_eventos = al_create_event_queue();
    al_register_event_source(hud->fila_eventos, al_get_keyboard_event_source());

}

void enviar_score(Hud *hud){
	ler_high_score(hud);
	if(hud->nome_score[0]!='_'){
		for(int i=0;i<3;i++){
			hud->nomes_high_score[9][i]=hud->nome_score[i];
		}
		hud->scores_high_score[9]=hud->score;
		ordenar_vetor(hud->scores_high_score,hud->nomes_high_score, 10);
		hud->arquivo_high_score = fopen("high_score.txt","w+");
		if(hud->arquivo_high_score==NULL){
			puts("Erro ao abrir arquivo high_score.txt");
			exit(0);
		}
		for(int i=0;i<10;i++){
			for(int j=0;j<3;j++){
				fprintf(hud->arquivo_high_score," %c ",hud->nomes_high_score[i][j]);
			}
			fprintf(hud->arquivo_high_score,"%d\n",hud->scores_high_score[i]);
		}
		fclose(hud->arquivo_high_score);
	}
}
void permutar_strings(char *nome1, char*nome2){
	char auxiliar[3];
	for(int i=0;i<3;i++){
		auxiliar[i]=nome1[i];
		nome1[i]=nome2[i];
		nome2[i]=auxiliar[i];
	}
}
void ordenar_vetor(int *vetor, char nomes[][3], int n_elementos){
	int i, j, chave;
	char chave2[3];
	for(j=1;j<n_elementos;j++){
		chave = vetor[j];
		permutar_strings(chave2,nomes[j]);
		i = j-1;
		while(chave > vetor[i] && i>=0){
			vetor[i+1]=vetor[i];
			permutar_strings(nomes[i+1],nomes[i]);
			i--;
		}
		vetor[i+1]=chave;
		permutar_strings(nomes[i+1],chave2);
	}

}




