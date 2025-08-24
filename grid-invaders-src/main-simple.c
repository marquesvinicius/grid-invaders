// Grid Invaders - Versão Simplificada
// Evita conflitos de compilação com GCC

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>

#define FPS 60
#define SCREEN_W 640
#define SCREEN_H 480
#define NUM_TIROS 6
#define NUM_INIMIGOS 18

// Estruturas
typedef struct {
    float x, y;
    bool ativo;
    float velocidade;
} Tiro;

typedef struct {
    float x, y;
    bool ativo;
    float velocidade;
    bool eh_aliado;
} Inimigo;

typedef struct {
    float x, y;
    float velocidade;
} Nave;

// Variáveis globais
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;
ALLEGRO_FONT* font = NULL;
ALLEGRO_FONT* font_grande = NULL;

// Objetos do jogo
Nave nave;
Tiro tiros[NUM_TIROS];
Inimigo inimigos[NUM_INIMIGOS];

// Estado do jogo
int pontuacao = 0;
int inimigos_restantes = NUM_INIMIGOS;
int game_state = 0; // 0=menu, 1=jogando, 2=gameover
int spawn_timer = 0;
int spawn_delay = 120; // 2 segundos

// Controles
bool teclas[4] = {false}; // ESQUERDA, DIREITA, ESPACO, ENTER

void error_msg(const char* msg) {
    al_show_native_message_box(display, "ERRO", "Erro no jogo", msg, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

void init_game() {
    // Inicializar nave
    nave.x = SCREEN_W / 2;
    nave.y = SCREEN_H - 50;
    nave.velocidade = 5;
    
    // Inicializar tiros
    for (int i = 0; i < NUM_TIROS; i++) {
        tiros[i].ativo = false;
        tiros[i].velocidade = 8;
    }
    
    // Inicializar inimigos
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        inimigos[i].ativo = false;
        inimigos[i].velocidade = 2;
        inimigos[i].eh_aliado = false;
    }
    
    pontuacao = 0;
    inimigos_restantes = NUM_INIMIGOS;
    spawn_timer = 0;
}

void spawn_inimigo() {
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        if (!inimigos[i].ativo) {
            inimigos[i].x = (rand() % 6) * 100 + 50;
            inimigos[i].y = 50;
            inimigos[i].ativo = true;
            inimigos[i].eh_aliado = (rand() % 3 == 0); // 33% chance de ser aliado
            break;
        }
    }
}

void atirar() {
    for (int i = 0; i < NUM_TIROS; i++) {
        if (!tiros[i].ativo) {
            tiros[i].x = nave.x;
            tiros[i].y = nave.y;
            tiros[i].ativo = true;
            break;
        }
    }
}

void update_tiros() {
    for (int i = 0; i < NUM_TIROS; i++) {
        if (tiros[i].ativo) {
            tiros[i].y -= tiros[i].velocidade;
            if (tiros[i].y < 0) {
                tiros[i].ativo = false;
            }
        }
    }
}

void update_inimigos() {
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        if (inimigos[i].ativo) {
            inimigos[i].y += inimigos[i].velocidade;
            if (inimigos[i].y > SCREEN_H) {
                inimigos[i].ativo = false;
                inimigos_restantes--;
            }
        }
    }
}

void check_collisions() {
    for (int i = 0; i < NUM_TIROS; i++) {
        if (tiros[i].ativo) {
            for (int j = 0; j < NUM_INIMIGOS; j++) {
                if (inimigos[j].ativo) {
                    if (tiros[i].x >= inimigos[j].x - 20 && 
                        tiros[i].x <= inimigos[j].x + 20 &&
                        tiros[i].y >= inimigos[j].y - 20 && 
                        tiros[i].y <= inimigos[j].y + 20) {
                        
                        tiros[i].ativo = false;
                        inimigos[j].ativo = false;
                        inimigos_restantes--;
                        
                        if (inimigos[j].eh_aliado) {
                            pontuacao += 10;
                        } else {
                            pontuacao += 20;
                        }
                        break;
                    }
                }
            }
        }
    }
}

void draw_menu() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font_grande, al_map_rgb(255, 255, 0), SCREEN_W/2, 150, ALLEGRO_ALIGN_CENTRE, "GRID INVADERS");
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2, 250, ALLEGRO_ALIGN_CENTRE, "Pressione ENTER para jogar");
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2, 280, ALLEGRO_ALIGN_CENTRE, "Setas: Mover | Espaco: Atirar | ESC: Sair");
}

void draw_game() {
    al_clear_to_color(al_map_rgb(0, 0, 50));
    
    // Desenhar nave
    al_draw_filled_triangle(nave.x, nave.y, nave.x-20, nave.y+30, nave.x+20, nave.y+30, al_map_rgb(0, 255, 0));
    
    // Desenhar tiros
    for (int i = 0; i < NUM_TIROS; i++) {
        if (tiros[i].ativo) {
            al_draw_filled_circle(tiros[i].x, tiros[i].y, 3, al_map_rgb(255, 255, 0));
        }
    }
    
    // Desenhar inimigos
    for (int i = 0; i < NUM_INIMIGOS; i++) {
        if (inimigos[i].ativo) {
            if (inimigos[i].eh_aliado) {
                al_draw_filled_circle(inimigos[i].x, inimigos[i].y, 15, al_map_rgb(0, 0, 255));
            } else {
                al_draw_filled_circle(inimigos[i].x, inimigos[i].y, 15, al_map_rgb(255, 100, 0));
            }
        }
    }
    
    // UI
    al_draw_text(font_grande, al_map_rgb(255, 255, 0), SCREEN_W/2, 10, ALLEGRO_ALIGN_CENTRE, "GRID INVADERS");
    al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, "Pontuacao: %d", pontuacao);
    al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 30, ALLEGRO_ALIGN_LEFT, "Inimigos: %d", inimigos_restantes);
}

void draw_gameover() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font_grande, al_map_rgb(255, 255, 255), SCREEN_W/2, 150, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
    al_draw_textf(font, al_map_rgb(255, 255, 255), SCREEN_W/2, 200, ALLEGRO_ALIGN_CENTRE, "Pontuacao Final: %d", pontuacao);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W/2, 250, ALLEGRO_ALIGN_CENTRE, "Pressione ENTER para jogar novamente");
}

int main() {
    // Inicializar Allegro
    if (!al_init()) {
        printf("Falha ao inicializar Allegro\n");
        return -1;
    }
    
    // Inicializar add-ons
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    
    // Criar display
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        printf("Falha ao criar display\n");
        return -1;
    }
    al_set_window_title(display, "Grid Invaders - Versao Simplificada");
    
    // Carregar fontes
    font = al_load_font("arial.ttf", 16, 0);
    if (!font) {
        font = al_create_builtin_font();
    }
    
    font_grande = al_load_font("SPACEBAR.ttf", 36, 0);
    if (!font_grande) {
        font_grande = al_create_builtin_font();
    }
    
    // Timer e fila de eventos
    timer = al_create_timer(1.0 / FPS);
    event_queue = al_create_event_queue();
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    
    // Inicializar jogo
    srand(time(NULL));
    init_game();
    
    al_start_timer(timer);
    
    bool running = true;
    bool redraw = false;
    
    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
            
            if (game_state == 1) {
                // Movimento da nave
                if (teclas[0] && nave.x > 20) nave.x -= nave.velocidade;
                if (teclas[1] && nave.x < SCREEN_W - 20) nave.x += nave.velocidade;
                
                // Atualizar tiros
                update_tiros();
                
                // Spawn de inimigos
                spawn_timer++;
                if (spawn_timer >= spawn_delay) {
                    spawn_inimigo();
                    spawn_timer = 0;
                }
                
                // Atualizar inimigos
                update_inimigos();
                
                // Verificar colisões
                check_collisions();
                
                // Game over
                if (inimigos_restantes <= 0) {
                    game_state = 2;
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    teclas[0] = true;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[1] = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    if (game_state == 1) {
                        atirar();
                    }
                    break;
                case ALLEGRO_KEY_ENTER:
                    if (game_state == 0) {
                        game_state = 1;
                        init_game();
                    } else if (game_state == 2) {
                        game_state = 0;
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_LEFT:
                    teclas[0] = false;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[1] = false;
                    break;
            }
        }
        
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            
            if (game_state == 0) {
                draw_menu();
            } else if (game_state == 1) {
                draw_game();
            } else if (game_state == 2) {
                draw_gameover();
            }
            
            al_flip_display();
        }
    }
    
    // Limpeza
    al_destroy_font(font);
    al_destroy_font(font_grande);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    
    return 0;
}
