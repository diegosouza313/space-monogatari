#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "game.h"

void inicializa_display (Jogo* jogo, int largura, int altura) {
    jogo->largura = largura;
    jogo->altura = altura;

    jogo->display = al_create_display(largura, altura);
 
    if (!jogo->display) {
        fprintf(stderr, "Falha ao inicializar o display!\n");
        exit(-1);
    }
}

void finaliza_display (Jogo* jogo) {
    al_destroy_display(jogo->display);
}

void inicializa_jogo (Jogo* jogo) {
    jogo->event_queue = NULL;
    jogo->menu.new_game = 1;

    jogo->numero_de_projeteis = 0;
    jogo->loop_count_projetil = jogo->player.projetil_cooldown;
    jogo->loop_count_menu_pause = 1;
    jogo->menu.new_game = 0;
    jogo->loop_alien_movement = 0;
    jogo->loop_alien_shots = 0;
    jogo->numero_shields = 4;

    jogo->shields = (Shield*) malloc(sizeof(Shield) * jogo->numero_shields);

    inicializa_teclado_jogo(jogo);
    inicializa_timer_jogo(jogo);
    inicializa_event_queue_jogo(jogo);

    jogo->fundo = al_load_bitmap("resources/fundo.png");
    if (jogo->fundo == NULL) {
        puts("Erro ao carregar o arquivo resources/fundo.png");
        exit(0);
    }

    inicializa_player(&jogo->player, jogo->largura/2.0, jogo->altura/12.0*10);
    inicializa_tropa(jogo->alien, jogo->largura/10, jogo->altura/12);
    inicializa_mothership(&jogo->mothership, jogo);
    inicializa_hud(&jogo->hud);
    inicializa_tropa(jogo->alien, ((jogo->largura - 20) - (1.5*LARGURA_SPRITES_ALIEN*COLUNAS_TROPA - LARGURA_SPRITES_ALIEN/2)) / 2 , 1.5*ALTURA_SPRITES_ALIEN);
    
    for (int i = 0; i < jogo->numero_shields; i++) {
        inicializa_shield(&jogo->shields[i],
                          (((LARGURA_DISPLAY - 2*15) / 4 ) + 20)*i + 15,
                          (ALTURA_DISPLAY / 12.0) * 8.75);
    }
    //- (1.5*altura_aliens) * (LINHAS_TROPA - 1));
    //(jogo->largura % (3*jogo->alien[0][0].delta_x*COLUNAS_TROPA - jogo->alien[0][0].delta_x))/2
    //jogo->altura - 1.5*2*jogo->alien[0][0].delta_y*LINHAS_TROPA + 1.5*2*jogo->alien[0][0].delta_y)
}

void finaliza_jogo (Jogo* jogo) {
    free(jogo->shields);
	finaliza_player(&jogo->player);
	finaliza_mothership(&jogo->mothership);
}

void desenha_jogo (Jogo* jogo) {
    desenha_fundo_jogo(jogo);
    desenha_player(&jogo->player);
    desenha_mothership(&jogo->mothership);
    desenha_hud(&jogo->hud);

    for (int i = 0; i < jogo->numero_shields; i++)
        desenha_shield(&jogo->shields[i]);

    for (int i = 0; i < jogo->numero_de_projeteis; i++) {
        desenha_projetil(&jogo->projetil_stack[i]);
        move_projetil(&jogo->projetil_stack[i]);
        
        if (jogo->projetil_stack[i].posicao_y < 0 - jogo->projetil_stack[i].altura_sprite) {
            copy_projetil(&jogo->projetil_stack[i], &jogo->projetil_stack[jogo->numero_de_projeteis-1]);
            desenha_projetil(&jogo->projetil_stack[i]);
            finaliza_projetil(&jogo->projetil_stack[jogo->numero_de_projeteis-1]);
            jogo->numero_de_projeteis--;
        }
    }

    desenha_tropa(jogo->alien);
    
	al_flip_display();
}

void loop_de_jogo (Jogo* jogo) {
    al_start_timer(jogo->timer);
    bool doexit = false;
    bool redraw = true;

    while (!doexit) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(jogo->event_queue, &ev);

        /* AQUI VAO FICAR OS TESTES DE COLISAO */
        /*---------------------------------------------------------------*/
        colisao_mothership_vs_projetil(jogo);
        for (int i = 0; i < jogo->numero_shields; i++)
            colisao_shield_vs_projetil (jogo, &jogo->shields[i]);
        /*---------------------------------------------------------------*/

		if (ev.type == ALLEGRO_EVENT_TIMER) {
        	redraw = true;
    
            if (jogo->key[KEY_LEFT] && get_posicao_x_min_player(&jogo->player) > 0 + BARREIRA_SEPARACAO_PLAYER_PAREDE)
                move_player(&jogo->player, ESQUERDA);

            if (jogo->key[KEY_RIGHT] && get_posicao_x_max_player(&jogo->player) < jogo->largura - BARREIRA_SEPARACAO_PLAYER_PAREDE)
                move_player(&jogo->player, DIREITA);
 
            if (jogo->key[KEY_ESCAPE] && jogo->loop_count_menu_pause > 1) {
                inicializa_menus(&jogo->menu);
                doexit = loop_menu(&jogo->menu, PAUSE);
                jogo->key[KEY_Z] = false;
                jogo->loop_count_menu_pause = 0;
            }

            if (jogo->key[KEY_Z]
                && jogo->loop_count_projetil > jogo->player.projetil_cooldown
                && jogo->loop_count_menu_pause > 1) {
                jogo->loop_count_projetil = 0;
                cria_projetil(&jogo->projetil_stack[jogo->numero_de_projeteis],
                              get_posicao_x_centro_player(&jogo->player),
                              jogo->player.posicao_y,
                              CIMA);
                jogo->numero_de_projeteis++;
            }
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
                    //F1 é o hard exit
                    doexit = true;
                    break;

                case ALLEGRO_KEY_ESCAPE:
                    jogo->key[KEY_ESCAPE] = true;
                    break;

            	case ALLEGRO_KEY_Z:
               		jogo->key[KEY_Z] = true;
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

			if (!(jogo->loop_alien_movement % (GAME_FPS/2)))
				rota_tropa (jogo->alien, jogo);

			if (!(jogo->loop_alien_shots % (GAME_FPS/2*3))) {
				atira_tropa (jogo->alien, &jogo->projetil_stack[jogo->numero_de_projeteis]);
				jogo->numero_de_projeteis++;
			}

			incremento_loop_elementos_jogo(jogo);

			desenha_jogo(jogo);

			al_flip_display();
		}
    }
}

void incremento_loop_elementos_jogo (Jogo* jogo) {
	jogo->loop_count_projetil++;
	jogo->loop_count_menu_pause++;
	jogo->loop_alien_movement++;
	jogo->loop_alien_shots++;
}

void desenha_fundo_jogo (Jogo* jogo) {
	al_clear_to_color(al_map_rgb(20,20,20));
	// al_draw_bitmap (jogo->fundo, 0, 0, 0);
}

void tela_boot_jogo (Jogo* jogo) {
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_EVENT ev;

	int alfa = 0;
	int velocidade = 5;

	ALLEGRO_BITMAP* minerva = al_load_bitmap("resources/minerva.png");
	if (minerva == NULL) {
		puts("Erro ao carregar o arquivo \"resources/minerva.png\"");
		exit(0);
	}

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 30);

	al_register_event_source(event_queue, al_get_display_event_source(jogo->display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_install_keyboard();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_rest(1);

	al_start_timer(timer);

	while (true) {
		alfa += velocidade;

		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {

			if (alfa > 0 && alfa < 255) {
				al_clear_to_color(al_map_rgb(0,0,0));
				al_draw_tinted_bitmap(minerva,
									  al_map_rgba(alfa, alfa, alfa, alfa),
									  (jogo->largura - al_get_bitmap_width(minerva))/2,
									  (jogo->altura - al_get_bitmap_height(minerva))/2,
									  0);
				al_flip_display();
			}

			if (alfa > 350)
				velocidade = -velocidade;
			if (alfa < 0)
				break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			break;
		}
	}

	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(minerva);
}

void inicializa_teclado_jogo (Jogo* jogo) {
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
	jogo->timer = al_create_timer(1.0/GAME_FPS);

	if(!jogo->timer){
		fprintf(stderr, "Falha em executar timer!\n");
		exit(-1);
	}
}

void inic_allegro (void) {
    inic_biblioteca_allegro();
    inic_biblioteca_allegro_primitive();
    inic_biblioteca_allegro_image();
    inic_biblioteca_allegro_font(); 
    inic_biblioteca_allegro_ttf(); 
}

void inic_biblioteca_allegro (void) {
	if (!al_init()) {
  	    fprintf(stderr, "Falha ao inicializar o Allegro!\n");
  	    exit(-1);
	}
}

void inic_biblioteca_allegro_primitive (void) {
	if (!al_init_primitives_addon()) {
  	    fprintf(stderr, "Falha ao inicializar o Allegro Primitives!\n");
  	    exit(-1);
	}
}

void inic_biblioteca_allegro_image (void) {
	if (!al_init_image_addon()) {
  	    fprintf(stderr, "Falha ao inicializar o Allegro Image!\n");
  	    exit(-1);
	}
}

void inic_biblioteca_allegro_font (void) {
	al_init_font_addon();
}

void inic_biblioteca_allegro_ttf (void) {
  if (!al_init_ttf_addon()) {
        fprintf(stderr, "Falha ao inicializar o Allegro TrueType!\n");
        exit(-1);
  }
}
