//------------CABECALHOS--------------------------------------


//inclui o cabeçalho da bibilioteca Allegro 5
#include <allegro5/allegro.h>

//inclui o cabeçalho do add-on para uso de imagens
#include <allegro5/allegro_image.h>

//inclui o cabeçalho do add-on para uso de texto
#include <allegro5/allegro_font.h>

//inclui o cabeçalho do add-on para uso de true font
#include <allegro5/allegro_ttf.h>

//inclui o cabeçalho de add-on para uso de mensagens nativas
#include <allegro5/allegro_native_dialog.h>

//inclui o cabeçalho de add-on para uso de primitivas
#include <allegro5/allegro_primitives.h>

//inclui o arquivo objetos.h
#include "objetos.h"

//__________________________________________________________


//----------------VARIAVEIS GLOBAIS-------------------------

const int largura_t = 640;
const int altura_t = 480;
int atirar = 0;

//__________________________________________________________

// --------------PROTOTIPOS---------------------------------

void error_msg(char* text) {
    al_show_native_message_box(NULL, "ERRO",
        "Ocorreu o seguinte erro e o programa sera finalizado:",
        text, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

void control();




// _________________________________________________________


// ----------------------------------- FUNCAO PRINCIPAL ----------------------------------------------------------

int main(void)
{
    //----------VARIAVEIS DO JOGO --------------------------

    //Variável pontos
    int aliados = 0;
    int invasores = 0;

    // Variável representando a janela principal
    ALLEGRO_DISPLAY* janela = NULL;

    // Variável representando a imagem
    ALLEGRO_BITMAP* fundo = NULL;

    //O nosso arquivo de fonte
    ALLEGRO_FONT* fontetitulo = NULL;

    ALLEGRO_FONT* fontpontos = NULL;

    //declara a fila de eventos
    ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;


    //_______________________________________________________

    //-------INICIALIZACAO DE OBJETOS------------------------
    struct obj nave = {290, 470, 345, 470, 320, 410}
   






    //________________________________________________________

    //-------INICIALIZACAO DE ALLEGRO, DISPLAY E ADDONS------------------------

    // Inicializa a Allegro
    al_init();

    // Inicializa o add-on para utilizacao de texto
    al_init_font_addon();

    // Inicializa o add-on para utilização de imagens
    al_init_image_addon();

    //Inicializa o add-on para ttf
    al_init_ttf_addon();

    //Inicializa o add-on para primitvas
    al_init_primitives_addon();

    //Inicializa o add-on do teclado
    al_install_keyboard();

    // Configura a janela
    janela = al_create_display(largura_t, altura_t);

    al_set_window_title(janela, "Grid Invaders");


    // Carregando o arquivo de fonte
    fontetitulo = al_load_font("SPACEBAR.ttf", 36, 0);
    if (!fontetitulo) {
        al_destroy_display(janela);
        error_msg("Falha ao carregar fonte");
        return -1;
    }
    fontpontos = al_load_font("arial-bold.ttf", 14, 0);
    if (!fontpontos) {
        al_destroy_display(janela);
        error_msg("Falha ao carregar fonte");
        return -1;
    }


    // Carrega a imagem
    fundo = al_load_bitmap("Image.bmp");

    al_clear_to_color(al_map_rgb(255, 255, 255));

    //________________________________________________________________________

    //------------------CRIACAO DA FILA E DEMAIS DISPOSITIVOS-------------------

    //cria fila de eventos
    fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        error_msg("Falha ao criar fila de eventos");
        al_destroy_display(janela);
        return -1;
    }

   

    //________________________________________________________________________


    //----------------REGISTRO DE SOURCES--------------------------------------

    //registra eventos da janela em fila_eventos
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    //_________________________________________________________________________


    //-----------------FUNCOES INICIAIS-----------------------------------------
    control();




    //__________________________________________________________________________

    // ------------------ ESPACO ONDE TAVA OS DESENHOS -------------------------











    //__________________________________________________________________________

    //---------------------------LOOP PRINCIPAL--------------------------------------------------

    while(1)
    {
        //declara variavel que recebe evento e timeout
        ALLEGRO_EVENT evento;
        //espero por um evento da fila e guarda em evento
        al_wait_for_event(fila_eventos, &evento);

        //---------------EVENTOS E LOGICA DO JOGO------------------------
        //se teve eventos e foi um evento de fechar janela, encerra loop
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

    }



        //________________________________________________________________

        //--------------DESENHOS-------------------------------------------

        // Desenha a imagem na tela
        al_draw_bitmap(fundo, 0, 0, 0);

        // Texto centralizado
        al_draw_text(fontetitulo, al_map_rgb(255, 255, 0), 640 / 2, 10, ALLEGRO_ALIGN_CENTRE, "GRID INVADERS");

        //Texto pontos
        al_draw_textf(fontpontos, al_map_rgb(0, 255, 0), 534, 16, ALLEGRO_ALIGN_LEFT, "ALIADOS:      %d", aliados);
        al_draw_textf(fontpontos, al_map_rgb(255, 0, 0), 534, 33, ALLEGRO_ALIGN_LEFT, "INVASORES:  %d", invasores);

        //Desenha a nave
        al_draw_filled_triangle(nave.x, nave.y, nave.wx, nave.wy, nave.w, nave.h, al_map_rgb(0, 255, 0));
        



        // Atualiza a tela
        al_flip_display();


        //________________________________________________________________


    //___________________________________________________________________________________________

    // -------------------------FINALIZACOES DO PROGRAMA-----------------------------------------

    // Finaliza a janela, fonte e fila
    al_destroy_font(fontpontos);
    al_destroy_font(fontetitulo);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);

    //___________________________________________________________________________________________
    return 0;
}

//______________________________________________________________________________________________________________________________________


// --------------------------DEFINICAO DE FUNCOES------------------------------------------------

//----------------------NAVE--------------------------------

void control() {
    if (evento.type == ALLEGRO_KEY_LEFT) {
        nave.x -= 2;
    }
    if (evento.type == ALLEGRO_KEY_RIGHT) {
        nave.x += 2;
    }
    if (evento.type == ALLEGRO_KEY_SPACE) {
        atirar = 1;
    }

}
    




//__________________________________________________________


//________________________________________________________________________________________________