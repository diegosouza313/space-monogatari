#include <stdio.h>
#include <allegro5/allegro.h>

#include "game.h"
#include "player.h"
#include "alien.h"

int main(int argc, char **argv) {

    int fase = 0;

    Jogo jogo;

	Alien alien[LINHAS_TROPA][COLUNAS_TROPA];

	inic_allegro();

   	inicializa_display (&jogo, LARGURA_DISPLAY, ALTURA_DISPLAY);

    // TITLE SCREEN
	tela_boot_jogo(&jogo);

	inicializa_menus(&jogo.menu, &jogo.hud);

	loop_menu(&jogo.menu, &jogo.hud, TITLE_SCREEN);

    while (jogo.menu.new_game == 1) {
    	inicializa_jogo (&jogo, fase);

    	if (loop_de_jogo (&jogo))
            fase++;

    	finaliza_jogo (&jogo);
    }

	finaliza_display (&jogo);

    return 0;
}
