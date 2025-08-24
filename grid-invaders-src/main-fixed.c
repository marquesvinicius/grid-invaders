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
#include <math.h>

// Definições das variáveis globais
TIRO tiro[6];
INIMIGOS inimigos[18];

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
int gameState = 0; // 0 = menu, 1 = jogando, 2 = game over, 3 = escolha de continuar
int spawnTimer = 0;
int spawnDelay = 120; // Frames entre spawns (2 segundos a 60 FPS)
int score = 0; // Pontuação total
int level = 1; // Nível atual
int enemiesDestroyed = 0; // Inimigos destruídos
bool trincheiras_invadidas[6] = {false}; // Status das trincheiras
int trincheiras_invadidas_count = 0; // Contador de trincheiras invadidas

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
void DrawContinueChoice();
void ResetGameComplete();

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
    if (!al_init_image_addon()) {
        error_msg("Falha ao inicializar add-on de imagem");
        return -1;
    }
    if (!al_init_font_addon()) {
        error_msg("Falha ao inicializar add-on de fonte");
        return -1;
    }
    if (!al_init_ttf_addon()) {
        error_msg("Falha ao inicializar add-on TTF");
        return -1;
    }
    if (!al_init_primitives_addon()) {
        error_msg("Falha ao inicializar add-on de primitivas");
        return -1;
    }
    if (!al_install_keyboard()) {
        error_msg("Falha ao inicializar teclado");
        return -1;
    }
    if (!al_install_audio()) {
        error_msg("Falha ao inicializar audio");
        return -1;
    }
    if (!al_init_acodec_addon()) {
        error_msg("Falha ao inicializar codecs de audio");
        return -1;
    }
    if (!al_reserve_samples(2)) {
        error_msg("Falha ao reservar samples");
        return -1;
    }

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

    // Carrega e configura a trilha sonora
    trilha_sonora = al_load_sample("trilha_sonora2.OGG");
    if (!trilha_sonora) {
        error_msg("Falha ao carregar trilha_sonora2.OGG");
        return -1;
    }
    inst_trilha_sonora = al_create_sample_instance(trilha_sonora);
    if (!inst_trilha_sonora) {
        error_msg("Falha ao criar instancia da trilha sonora");
        return -1;
    }
    al_attach_sample_instance_to_mixer(inst_trilha_sonora, al_get_default_mixer());
    al_set_sample_instance_playmode(inst_trilha_sonora, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(inst_trilha_sonora, 0.5);

    // Carrega e configura o som do laser
    laser = al_load_sample("laser.wav");
    if (!laser) {
        error_msg("Falha ao carregar laser.wav");
        return -1;
    }
    inst_laser = al_create_sample_instance(laser);
    if (!inst_laser) {
        error_msg("Falha ao criar instancia do laser");
        return -1;
    }
    al_attach_sample_instance_to_mixer(inst_laser, al_get_default_mixer());
    al_set_sample_instance_gain(inst_laser, 0.8);

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
                         ResetGameComplete(); // Reset completo no menu
                     } else if (gameState == 2) {
                         gameState = 0;
                         ResetGameComplete(); // Reset completo no game over
                     } else if (gameState == 3) {
                         // Continuar para próximo nível - incrementa nível antes de resetar
                         level++;
                         gameState = 1;
                         ResetGame(); // Reset parcial - mantém trincheiras
                     }
                     break;
                                  case ALLEGRO_KEY_ESCAPE:
                     if (gameState == 3) {
                         // Encerrar jogo com vitória
                         gameState = 2;
                     } else {
                         fim = true;
                     }
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
                int inimigosAtivos = 0;
                
                // Conta inimigos caídos e ativos
                for (int i = 0; i < 6; i++) {
                    totalCaidos += contCaidos[i];
                }
                for (int i = 0; i < NUM_INIMIGOS; i++) {
                    if (inimigos[i].ativo) {
                        inimigosAtivos++;
                    }
                }
                
                                 // Verificar condições de game over
                 if (score < 0) {
                     // Game over por pontuação negativa
                     if (inst_trilha_sonora) {
                         al_stop_sample_instance(inst_trilha_sonora);
                     }
                     gameState = 2;
                 } else if (trincheiras_invadidas_count >= 6) {
                     // Game over por todas as trincheiras invadidas
                     if (inst_trilha_sonora) {
                         al_stop_sample_instance(inst_trilha_sonora);
                     }
                     gameState = 2;
                 } else if (totalCaidos >= NUM_INIMIGOS && inimigosAtivos == 0) {
                     // Nível completo - verificar se ganhou
                     if (aliados > invasores) {
                         // Vitória - perguntar se quer continuar
                         if (inst_trilha_sonora) {
                             al_stop_sample_instance(inst_trilha_sonora);
                         }
                         gameState = 3;
                     } else {
                         // Derrota
                         if (inst_trilha_sonora) {
                             al_stop_sample_instance(inst_trilha_sonora);
                         }
                         gameState = 2;
                     }
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
                
                DesenhaInimigos(inimigos, NUM_INIMIGOS);

                // Trincheira
                for (int i = 0; i < 6; i++) {
                    if (trincheiras_invadidas[i]) {
                        // Trincheira invadida - vermelha
                        al_draw_rectangle(20 + i * 100, 280, 120 + i * 100, 380, al_map_rgb(255, 0, 0), 3);
                    } else {
                        // Trincheira livre - verde
                        al_draw_rectangle(20 + i * 100, 280, 120 + i * 100, 380, al_map_rgb(0, 255, 0), 3);
                    }
                }

                // UI
                al_draw_text(fontetitulo, al_map_rgb(255, 255, 0), 320, 10, ALLEGRO_ALIGN_CENTRE, "GRID INVADERS");
                al_draw_textf(fontpontos, al_map_rgb(0, 255, 0), 530, 16, ALLEGRO_ALIGN_LEFT, "ALIADOS: %d", aliados);
                al_draw_textf(fontpontos, al_map_rgb(255, 0, 0), 530, 33, ALLEGRO_ALIGN_LEFT, "INVASORES: %d", invasores);
                al_draw_textf(fontpontos, al_map_rgb(255, 255, 255), 10, 16, ALLEGRO_ALIGN_LEFT, "SCORE: %d", score);
                al_draw_textf(fontpontos, al_map_rgb(255, 255, 255), 10, 33, ALLEGRO_ALIGN_LEFT, "NIVEL: %d", level);
                al_draw_textf(fontpontos, al_map_rgb(255, 255, 255), 10, 50, ALLEGRO_ALIGN_LEFT, "DESTRUIDOS: %d", enemiesDestroyed);
                al_draw_textf(fontpontos, al_map_rgb(255, 0, 0), 10, 67, ALLEGRO_ALIGN_LEFT, "TRINCHEIRAS INVADIDAS: %d/6", trincheiras_invadidas_count);
                
                                 // Removido contadores de debug
                         } else if (gameState == 2) {
                 DrawGameOver();
             } else if (gameState == 3) {
                 DrawContinueChoice();
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
    al_draw_text(fontetitulo, al_map_rgb(255, 255, 0), 320, 100, ALLEGRO_ALIGN_CENTRE, "GRID INVADERS");
    al_draw_textf(fontpontos, al_map_rgb(255, 255, 255), 320, 150, ALLEGRO_ALIGN_CENTRE, "NIVEL: %d", level);
    al_draw_text(fontpontos, al_map_rgb(255, 255, 255), 320, 200, ALLEGRO_ALIGN_CENTRE, "Pressione ENTER para jogar");
    al_draw_text(fontpontos, al_map_rgb(255, 255, 255), 320, 230, ALLEGRO_ALIGN_CENTRE, "Setas: Mover | Espaco: Atirar | ESC: Sair");
    
    // Dicas sobre os inimigos
    al_draw_text(fontpontos, al_map_rgb(255, 255, 0), 320, 280, ALLEGRO_ALIGN_CENTRE, "TIPOS DE INIMIGOS:");
    al_draw_text(fontpontos, al_map_rgb(255, 0, 0), 320, 300, ALLEGRO_ALIGN_CENTRE, "Laranja com borda vermelha = RAPIDO");
    al_draw_text(fontpontos, al_map_rgb(255, 215, 0), 320, 320, ALLEGRO_ALIGN_CENTRE, "Laranja com borda dourada = RESISTENTE (3 tiros)");
    al_draw_text(fontpontos, al_map_rgb(0, 255, 255), 320, 340, ALLEGRO_ALIGN_CENTRE, "Azul = ALIADO (nao atire!)");
}

void DrawGameOver() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(fontetitulo, al_map_rgb(255, 255, 255), 320, 120, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
    
    // Estatísticas do jogo
    al_draw_textf(fontpontos, al_map_rgb(0, 255, 0), 180, 160, ALLEGRO_ALIGN_LEFT, "ALIADOS: %d", aliados);
    al_draw_textf(fontpontos, al_map_rgb(255, 0, 0), 350, 160, ALLEGRO_ALIGN_LEFT, "INVASORES: %d", invasores);
    al_draw_textf(fontpontos, al_map_rgb(255, 255, 255), 180, 180, ALLEGRO_ALIGN_LEFT, "SCORE: %d", score);
    al_draw_textf(fontpontos, al_map_rgb(255, 255, 255), 350, 180, ALLEGRO_ALIGN_LEFT, "NIVEL: %d", level);
    al_draw_textf(fontpontos, al_map_rgb(255, 255, 255), 180, 200, ALLEGRO_ALIGN_LEFT, "INIMIGOS DESTRUIDOS: %d", enemiesDestroyed);
    al_draw_textf(fontpontos, al_map_rgb(255, 0, 0), 180, 220, ALLEGRO_ALIGN_LEFT, "TRINCHEIRAS INVADIDAS: %d/6", trincheiras_invadidas_count);
    
    if (aliados > invasores) {
        al_draw_text(fontetitulo, al_map_rgb(0, 255, 0), 320, 250, ALLEGRO_ALIGN_CENTRE, "VOCE VENCEU!");
    } else if (aliados == invasores) {
        al_draw_text(fontetitulo, al_map_rgb(255, 255, 0), 320, 250, ALLEGRO_ALIGN_CENTRE, "EMPATE!");
    } else {
        al_draw_text(fontetitulo, al_map_rgb(255, 0, 0), 320, 250, ALLEGRO_ALIGN_CENTRE, "VOCE PERDEU!");
    }
    
    al_draw_text(fontpontos, al_map_rgb(255, 255, 255), 320, 300, ALLEGRO_ALIGN_CENTRE, "Pressione ENTER para voltar ao menu");
}

void DrawContinueChoice() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_textf(fontetitulo, al_map_rgb(0, 255, 0), 320, 120, ALLEGRO_ALIGN_CENTRE, "NIVEL %d COMPLETO!", level);
    al_draw_text(fontetitulo, al_map_rgb(0, 255, 0), 320, 160, ALLEGRO_ALIGN_CENTRE, "VOCE VENCEU!");
    
    // Estatísticas do nível
    al_draw_textf(fontpontos, al_map_rgb(255, 255, 255), 320, 200, ALLEGRO_ALIGN_CENTRE, "ALIADOS: %d | INVASORES: %d", aliados, invasores);
    al_draw_textf(fontpontos, al_map_rgb(255, 255, 255), 320, 220, ALLEGRO_ALIGN_CENTRE, "SCORE: %d | INIMIGOS DESTRUIDOS: %d", score, enemiesDestroyed);
    
    al_draw_text(fontpontos, al_map_rgb(255, 255, 0), 320, 260, ALLEGRO_ALIGN_CENTRE, "O que voce quer fazer?");
    al_draw_textf(fontpontos, al_map_rgb(0, 255, 0), 320, 290, ALLEGRO_ALIGN_CENTRE, "ENTER: Continuar para o nivel %d", level + 1);
    al_draw_text(fontpontos, al_map_rgb(255, 0, 0), 320, 320, ALLEGRO_ALIGN_CENTRE, "ESC: Encerrar jogo com vitoria");
}

void ResetGame() {
    // Reseta contadores
    aliados = 0;
    invasores = 0;
    colidiu = 0;
    spawnTimer = 0;
    score = 0;
    enemiesDestroyed = 0;
    
    // Aumenta nível apenas se for game over (não quando continuar)
    if (gameState == 2) {
        level++;
    }
    
    // Limpa contadores por coluna (mas mantém trincheiras invadidas)
    for (int i = 0; i < 6; i++) {
        contCaidos[i] = 0;
        // NÃO limpa trincheiras_invadidas - elas permanecem invadidas
    }
    // NÃO reseta trincheiras_invadidas_count - mantém o total
    
    // Reinicia objetos do jogo
    InitNave(&nave);
    InitInimigos(inimigos, NUM_INIMIGOS);
    InitTiro(tiro, NUM_TIRO);
    
    // Reinicia música
    if (inst_trilha_sonora) {
        al_stop_sample_instance(inst_trilha_sonora);
        al_play_sample_instance(inst_trilha_sonora);
    }
    
    // Garante que todos os tiros e inimigos estejam inativos
    for (int i = 0; i < NUM_TIRO; i++) {
        tiro[i].ativo = false;
    }
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        inimigos[i].ativo = false;
        inimigos[i].eh_aliado = false;
        inimigos[i].tipo = 0;
        inimigos[i].vida = 1;
        inimigos[i].na_trincheira = false;
        inimigos[i].trincheira_id = 0;
    }
}

void ResetGameComplete() {
    // Reseta contadores
    aliados = 0;
    invasores = 0;
    colidiu = 0;
    spawnTimer = 0;
    score = 0;
    enemiesDestroyed = 0;
    level = 1; // Reset do nível
    
    // Limpa contadores por coluna E trincheiras invadidas
    for (int i = 0; i < 6; i++) {
        contCaidos[i] = 0;
        trincheiras_invadidas[i] = false;
    }
    trincheiras_invadidas_count = 0;
    
    // Reinicia objetos do jogo
    InitNave(&nave);
    InitInimigos(inimigos, NUM_INIMIGOS);
    InitTiro(tiro, NUM_TIRO);
    
    // Reinicia música
    if (inst_trilha_sonora) {
        al_stop_sample_instance(inst_trilha_sonora);
        al_play_sample_instance(inst_trilha_sonora);
    }
    
    // Garante que todos os tiros e inimigos estejam inativos
    for (int i = 0; i < NUM_TIRO; i++) {
        tiro[i].ativo = false;
    }
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        inimigos[i].ativo = false;
        inimigos[i].eh_aliado = false;
        inimigos[i].tipo = 0;
        inimigos[i].vida = 1;
        inimigos[i].na_trincheira = false;
        inimigos[i].trincheira_id = 0;
    }
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
                    // Calcula o centro do tiro
                    float tiro_centro_x = tiro[i].x + (tiro[i].borda_x / 2.0f);
                    float tiro_centro_y = tiro[i].y + (tiro[i].borda_y / 2.0f);
                    
                    // Calcula o centro do inimigo
                    float inimigo_centro_x = inimigos[j].x + (inimigos[j].borda_x / 2.0f);
                    float inimigo_centro_y = inimigos[j].y + (inimigos[j].borda_y / 2.0f);
                    
                    // Calcula a distância entre os centros
                    float dx = tiro_centro_x - inimigo_centro_x;
                    float dy = tiro_centro_y - inimigo_centro_y;
                    float distancia = sqrt(dx * dx + dy * dy);
                    
                    // Raio de colisão (metade da largura do objeto)
                    float raio_tiro = tiro[i].borda_x / 2.0f;
                    float raio_inimigo = inimigos[j].borda_x / 2.0f;
                    
                                         // Se a distância for menor que a soma dos raios, houve colisão
                     if (distancia < (raio_tiro + raio_inimigo)) {
                         tiro[i].ativo = false;
                         
                         // Verificar se atirou em aliado
                         if (inimigos[j].eh_aliado) {
                             score -= 20; // Penalidade por atirar em aliado
                         } else {
                             // Sistema de vida para inimigos resistentes
                             if (inimigos[j].tipo == 2) {
                                 inimigos[j].vida--;
                                 if (inimigos[j].vida <= 0) {
                                     inimigos[j].ativo = false;
                                     enemiesDestroyed++;
                                     score += 50; // Pontos extras para inimigos resistentes
                                 }
                             } else {
                                 inimigos[j].ativo = false;
                                 enemiesDestroyed++;
                                 score += 10; // Pontos por destruir inimigo
                             }
                         }
                         
                         // Efeito sonoro de colisão
                         if (inst_laser) {
                             al_stop_sample_instance(inst_laser);
                             al_play_sample_instance(inst_laser);
                         }
                         
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
        // Movimento será definido quando o inimigo for liberado
        inimigos[i].movimento = 0; // Será definido em LiberaInimigos
        inimigos[i].x = 0;
        inimigos[i].y = 0;
        inimigos[i].pontocentral_x = inimigos[i].x + 22;
        inimigos[i].pontocentral_y = inimigos[i].y + 22;
        inimigos[i].borda_x = 44;
        inimigos[i].borda_y = 44;
        inimigos[i].ativo = false;
        inimigos[i].eh_aliado = false;
        inimigos[i].tipo = 0;
        inimigos[i].vida = 1;
        inimigos[i].na_trincheira = false;
        inimigos[i].trincheira_id = 0;
    }
}

void LiberaInimigos(INIMIGOS inimigos[], int tamanho) {
    // Conta total de inimigos ativos
    int total_ativos = 0;
    for (int i = 0; i < tamanho; i++) {
        if (inimigos[i].ativo) {
            total_ativos++;
        }
    }
    
    // Limita o número máximo de inimigos ativos simultaneamente
    if (total_ativos >= 6) {
        return;
    }
    
    // Conta total de inimigos já liberados
    int total_liberados = 0;
    for (int i = 0; i < 6; i++) {
        total_liberados += contCaidos[i];
    }
    
    // Se já liberou todos os inimigos, não libera mais
    if (total_liberados >= NUM_INIMIGOS) {
        return;
    }
    
    // Tenta liberar um novo inimigo
    for (int i = 0; i < tamanho; i++) {
        if (!inimigos[i].ativo) {
            // Tenta encontrar uma coluna que não esteja cheia
            int tentativas = 0;
            int coluna;
            do {
                coluna = rand() % 6;
                tentativas++;
                if (tentativas > 10) return; // Evita loop infinito
            } while (contCaidos[coluna] >= MAX_INIMIGOS_POR_COLUNA);
            
            // Libera o inimigo com tipo aleatório
            contCaidos[coluna]++;
            inimigos[i].x = coluna * 100 + 47;
            inimigos[i].y = 60;
            inimigos[i].ativo = true;
            inimigos[i].eh_aliado = (rand() % 3 == 0); // 33% chance de ser aliado
            
                         // Define tipo do inimigo (apenas para inimigos, não aliados)
             if (!inimigos[i].eh_aliado) {
                 int tipo_rand = rand() % 100;
                 if (tipo_rand < 60) {
                     inimigos[i].tipo = 0; // Normal (60%)
                     inimigos[i].vida = 1;
                 } else if (tipo_rand < 85) {
                     inimigos[i].tipo = 1; // Rápido (25%)
                     inimigos[i].vida = 1;
                 } else {
                     inimigos[i].tipo = 2; // Resistente (15%)
                     inimigos[i].vida = 3;
                 }
             } else {
                 inimigos[i].tipo = 0;
                 inimigos[i].vida = 1;
             }
             
             // TODOS os inimigos ganham velocidade com o nível
             if (inimigos[i].tipo == 2) {
                 inimigos[i].movimento = 1.2 + (level * 0.3); // Resistente - mais rápido que antes
             } else if (inimigos[i].tipo == 1) {
                 inimigos[i].movimento = 3.0 + (level * 0.4); // Rápido - ainda mais rápido
             } else {
                 inimigos[i].movimento = 1.8 + (level * 0.25); // Normal - mais rápido que antes
             }
            
            break;
        }
    }
}

void AtualizarInimigos(INIMIGOS inimigos[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (inimigos[i].ativo) {
            inimigos[i].y += inimigos[i].movimento;
            if (inimigos[i].y > 305) {
                // Inimigo atingiu a trincheira
                if (inimigos[i].eh_aliado) {
                    aliados++;
                    inimigos[i].ativo = false;
                } else {
                    // Inimigo invade trincheira
                    invasores++;
                    inimigos[i].ativo = false; // IMPORTANTE: desativa o inimigo
                    inimigos[i].na_trincheira = true;
                    inimigos[i].trincheira_id = (inimigos[i].x - 47) / 100; // Calcula ID da trincheira
                    
                    // Marca trincheira como invadida se não estava
                    if (!trincheiras_invadidas[inimigos[i].trincheira_id]) {
                        trincheiras_invadidas[inimigos[i].trincheira_id] = true;
                        trincheiras_invadidas_count++;
                    }
                }
            }
        }
    }
}

void DesenhaInimigos(INIMIGOS inimigos[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (inimigos[i].ativo || inimigos[i].na_trincheira) {
            // Desenha o inimigo com a cor correta
            if (inimigos[i].eh_aliado) {
                al_draw_bitmap(azul, inimigos[i].x, inimigos[i].y, 0);
            } else {
                al_draw_bitmap(laranja, inimigos[i].x, inimigos[i].y, 0);
                
                // Indicador visual para inimigos especiais (apenas quando ativos)
                if (inimigos[i].ativo) {
                    if (inimigos[i].tipo == 1) {
                        // Inimigo rápido - borda vermelha
                        al_draw_rectangle(inimigos[i].x, inimigos[i].y, 
                                        inimigos[i].x + inimigos[i].borda_x, 
                                        inimigos[i].y + inimigos[i].borda_y, 
                                        al_map_rgb(255, 0, 0), 2);
                    } else if (inimigos[i].tipo == 2) {
                        // Inimigo resistente - borda dourada
                        al_draw_rectangle(inimigos[i].x, inimigos[i].y, 
                                        inimigos[i].x + inimigos[i].borda_x, 
                                        inimigos[i].y + inimigos[i].borda_y, 
                                        al_map_rgb(255, 215, 0), 3);
                        
                        // Mostra vida do inimigo resistente
                        for (int v = 0; v < inimigos[i].vida; v++) {
                            al_draw_filled_circle(inimigos[i].x + 10 + (v * 8), 
                                                inimigos[i].y - 5, 3, al_map_rgb(255, 0, 0));
                        }
                    }
                }
                
                // Indicador de trincheira invadida (apenas quando na trincheira)
                if (inimigos[i].na_trincheira) {
                    al_draw_filled_rectangle(inimigos[i].x, inimigos[i].y, 
                                           inimigos[i].x + inimigos[i].borda_x, 
                                           inimigos[i].y + inimigos[i].borda_y, 
                                           al_map_rgba(255, 0, 0, 100));
                }
            }
        }
    }
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
