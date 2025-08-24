//------------CABECALHOS--------------------------------------

//inclui o cabeçalho da biblioteca Allegro 5
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "objetos.h"
#include <stdbool.h>
#include <time.h>

//define o FPS
#define FPS 60

//----------------VARIAVEIS GLOBAIS-------------------------
const int largura_t = 640;
const int altura_t = 480;
const int NUM_TIRO = 6;
const int NUM_INIMIGOS = 18;
const int MAX_INIMIGOS_POR_COLUNA = 3;

int aliados = 0;
int invasores = 0;
int contCaidos[6] = {0}; // Contador de inimigos por coluna
int colidiu = 0;
int gameState = 0; // 0 = menu, 1 = jogando, 2 = game over
int spawnTimer = 0;
int spawnDelay = 120; // Frames entre spawns (2 segundos a 60 FPS)

// Audio
ALLEGRO_SAMPLE *trilha_sonora = NULL;
ALLEGRO_SAMPLE *laser = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_trilha_sonora = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_laser = NULL;

// Bitmaps
ALLEGRO_BITMAP* fundo = NULL;
ALLEGRO_BITMAP* bala = NULL;
ALLEGRO_BITMAP* laranja = NULL;
ALLEGRO_BITMAP* azul = NULL;
ALLEGRO_BITMAP* fotonave = NULL;

// Fonts
ALLEGRO_FONT* fontetitulo = NULL;
ALLEGRO_FONT* fontpontos = NULL;

// Dispositivos
ALLEGRO_DISPLAY* janela = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;

// Controle de teclas
bool teclas[4] = {false, false, false, false};
bool fim = false;
bool desenha = false;

enum TECLAS {
    ESQUERDA, DIREITA, SPACE, ENTER
};

// --------------PROTOTIPOS---------------------------------
void error_msg(char* text);
void InitNave(NAVE* nave);
void MoveNaveEsquerda(NAVE* nave);
void MoveNaveDireita(NAVE* nave);
void InitTiro(TIRO tiro[], int tamanho);
void AtiraTiro(TIRO tiro[], int tamanho, NAVE* nave);
void AtualizaTiro(TIRO tiro[], int tamanho);
void DesenhaTiro(TIRO tiro[], int tamanho);
void TiroColidido(TIRO tiro[], int tamanhot, INIMIGOS inimigos[], int tamanhoi);
void InitInimigos(INIMIGOS inimigos[], int tamanho);
void LiberaInimigos(INIMIGOS inimigos[], int tamanho);
void AtualizarInimigos(INIMIGOS inimigos[], int tamanho);
void DesenhaInimigos(INIMIGOS inimigos[], int tamanho);
void DesenhaAliados(INIMIGOS inimigos[], int tamanho);
void ResetGame();
void DrawMenu();
void DrawGameOver();

// Função de erro
void error_msg(char* text) {
    al_show_native_message_box(NULL, "ERRO",
        "Ocorreu o seguinte erro e o programa sera finalizado:",
        text, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

int main() {
    // Inicialização do Allegro
    if (!al_init()) {
        error_msg("Falha ao inicializar Allegro");
        return -1;
    }

    // Inicialização dos add-ons
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10);

    // Configuração da janela
    janela = al_create_display(largura_t, altura_t);
    if (!janela) {
        error_msg("Falha ao criar display");
        return -1;
    }
    al_set_window_title(janela, "Grid Invaders");

    // Carregamento de fontes
    fontetitulo = al_load_font("SPACEBAR.ttf", 36, 0);
    if (!fontetitulo) {
        error_msg("Falha ao carregar fonte SPACEBAR.ttf");
        return -1;
    }
    
    fontpontos = al_load_font("arial.ttf", 14, 0);
    if (!fontpontos) {
        error_msg("Falha ao carregar fonte arial.ttf");
        return -1;
    }

    // Timer
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        error_msg("Falha ao criar temporizador");
        return -1;
    }

    // Carregamento de imagens
    fundo = al_load_bitmap("space.bmp");
    if (!fundo) {
        error_msg("Falha ao carregar fundo");
        return -1;
    }
    
    fotonave = al_load_bitmap("fotonave.png");
    bala = al_load_bitmap("bala-1.png");
    laranja = al_load_bitmap("laranja.png");
    azul = al_load_bitmap("azul.png");

    // Carregamento de áudio
    trilha_sonora = al_load_sample("trilha_sonora.ogg");
    laser = al_load_sample("laser.wav");
    
    if (trilha_sonora) {
        inst_trilha_sonora = al_create_sample_instance(trilha_sonora);
        al_attach_sample_instance_to_mixer(inst_trilha_sonora, al_get_default_mixer());
        al_set_sample_instance_playmode(inst_trilha_sonora, ALLEGRO_PLAYMODE_LOOP);
        al_set_sample_instance_gain(inst_trilha_sonora, 0.5);
    }
    
    if (laser) {
        inst_laser = al_create_sample_instance(laser);
        al_attach_sample_instance_to_mixer(inst_laser, al_get_default_mixer());
    }

    // Fila de eventos
    fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        error_msg("Falha ao criar fila de eventos");
        return -1;
    }

    // Registro de eventos
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    // Inicialização do jogo
    srand(time(NULL));
    InitNave(&nave);
    InitInimigos(inimigos, NUM_INIMIGOS);
    InitTiro(tiro, NUM_TIRO);

    // Iniciar timer e música
    al_start_timer(timer);
    if (inst_trilha_sonora) {
        al_play_sample_instance(inst_trilha_sonora);
    }

    // Loop principal
    while (!fim) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            fim = true;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (evento.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    teclas[ESQUERDA] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[DIREITA] = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    teclas[SPACE] = true;
                    if (gameState == 1) {
                        AtiraTiro(tiro, NUM_TIRO, &nave);
                    }
                    break;
                case ALLEGRO_KEY_ENTER:
                    if (gameState == 0) {
                        gameState = 1;
                        ResetGame();
                    } else if (gameState == 2) {
                        gameState = 0;
                        ResetGame();
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    fim = true;
                    break;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            switch (evento.keyboard.keycode) {
                case ALLEGRO_KEY_RIGHT:
                    teclas[DIREITA] = false;
                    break;
                case ALLEGRO_KEY_LEFT:
                    teclas[ESQUERDA] = false;
                    break;
                case ALLEGRO_KEY_SPACE:
                    teclas[SPACE] = false;
                    break;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_TIMER) {
            desenha = true;

            if (gameState == 1) {
                // Movimento da nave
                if (teclas[DIREITA])
                    MoveNaveDireita(&nave);
                if (teclas[ESQUERDA])
                    MoveNaveEsquerda(&nave);

                // Atualização de tiros
                AtualizaTiro(tiro, NUM_TIRO);

                // Spawn de inimigos com delay
                spawnTimer++;
                if (spawnTimer >= spawnDelay) {
                    LiberaInimigos(inimigos, NUM_INIMIGOS);
                    spawnTimer = 0;
                }

                // Atualização de inimigos
                AtualizarInimigos(inimigos, NUM_INIMIGOS);
                TiroColidido(tiro, NUM_TIRO, inimigos, NUM_INIMIGOS);

                // Verificar game over
                int totalCaidos = 0;
                for (int i = 0; i < 6; i++) {
                    totalCaidos += contCaidos[i];
                }
                
                if (totalCaidos >= NUM_INIMIGOS) {
                    gameState = 2;
                }
            }
        }

        // Renderização
        if (desenha && al_is_event_queue_empty(fila_eventos)) {
            if (gameState == 0) {
                DrawMenu();
            } else if (gameState == 1) {
                // Desenhar jogo
                al_draw_bitmap(fundo, 0, 0, 0);
                al_draw_bitmap(fotonave, nave.x, 400, 0);
                DesenhaTiro(tiro, NUM_TIRO);
                
                if (colidiu == 0) {
                    DesenhaInimigos(inimigos, NUM_INIMIGOS);
                } else {
                    DesenhaAliados(inimigos, NUM_INIMIGOS);
                }

                // Trincheira
                al_draw_rectangle(20, 280, 620, 380, al_map_rgb(0, 255, 0), 3);
                for (int i = 1; i < 6; i++) {
                    al_draw_line(20 + i * 100, 280, 20 + i * 100, 380, al_map_rgb(0, 255, 0), 3);
                }

                // UI
                al_draw_text(fontetitulo, al_map_rgb(255, 255, 0), 320, 10, ALLEGRO_ALIGN_CENTRE, "GRID INVADERS");
                al_draw_textf(fontpontos, al_map_rgb(0, 255, 0), 530, 16, ALLEGRO_ALIGN_LEFT, "ALIADOS: %d", aliados);
                al_draw_textf(fontpontos, al_map_rgb(255, 0, 0), 530, 33, ALLEGRO_ALIGN_LEFT, "INVASORES: %d", invasores);
                
                // Contadores
                for (int i = 0; i < 6; i++) {
                    al_draw_textf(fontpontos, al_map_rgb(0, 255, 0), 320, 180 + i * 20, ALLEGRO_ALIGN_LEFT, "Cont %d: %d", i, contCaidos[i]);
                }
            } else if (gameState == 2) {
                DrawGameOver();
            }

            desenha = false;
            al_flip_display();
        }
    }

    // Limpeza
    al_destroy_font(fontpontos);
    al_destroy_font(fontetitulo);
    al_destroy_bitmap(fotonave);
    al_destroy_bitmap(laranja);
    al_destroy_bitmap(bala);
    al_destroy_bitmap(azul);
    al_destroy_bitmap(fundo);
    al_destroy_sample(trilha_sonora);
    al_destroy_sample(laser);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);
    al_destroy_sample_instance(inst_trilha_sonora);
    al_destroy_sample_instance(inst_laser);

    return 0;
}

void DrawMenu() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(fontetitulo, al_map_rgb(255, 255, 0), 320, 150, ALLEGRO_ALIGN_CENTRE, "GRID INVADERS");
    al_draw_text(fontpontos, al_map_rgb(255, 255, 255), 320, 250, ALLEGRO_ALIGN_CENTRE, "Pressione ENTER para jogar");
    al_draw_text(fontpontos, al_map_rgb(255, 255, 255), 320, 280, ALLEGRO_ALIGN_CENTRE, "Setas: Mover | Espaco: Atirar | ESC: Sair");
}

void DrawGameOver() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(fontetitulo, al_map_rgb(255, 255, 255), 320, 150, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
    al_draw_textf(fontpontos, al_map_rgb(0, 255, 0), 180, 200, ALLEGRO_ALIGN_LEFT, "ALIADOS: %d", aliados);
    al_draw_textf(fontpontos, al_map_rgb(255, 0, 0), 350, 200, ALLEGRO_ALIGN_LEFT, "INVASORES: %d", invasores);
    
    if (aliados > invasores) {
        al_draw_text(fontetitulo, al_map_rgb(0, 255, 0), 320, 230, ALLEGRO_ALIGN_CENTRE, "VOCE VENCEU!");
    } else if (aliados == invasores) {
        al_draw_text(fontetitulo, al_map_rgb(255, 255, 0), 320, 230, ALLEGRO_ALIGN_CENTRE, "EMPATE!");
    } else {
        al_draw_text(fontetitulo, al_map_rgb(255, 0, 0), 320, 230, ALLEGRO_ALIGN_CENTRE, "VOCE PERDEU!");
    }
    
    al_draw_text(fontpontos, al_map_rgb(255, 255, 255), 320, 280, ALLEGRO_ALIGN_CENTRE, "Pressione ENTER para voltar ao menu");
}

void ResetGame() {
    aliados = 0;
    invasores = 0;
    colidiu = 0;
    spawnTimer = 0;
    
    for (int i = 0; i < 6; i++) {
        contCaidos[i] = 0;
    }
    
    InitNave(&nave);
    InitInimigos(inimigos, NUM_INIMIGOS);
    InitTiro(tiro, NUM_TIRO);
}

// TIRO
void InitTiro(TIRO tiro[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        tiro[i].movimento = 4;
        tiro[i].pontocentral_x = tiro[i].x + 5;
        tiro[i].pontocentral_y = tiro[i].y + 5;
        tiro[i].borda_x = 9;
        tiro[i].borda_y = 9;
        tiro[i].ativo = false;
    }
}

void AtiraTiro(TIRO tiro[], int tamanho, NAVE* nave) {
    for (int i = 0; i < tamanho; i++) {
        if (!tiro[i].ativo) {
            if (inst_laser) {
                al_stop_sample_instance(inst_laser);
                al_play_sample_instance(inst_laser);
            }
            tiro[i].x = nave->x + 34;
            tiro[i].y = nave->y;
            tiro[i].pontocentral_x = tiro[i].x;
            tiro[i].pontocentral_y = tiro[i].y;
            tiro[i].ativo = true;
            break;
        }
    }
}

void AtualizaTiro(TIRO tiro[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (tiro[i].ativo) {
            tiro[i].y -= tiro[i].movimento;
            if (tiro[i].y < 60) {
                tiro[i].ativo = false;
            }
        }
    }
}

void DesenhaTiro(TIRO tiro[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (tiro[i].ativo) {
            al_draw_bitmap(bala, tiro[i].x, tiro[i].y, 0);
        }
    }
}

void TiroColidido(TIRO tiro[], int tamanhot, INIMIGOS inimigos[], int tamanhoi) {
    for (int i = 0; i < tamanhot; i++) {
        if (tiro[i].ativo) {
            for (int j = 0; j < tamanhoi; j++) {
                if (inimigos[j].ativo) {
                    if (inimigos[j].y < 280 && 
                        tiro[i].x < (inimigos[j].x + inimigos[j].borda_x) &&
                        tiro[i].x + tiro[i].borda_x > inimigos[j].x &&
                        tiro[i].y < (inimigos[j].y + inimigos[j].borda_y) &&
                        tiro[i].y + tiro[i].borda_y > inimigos[j].y) {
                        
                        tiro[i].ativo = false;
                        inimigos[j].ativo = false;
                        colidiu = 1;
                        aliados++;
                        invasores++;
                        break;
                    }
                }
            }
        }
    }
}

// INIMIGOS
void InitInimigos(INIMIGOS inimigos[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        inimigos[i].movimento = 1.4;
        inimigos[i].pontocentral_x = inimigos[i].x + 22;
        inimigos[i].pontocentral_y = inimigos[i].y + 22;
        inimigos[i].borda_x = 44;
        inimigos[i].borda_y = 44;
        inimigos[i].ativo = false;
    }
}

void LiberaInimigos(INIMIGOS inimigos[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (!inimigos[i].ativo) {
            int coluna = rand() % 6;
            if (contCaidos[coluna] < MAX_INIMIGOS_POR_COLUNA) {
                contCaidos[coluna]++;
                inimigos[i].x = coluna * 100 + 47;
                inimigos[i].y = 60;
                inimigos[i].ativo = true;
                break;
            }
        }
    }
}

void AtualizarInimigos(INIMIGOS inimigos[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (inimigos[i].ativo) {
            inimigos[i].y += inimigos[i].movimento;
            if (inimigos[i].y > 305) {
                inimigos[i].y = 305;
                inimigos[i].ativo = false;
            }
        }
    }
}

void DesenhaInimigos(INIMIGOS inimigos[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (inimigos[i].ativo) {
            al_draw_bitmap(laranja, inimigos[i].x, inimigos[i].y, 0);
        }
    }
}

void DesenhaAliados(INIMIGOS inimigos[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (inimigos[i].ativo) {
            al_draw_bitmap(azul, inimigos[i].x, inimigos[i].y, 0);
        }
    }
    colidiu = 0;
}

// NAVE
void InitNave(NAVE* nave) {
    nave->x = 280;
    nave->y = 400;
    nave->movimento = 8;
}

void MoveNaveEsquerda(NAVE* nave) {
    nave->x = nave->x - nave->movimento;
    if (nave->x < 20) {
        nave->x = 20;
    }
}

void MoveNaveDireita(NAVE* nave) {
    nave->x = nave->x + nave->movimento;
    if (nave->x > 540) {
        nave->x = 540;
    }
}
