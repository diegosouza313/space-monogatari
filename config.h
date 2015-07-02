#ifndef CONFIG_H
#define CONFIG_H

#define LARGURA_DISPLAY 800
#define ALTURA_DISPLAY 600
#define BARREIRA_LATERAL_DISPLAY 15
#define SCALE_BITMAPS true

//CORES
#define MARROM_ESCURO al_map_rgb(20, 20, 12)
#define MARROM_CLARO al_map_rgb(247, 180, 109)
#define AMARELO al_map_rgb(255, 180, 0)
#define BRANCO al_map_rgb(255, 255, 255)
#define CINZA al_map_rgb(211, 211, 211)
#define VERDE al_map_rgb(0, 250, 0)

//INPUT
#define N_KEYS 7
enum MYKEYS { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_Z, KEY_ESCAPE, KEY_ENTER };

//DIRECAO
enum DIRECAO { ESQUERDA, DIREITA, CIMA, BAIXO };
enum MODO { CLASSIC, INFINITE };

//PONTOS
#define PONTOS_MOTHERSHIP 500
#define PONTOS_ALIEN 200

//MENU
#define MENU_FPS 10
#define MAX_BOTOES 32
#define GAME_FPS 60
#define N_TELAS 10
enum TELA { PAUSE, OPTIONS, GAME_OVER, TITLE_SCREEN, RESOLUTIONS, LANGUAGES, MECHANICS, MODES };

//ALIEN
#define LINHAS_TROPA 5
#define COLUNAS_TROPA 10
#define DISTANCIA_PASSO_ALIEN 20
#define LARGURA_SPRITES_ALIEN 28
#define ALTURA_SPRITES_ALIEN 28

//MOTHERSHIP
#define LARGURA_SPRITES_MOTHERSHIP 25
#define ALTURA_SPRITES_MOTHERSHIP 22
#define FREQUENCIA_MOTHERSHIP 15
#define VELOCIDADE_MOTHERSHIP 3

//PLAYER
#define NUMERO_VIDAS 10
#define DISTANCIA_PASSO_PLAYER 5
#define LARGURA_SPRITES_PLAYER 48
#define ALTURA_SPRITES_PLAYER 48

//PROJETIL
#define PROJETIL_COOLDOWN 20
#define VELOCIDADE_PROJETIL 10
#define LARGURA_PROJETIL 8
#define ALTURA_PROJETIL 16
#define MAX_PROJETEIS 32

//SHIELD
#define NUMERO_SHIELDS 4
#define LARGURA_SHIELD 96
#define ALTURA_SHIELD 48
#define PARTES_X 4
#define PARTES_Y 2
enum ESTADO { INTEIRO, DANIFICADO, DESTRUIDO };

#endif
