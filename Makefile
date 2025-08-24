# Makefile para Grid Invaders
# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c99 -g
INCLUDES = -Igrid-invaders-src -Ipackages/Allegro.5.2.6/build/native/include
LIBS = -Lpackages/Allegro.5.2.6/build/native/v142/win32/lib -lallegro-debug -lallegro_main-debug -lallegro_image-debug -lallegro_font-debug -lallegro_ttf-debug -lallegro_primitives-debug -lallegro_audio-debug -lallegro_acodec-debug -lallegro_physfs-debug -lallegro_dialog-debug -lallegro_memfile-debug -lallegro_color-debug -lallegro_video-debug

# Arquivos fonte
SOURCES = grid-invaders-src/main.c
OBJECTS = $(SOURCES:.c=.o)

# Nome do executável
TARGET = grid-invaders.exe

# Regra principal
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpar arquivos compilados
clean:
	del /Q $(OBJECTS) $(TARGET)

# Executar o jogo
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
