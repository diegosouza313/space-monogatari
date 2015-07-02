#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "menu.h"

void inicializa_menus (Menu* menu, Hud* hud) {
    menu->event_queue = NULL;
    menu->timer = NULL;

    inicializa_timer_menu_pause (menu);
    inicializa_event_queue_menu_pause(menu);

	inicializar_high_score(hud);


    menu->botao_selecionado = 0;

    menu->font_size = ALTURA_DISPLAY/20;
    menu->font_title = al_load_font("resources/acknowledge.ttf", menu->font_size*2, 0);
    menu->font_subtitle = al_load_font("resources/acknowledge.ttf", menu->font_size*1.25, 0);
    menu->font_items = al_load_font("resources/acknowledge.ttf", menu->font_size, 0);

    if (menu->font_title == NULL || menu->font_items == NULL || menu->font_subtitle == NULL) {
        puts("Erro ao carregar o arquivo \"resources/acknowledge.ttf\"");
        exit(0);
    }

    for(int i = 0; i < N_KEYS; i++)
        menu->key[i] = false;
}

void desenha_fundo_menu_pause () {
	al_clear_to_color(MARROM_ESCURO);
}

void desenha_menu_pause (Menu* menu, Hud *hud) {
    desenha_fundo_menu_pause ();

    char tela_botao[N_TELAS - 1][MAX_BOTOES - 1][20];
    char titulo_tela[N_TELAS - 1][20];
    
    //PAUSE
    strcpy(titulo_tela[0], "PAUSE");
    strcpy(tela_botao[0][0], "RESUME");
    strcpy(tela_botao[0][1], "OPTIONS");
    strcpy(tela_botao[0][2], "TITLE SCREEN");
    strcpy(tela_botao[0][3], "QUIT GAME");
    menu->numero_de_botoes[0] = 3;

    //OPTIONS
    strcpy(titulo_tela[1], "PAUSE");
    strcpy(tela_botao[1][0], "RESOLUTIONS");
    strcpy(tela_botao[1][1], "LANGUAGES");
    strcpy(tela_botao[1][2], "MECHANICS");
    strcpy(tela_botao[1][3], "<===");
    menu->numero_de_botoes[1] = 3;

    //GAME_OVER
    strcpy(titulo_tela[2], "GAME OVER");
    strcpy(tela_botao[2][0], "NEW GAME");
    strcpy(tela_botao[2][1], "QUIT GAME");
    strcpy(tela_botao[2][2], "SALVAR SCORE");
    menu->numero_de_botoes[2] = 2;

    //TITLE_SCREEN
    strcpy(titulo_tela[3], "space monogatari");
    strcpy(tela_botao[3][0], "NEW GAME");
    strcpy(tela_botao[3][1], "HIGH SCORES");
    strcpy(tela_botao[3][2], "QUIT GAME");   
    menu->numero_de_botoes[3] = 2;
    
    //RESOLUTIONS
    strcpy(titulo_tela[4], "RESOLUTIONS");
    strcpy(tela_botao[4][0], "640 x 480");
    strcpy(tela_botao[4][1], "800 x 600");
    strcpy(tela_botao[4][2], "1024 x 768");
    strcpy(tela_botao[4][3], "<===");
    menu->numero_de_botoes[4] = 3;

    //LANGUAGES
    strcpy(titulo_tela[5], "LANGUAGES");
    strcpy(tela_botao[5][0], "PORTUGUESE");
    strcpy(tela_botao[5][1], "ENGLISH");
    strcpy(tela_botao[5][2], "CHINESE (ORIENTAL MANDARIN)");
    strcpy(tela_botao[5][3], "<===");
    menu->numero_de_botoes[5] = 3;

    //MECHANICS
    strcpy(titulo_tela[6], "MECHANICS");
    strcpy(tela_botao[6][0], "ALIEN MOVEMENT");
    strcpy(tela_botao[6][1], "PLAYER MOVEMENT");
    strcpy(tela_botao[6][2], "<===");
    menu->numero_de_botoes[6] = 2;

    //MODES
    strcpy(titulo_tela[7], "MODES");
    strcpy(tela_botao[7][0], "CLASSIC");
    strcpy(tela_botao[7][1], "INFINITE");
    strcpy(tela_botao[7][2], "<===");
    menu->numero_de_botoes[7] = 2;

    //SALVAR SCORE
    strcpy(titulo_tela[8],"SALVAR SCORE");
    strcpy(tela_botao[8][0],"SALVAR");
    menu->numero_de_botoes[8] = 0;

    //HIGH SCORES
    strcpy(titulo_tela[9],"HIGH SCORES");
    strcpy(tela_botao[9][0],"VOLTAR");
    menu->numero_de_botoes[9] = 0;

    for (int i = 0; i < N_TELAS - 1; i++) {
        if (menu->tela_selecionada == i && i != 2 && i!=8 ) {
            for (int j = 0; j < menu->numero_de_botoes[i] + 1; j++) {
                al_draw_text(menu->font_title, BRANCO, LARGURA_DISPLAY/2, (ALTURA_DISPLAY/4), ALLEGRO_ALIGN_CENTRE, titulo_tela[i]);
                
                if (menu->botao_selecionado == j)
                    al_draw_text(menu->font_items, MARROM_CLARO, LARGURA_DISPLAY/2, ALTURA_DISPLAY/2 + (menu->font_size+6)*j, ALLEGRO_ALIGN_CENTRE, tela_botao[i][j]);
                else
                    al_draw_text(menu->font_items, BRANCO, LARGURA_DISPLAY/2, ALTURA_DISPLAY/2 + (menu->font_size+6)*j, ALLEGRO_ALIGN_CENTRE, tela_botao[i][j]);
            }
        }
    }

    //CASO ESPECIAL: GAME_OVER
    if (menu->tela_selecionada == 2) {
    	if(hud->score<=hud->scores_high_score[9])
    				menu->numero_de_botoes[2]--;

        for (int j = 0; j < menu->numero_de_botoes[2] + 1; j++) {
            al_draw_text(menu->font_title, BRANCO, LARGURA_DISPLAY/2, (ALTURA_DISPLAY/4), ALLEGRO_ALIGN_CENTRE, titulo_tela[2]);
            al_draw_text(menu->font_subtitle, BRANCO, LARGURA_DISPLAY/2, (ALTURA_DISPLAY/4) + (menu->font_size*1.25)*2, ALLEGRO_ALIGN_CENTRE, "your score was:");
            al_draw_textf(menu->font_title, AMARELO, LARGURA_DISPLAY/2, (ALTURA_DISPLAY/4) + (menu->font_size*2)*2, ALLEGRO_ALIGN_CENTRE, "%d", menu->score);
            if (menu->botao_selecionado == j)
                al_draw_text(menu->font_items, MARROM_CLARO, LARGURA_DISPLAY/2, ALTURA_DISPLAY/2 + (menu->font_size)*(j+2), ALLEGRO_ALIGN_CENTRE, tela_botao[2][j]);
            else
                al_draw_text(menu->font_items, BRANCO, LARGURA_DISPLAY/2, ALTURA_DISPLAY/2 + (menu->font_size)*(j+2), ALLEGRO_ALIGN_CENTRE, tela_botao[2][j]);
        }
    }
    //CASO ESPECIAL: SALVAR_SCORE
    if(menu->tela_selecionada == 8 ){
    	iniciar_salvar_score(hud);
    	for (int j = 0; j < menu->numero_de_botoes[8] + 1; j++) {
    		al_draw_text(menu->font_title, BRANCO, LARGURA_DISPLAY/2, (ALTURA_DISPLAY/4), ALLEGRO_ALIGN_CENTRE, titulo_tela[8]);
    		al_draw_text(menu->font_subtitle, BRANCO, LARGURA_DISPLAY/2, (ALTURA_DISPLAY/4) + (menu->font_size*1.25)*2, ALLEGRO_ALIGN_CENTRE, "PLEASE ENTER YOUR INITIALS");
    		al_draw_textf(menu->font_title, VERDE, LARGURA_DISPLAY/2, (ALTURA_DISPLAY/4) + (menu->font_size*1.75)*2, ALLEGRO_ALIGN_CENTRE,"%c%c%c" ,hud->nome_score[0],hud->nome_score[1],hud->nome_score[2]);

    		if (menu->botao_selecionado == j)
    			al_draw_text(menu->font_items, MARROM_CLARO, LARGURA_DISPLAY/2, ALTURA_DISPLAY/2 + (menu->font_size)*(j+2), ALLEGRO_ALIGN_CENTRE, tela_botao[8][j]);
    		else
    			al_draw_text(menu->font_items, BRANCO, LARGURA_DISPLAY/2, ALTURA_DISPLAY/2 + (menu->font_size)*(j+2), ALLEGRO_ALIGN_CENTRE, tela_botao[8][j]);
    		}

    }

    //CASO ESPECIAL: HIGH_SCORES
    if(menu->tela_selecionada == 9 ){
        	ler_high_score(hud);
        	for (int j = 0; j < menu->numero_de_botoes[9] + 1; j++) {
        		al_draw_text(menu->font_title, BRANCO, LARGURA_DISPLAY/2, (ALTURA_DISPLAY/10), ALLEGRO_ALIGN_CENTRE, titulo_tela[9]);
        		for(int i=0;i<10;i++){
        			al_draw_textf(menu->font_subtitle, VERDE, LARGURA_DISPLAY/2, (ALTURA_DISPLAY/8) + (menu->font_size*1.25)*(2+i), ALLEGRO_ALIGN_CENTRE, "%d°...%c%c%c...%d",i+1,hud->nomes_high_score[i][0],hud->nomes_high_score[i][1],hud->nomes_high_score[i][2],hud->scores_high_score[i]);
        		}
        		if (menu->botao_selecionado == j)
        			al_draw_text(menu->font_items, MARROM_CLARO, LARGURA_DISPLAY/2, ALTURA_DISPLAY/1.2 + (menu->font_size)*(j+2), ALLEGRO_ALIGN_CENTRE, tela_botao[9][j]);
        		else
        			al_draw_text(menu->font_items, BRANCO, LARGURA_DISPLAY/2, ALTURA_DISPLAY/1.2 + (menu->font_size)*(j+2), ALLEGRO_ALIGN_CENTRE, tela_botao[9][j]);
        		}

        }
}


bool loop_menu (Menu* menu, Hud* hud, TELA tela) {
    al_start_timer(menu->timer);
    ALLEGRO_EVENT ev;

    menu->score = hud->score;
    menu->tela_selecionada = tela;

    bool doexit = false;
    bool redraw = true;

    while (!doexit) {
        al_wait_for_event(menu->event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
            redraw = true;

        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(ev.keyboard.keycode) {

                case ALLEGRO_KEY_UP:
                    navega_botoes(menu, CIMA);
                    break;

                case ALLEGRO_KEY_DOWN:
                    navega_botoes(menu, BAIXO);
                    break;

                case ALLEGRO_KEY_F1:
                    return true;
                    break;

                case ALLEGRO_KEY_ESCAPE:
                    return false;
                    break;

                case ALLEGRO_KEY_ENTER: case ALLEGRO_KEY_Z:
                    switch (menu->tela_selecionada) {
                        case PAUSE:
                            switch (menu->botao_selecionado) {
                                case 0:
                                    return false;
                                    break;
                                case 1:
                                    seleciona_nova_tela(menu, OPTIONS);
                                    break;
                                case 2:
                                    seleciona_nova_tela(menu, TITLE_SCREEN);
                                    break;                      
                                case 3:
                                    return true;
                                    break;
                            }
                            break;

                        case OPTIONS:
                            switch (menu->botao_selecionado) {
                                case 0:
                                    seleciona_nova_tela(menu, RESOLUTIONS);
                                    break;
                                case 1:
                                    seleciona_nova_tela(menu, LANGUAGES);
                                    break;
                                case 2:
                                    seleciona_nova_tela(menu, MECHANICS);
                                    break;
                                case 3:
                                    seleciona_nova_tela(menu, PAUSE);
                                    break;
                            }  
                            break;

                        case GAME_OVER:
                            switch (menu->botao_selecionado) {
                                case 0:
                                    menu->new_game = 1;
                                    seleciona_nova_tela(menu, MODES);
                                    break;
                                case 1:
                                    return true;
                                    break;
                                case 2:
                                	seleciona_nova_tela(menu, SALVAR_SCORE);
                                	break;
                            }
                            break;

                        case TITLE_SCREEN:
                            switch (menu->botao_selecionado) {
                                case 0:
                                    menu->new_game = 1;
                                    seleciona_nova_tela(menu, MODES);
                                    break;
                                case 1:
                                    seleciona_nova_tela(menu, HIGH_SCORES);
                                    break;
                                case 2:
                                    return true;
                                    break;   
                            }  
                            break;

                        case RESOLUTIONS:
                            switch (menu->botao_selecionado) {
                                case 0:
                                    menu->new_game = 1;
                                    return true;
                                    break;
                                case 1:
                                    return true;
                                    break;
                                case 2:
                                    return true;
                                    break;
                                case 3:
                                    seleciona_nova_tela(menu, OPTIONS);
                                    break;   
                            } 
                            break;

                        case LANGUAGES:
                            switch (menu->botao_selecionado) {
                                case 0:
                                    menu->new_game = 1;
                                    return true;
                                    break;
                                case 1:
                                    return true;
                                    break;
                                case 2:
                                    return true;
                                    break;
                                case 3:
                                    seleciona_nova_tela(menu, OPTIONS);
                                    break;   
                            } 
                            break;

                        case MECHANICS:
                            switch (menu->botao_selecionado) {
                                case 0:
                                    menu->new_game = 1;
                                    return true;
                                    break;
                                case 1:
                                    return true;
                                    break;
                                case 2:
                                    seleciona_nova_tela(menu, OPTIONS);
                                    break;
                            } 
                            break;     

                        case MODES:
                            switch (menu->botao_selecionado) {
                                case 0:
                                    menu->modo = CLASSIC;
                                    return true;
                                    break;
                                case 1:
                                    menu->modo = INFINITE;
                                    return true;
                                    break;
                                case 2:
                                    seleciona_nova_tela(menu, TITLE_SCREEN);
                                    break;
                            } 
                            break;
                        case SALVAR_SCORE:
                        		switch(menu->botao_selecionado){
                        				case 0:
                        					ler_high_score(hud);
                        					if( hud->nome_score[2]!='_' ){
                        						enviar_score(hud);
                        						seleciona_nova_tela(menu, HIGH_SCORES);
                        					}
                        					break;
                        				case 1:
                        					seleciona_nova_tela(menu, TITLE_SCREEN);
                        					break;


                        	}
                        	break;
                        case HIGH_SCORES:
                        	switch(menu->botao_selecionado){
                        		case 0:
                        			seleciona_nova_tela(menu, TITLE_SCREEN);
                        			break;

                        	}
                        	break;

                    }
                    break; break;
            }                  
        }

        if (redraw && al_is_event_queue_empty(menu->event_queue)) {
           redraw = false;

           desenha_menu_pause(menu, hud);

           al_flip_display();
        }
    }
}

void seleciona_nova_tela (Menu* menu, TELA tela) {
    menu->tela_selecionada = tela;
    menu->botao_selecionado = 0;
}

void navega_botoes (Menu* menu, DIRECAO direcao) {
    if (direcao == CIMA) {
        if (menu->botao_selecionado == 0)
            menu->botao_selecionado = menu->numero_de_botoes[menu->tela_selecionada];
        else
            menu->botao_selecionado--;
    }

    if (direcao == BAIXO) {
        if (menu->botao_selecionado == menu->numero_de_botoes[menu->tela_selecionada])
            menu->botao_selecionado = 0;
        else
            menu->botao_selecionado++;
    }
}

void inicializa_event_queue_menu_pause (Menu* menu) {
	menu->event_queue = al_create_event_queue();

	if(!menu->event_queue) {
		fprintf(stderr, "Falha em executar lista de eventos do menu!\n");
		exit(-1);
	}

    al_install_keyboard();
    al_register_event_source(menu->event_queue, al_get_keyboard_event_source());
    al_register_event_source(menu->event_queue, al_get_timer_event_source(menu->timer));
}

void inicializa_timer_menu_pause (Menu* menu) {
	menu->timer = al_create_timer(1.0/MENU_FPS);

	if(!menu->timer){
		fprintf(stderr, "Falha em executar timer do menu!\n");
		exit(-1);
	}
}
