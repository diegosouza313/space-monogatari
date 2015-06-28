#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "mothership.h"

void desenha_mothership(Mothership *mothership){

	disparar_timer_mothership(mothership);

	if(autoriza_mothership(mothership) || mothership->status==1){
	 	 al_draw_bitmap(mothership->imagem, mothership->posicao_x,mothership->posicao_y,0);
	 	 if(mothership->posicao_x>=mothership->largura_tela){
	 		 reinicia_mothership(mothership);
	 		 return;
	 	 }
	 	 movimenta_mothership(mothership);

	}

}
void movimenta_mothership(Mothership *mothership){
	mothership->posicao_x +=mothership->velocidade;
}
void reinicia_mothership(Mothership *mothership){
	mothership->posicao_x = -mothership->largura_sprite;
	mothership->posicao_y = mothership->altura_sprite;
	mothership->status = 0;
}
void inicializa_mothership(Mothership *mothership, Jogo *jogo){
	mothership->imagem = al_load_bitmap("resources/mothership.png");
	if (mothership->imagem == NULL) {
			puts("Erro ao carregar o arquivo resources/mothership.png");
			exit(0);
	}
	mothership->fonte = al_load_font("resources/verdana.ttf",10,0);
	mothership->velocidade=3;
	mothership->frequencia=10;
	mothership->segundos=0;
	mothership->largura_tela = jogo->largura;
	mothership->largura_sprite=27;
	mothership->altura_sprite=19;
	reinicia_mothership(mothership);
	inicializar_timer_mothership(mothership);
}
void inicializar_timer_mothership(Mothership *mothership){
    mothership->fila_evento = al_create_event_queue();
    mothership->tempo = al_create_timer(1.0);
    al_register_event_source(mothership->fila_evento, al_get_timer_event_source(mothership->tempo));
	al_start_timer(mothership->tempo);
}
void disparar_timer_mothership(Mothership *mothership){
	if (!al_is_event_queue_empty(mothership->fila_evento)){
	            ALLEGRO_EVENT evento;
	            al_wait_for_event(mothership->fila_evento, &evento);

	            if (evento.type == ALLEGRO_EVENT_TIMER)
	                mothership->segundos++;
	 }
}
void finaliza_mothership(Mothership *mothership){
	al_destroy_bitmap(mothership->imagem);
}

int autoriza_mothership(Mothership *mothership){
	if( mothership->segundos % mothership->frequencia==0 && mothership->status==0){
		mothership->status=1;
		return 1;
	}
	else
		return 0;
}
