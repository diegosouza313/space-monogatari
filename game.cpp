#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "game.h"

void inicializa_jogo (Jogo* jogo, int largura, int altura) {

	inic_allegro();
	inic_allegro_primitive();
	inic_allegro_image();
  al_init_font_addon(); 
  al_init_ttf_addon(); 

	jogo->largura = largura;
	jogo->altura = altura;
	jogo->event_queue = NULL;
	jogo->display = al_create_display(largura, altura);
	
	if (!jogo->display) {
    	fprintf(stderr, "Falha ao inicializar o display!\n");
		exit(-1);
	}

  desenha_fundo_jogo();
  inicializa_player(&jogo->player, jogo->largura/2.0, jogo->altura/12.0*10);
  inicializa_tropa(jogo->alien, jogo->largura/10, jogo->altura/12);
  inicializa_nave(&jogo->nave);
  inicializa_timer_jogo(jogo);
  inicializa_teclado(jogo);
  inicializa_event_queue_jogo(jogo);

  jogo->segundos=0;

}
	
void finaliza_jogo (Jogo* jogo) {
	finaliza_player (&jogo->player);
	finaliza_nave(&jogo->nave);
	al_destroy_display(jogo->display);
}

void desenha_jogo (Jogo* jogo) {
	desenha_fundo_jogo();

	desenha_player(&jogo->player);

    desenha_tropa(jogo->alien);

    desenha_nave(&jogo->nave,jogo);

  for (int i = 0; i < jogo->numero_de_projeteis; i++) {

    desenha_projetil(&jogo->projetil_stack[i]);
    
    if (jogo->projetil_stack[i].posicao_y < 0 - jogo->projetil_stack[i].altura_sprite) {
      copy_projetil(&jogo->projetil_stack[i], &jogo->projetil_stack[jogo->numero_de_projeteis-1]);
      desenha_projetil(&jogo->projetil_stack[i]);
      finaliza_projetil(&jogo->projetil_stack[jogo->numero_de_projeteis-1]);
      jogo->numero_de_projeteis--;
    }

    move_projetil(&jogo->projetil_stack[i]);
  }

	al_flip_display();
}

void loop_de_jogo (Jogo* jogo) {
	al_start_timer(jogo->timer);
	bool doexit = false;
	bool redraw = true;

	jogo->numero_de_projeteis = 0;
  int loop_count_projetil = jogo->player.projetil_cooldown;
  int loop_count_menu = 2;

	while (!doexit) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(jogo->event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {

			jogo->segundos++;


        	if (jogo->key[KEY_LEFT] && get_posicao_x_min_player(&jogo->player) > 0 + 15)
            	move_player(&jogo->player, ESQUERDA);

        	if (jogo->key[KEY_RIGHT] && get_posicao_x_max_player(&jogo->player) < jogo->largura - 15)
            	move_player(&jogo->player, DIREITA);
 
          if (jogo->key[KEY_ESCAPE] && loop_count_menu > 2) {
              inicializa_menu(&jogo->menu); 
              doexit = loop_menu(&jogo->menu);
              jogo->key[KEY_Z] = false;
              loop_count_menu = 0;
          }

        	redraw = true;
    	}
    	
    	else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        	break;
      	}

      	else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        	switch(ev.keyboard.keycode) {
            	case ALLEGRO_KEY_LEFT: 
            		jogo->key[KEY_LEFT] = true;
                	break;
 
            	case ALLEGRO_KEY_RIGHT:
               		jogo->key[KEY_RIGHT] = true;
               		break;

              case ALLEGRO_KEY_F1:
                  doexit = true;
                  break;

              case ALLEGRO_KEY_ESCAPE:
                  jogo->key[KEY_ESCAPE] = true;
                  break;

            	case ALLEGRO_KEY_Z:
               		jogo->key[KEY_Z] = true;
                  if (loop_count_projetil > jogo->player.projetil_cooldown && loop_count_menu > 2) {
                    loop_count_projetil = 0;
                    cria_projetil(&jogo->projetil_stack[jogo->numero_de_projeteis],
                                  get_posicao_x_centro_player(&jogo->player),
                                  jogo->player.posicao_y,
                                  CIMA);
                    jogo->numero_de_projeteis++;
                  }
               		break;
        }
    }

    	else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        	switch(ev.keyboard.keycode) {
            	case ALLEGRO_KEY_LEFT: 
               		jogo->key[KEY_LEFT] = false;
               		break;
 
            	case ALLEGRO_KEY_RIGHT:
               		jogo->key[KEY_RIGHT] = false;
               		break;

            	case ALLEGRO_KEY_Z:
               		jogo->key[KEY_Z] = false;
               		break;
        }
    }
 
    	if (redraw && al_is_event_queue_empty(jogo->event_queue)) {
          redraw = false;
          jogo->key[KEY_ESCAPE] = false;

          loop_count_projetil++;
          loop_count_menu++;

          desenha_jogo(jogo);

        	al_flip_display();
      	}
   }
}

void desenha_fundo_jogo () {
	al_clear_to_color(al_map_rgb(60,60,100));
}

void inicializa_teclado (Jogo* jogo) {
  if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao inicializar o teclado!\n");
        exit(-1);
  }

  for(int i = 0; i < N_KEYS; i++)
    jogo->key[i] = false;
}

void inicializa_event_queue_jogo (Jogo* jogo) {
	jogo->event_queue = al_create_event_queue();

	if(!jogo->event_queue) {
		fprintf(stderr, "Falha em executar lista de eventos!\n");
		exit(-1);
	}

	al_register_event_source(jogo->event_queue, al_get_keyboard_event_source());
	al_register_event_source(jogo->event_queue, al_get_timer_event_source(jogo->timer));
	al_register_event_source(jogo->event_queue, al_get_display_event_source(jogo->display));
}

void inicializa_timer_jogo (Jogo* jogo) {
	jogo->timer = al_create_timer(1.0/FPS);

	if(!jogo->timer){
		fprintf(stderr, "Falha em executar timer!\n");
		exit(-1);
	}
}

void inic_allegro (void) {
	if (!al_init()) {
  	    fprintf(stderr, "Falha ao inicializar o Allegro!\n");
  	    exit(-1);
	}
}

void inic_allegro_primitive (void) {
	if (!al_init_primitives_addon()) {
  	    fprintf(stderr, "Falha ao inicializar o Allegro Primitives!\n");
  	    exit(-1);
	}
}

void inic_allegro_image (void) {
	if (!al_init_image_addon()) {
  	    fprintf(stderr, "Falha ao inicializar o Allegro Image!\n");
  	    exit(-1);
	}
}
