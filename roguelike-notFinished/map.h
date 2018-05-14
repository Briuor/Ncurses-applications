
extern short int current_map;//mapa atual

//struct player posicoes x e y
typedef struct Player
{
	int x, y;
} player;
//TAMANHO DO MAPA
#define MAPL 25
#define MAPC 77

//----------------MAP 0(VILA) ------------------
#define SAIDA_MAP0_Y 0
#define SAIDA_MAP0_X 12
void map0(int **map);

//----------------MAP 1 ------------------------
#define ENTR_MAP1_Y 22
#define ENTR_MAP1_X 12
//#define SAIDA_MAP1_Y 
//#define SAIDA_MAP1_X 
void map1(int **map);

/* 	Funcao que recebe mapa alocado, preenche 
	e retorna bit para ser verficada a colisao	*/
void draw_map(int **map, player *pl, int *bit);

//ALOCA E LIBERA MAPA
int **aloca_map();
void free_map(int **map);


