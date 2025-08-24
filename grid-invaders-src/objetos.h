//--------ARQUIVO OBJETOS-----------------

typedef struct NAVE
{
	int x;
	int y;
	int movimento;

}NAVE; 
NAVE nave;


typedef struct TIRO {
	int x;
	int y;
	int movimento;
	int borda_x;
	int borda_y;
	int pontocentral_x;
	int pontocentral_y;
	bool ativo;
}TIRO;
extern TIRO tiro[6]; 

typedef struct INIMIGOS {
	int x;
	int y;
	int borda_x;
	int borda_y;
	float movimento;
	int pontocentral_x;
	int pontocentral_y;
	bool ativo;
	bool eh_aliado;  // true = aliado (azul), false = inimigo (laranja)
	int tipo;        // 0 = normal, 1 = rapido, 2 = lento mas resistente
	int vida;        // Vida do inimigo (para tipos resistentes)
	bool na_trincheira; // Se está dentro da trincheira
	int trincheira_id;  // ID da trincheira (0-5)
}INIMIGOS;
extern INIMIGOS inimigos[18];








