#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "mothership.h"

void desenha_mothership(Mothership *mothership,Jogo *jogo, Projetil *projetil){
	if(autoriza_mothership(mothership,jogo) || mothership->status==1){
	 	 al_draw_bitmap(mothership->imagem, mothership->posicao_x,mothership->posicao_y,0);
	 	 if(mothership->posicao_x>=jogo->largura){
	 		 reinicia_mothership(mothership);
	 		 return;
	 	 }
	 	 detecta_colisao(mothership, jogo);
	 	 movimenta_mothership(mothership);

	}

}
void movimenta_mothership(Mothership *mothership){
	mothership->posicao_x +=mothership->velocidade;
}
void reinicia_mothership(Mothership *mothership){
	mothership->posicao_x = -35;
	mothership->posicao_y = 0;
	mothership->status = 0;
}
void inicializa_mothership(Mothership *mothership){
	mothership->imagem = al_load_bitmap("resources/mothership.png");
	if (mothership->imagem == NULL) {
			puts("Erro ao carregar o arquivo resources/mothership.png");
			exit(0);
	}
	mothership->fonte = al_load_font("resources/verdana.ttf",10,0);
	mothership->velocidade=3;
	mothership->largura=27;
	mothership->altura=19;
	reinicia_mothership(mothership);
}
void finaliza_mothership(Mothership *mothership){
	al_destroy_bitmap(mothership->imagem);
}
void detecta_colisao(Mothership *mothership, Jogo *jogo){
	for (int i = 0; i < jogo->numero_de_projeteis; i++) {
		if(!(jogo->projetil_stack[i].posicao_x > mothership->posicao_x + mothership->largura ||
						jogo->projetil_stack[i].posicao_y > mothership->posicao_y + mothership->altura  ||
						jogo->projetil_stack[i].posicao_y + 16 < mothership->posicao_y ||
						jogo->projetil_stack[i].posicao_x + 8 < mothership->posicao_x)
						)

		{
				copy_projetil(&jogo->projetil_stack[i], &jogo->projetil_stack[jogo->numero_de_projeteis-1]);
				desenha_projetil(&jogo->projetil_stack[i]);
				finaliza_projetil(&jogo->projetil_stack[jogo->numero_de_projeteis-1]);
				jogo->numero_de_projeteis--;
				al_draw_text(mothership->fonte,al_map_rgb(211,211,211), mothership->posicao_x, mothership->posicao_y + 25, 0, "+500");
				reinicia_mothership(mothership);

				return ;
		}

	}

}
int autoriza_mothership(Mothership *mothership, Jogo *jogo){
	if(((jogo->loop_count)/FPS)%10 == 0){
		mothership->status=1;
		return 1;
	}
	else
		return 0;
}
