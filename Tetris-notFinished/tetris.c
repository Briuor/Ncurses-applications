#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//tamanho da tela do jogo
#define WIDTH 12
#define HEIGHT 21

//quantidade de pecas e posicao inicial na hora de nascer e posicao Y do fim do mapa
#define QTD 7
#define INIX 5
#define INIY 1
#define FIM 19

//cada peça tem coordenadas x e y para cada carac#ter dela (todas elas tem 4 caracteres)
//Todas giram em sentido horario tipo ###	##
//				       #	 #
typedef struct Peca{
	short x[4], y[4];//4 caracteres com x e y cada
	short giro;//qual giro a peça está(primeiro ou segundo...)
} peca;

//funcao que desenha as peças
void draw_pecas(short atual, peca p[])
{	
	short i;
	for(i = 0;i < 4; i++)
	mvprintw(p[atual].y[i], p[atual].x[i], "#");
}

//funcao que desenha o mapa
void draw_map(int map[][HEIGHT])
{
	int i, j;
	for(i = 0;i < WIDTH; i++)
	{
		for(j = 0;j < HEIGHT; j++)
		{
			if(i == 0 || j == 0 || i == WIDTH-1 || j == HEIGHT-1)
				mvprintw(j,i,"+");//contorno

			else if(map[i][j] == 2)		mvprintw(j,i,"#");//peças feitas
			else 				mvprintw(j,i,".");//espaços vazios
		}printw("\n");
	}
}

//funcao que confere se linhas estao completas
void confere_linhas(int map[][HEIGHT]);

//limpa local onde linhas foram feitas
void limpa_linhas(int map[][HEIGHT], int pos[], int LINHAS);

//abaixa conteudo depois de limpar linhas feitas
void abaixa_pecas(int map[][HEIGHT], int LINHAS);

//mapa do jogo
//gera numero aleatorio da peça(1 a 6)
//desce 1 caracter para baixo a cada loop
//colisao com outra peça
//verifica se peça ja foi colocada ou se ainda esta sendo manipulada
//tecla cima muda posicao da peça
//verifica se parte do mapa esta completa para esvaziar

/*			peças
	##	####	###	##       ###	##	###
       ##		 #	 ##	 #	##	  #
       	0	1	2	3	4	5	6
*/

//a cada vez que for gerar uma peca inicia ela com pos inicial(pode-se gerar ela dentro da funcao tambem)
void inicializa_peca(short atual,peca p[])
{
	//INICIALIZA POSICAO DA PEÇA
	//INIX = 5 INIY = 1
	switch(atual)
	{
		case 0:	p[atual].x[0] = INIX;	p[atual].y[0] = INIY;	//
			p[atual].x[1] = INIX+1;	p[atual].y[1] = INIY;	// 	 23
			p[atual].x[2] = INIX+1;	p[atual].y[2] = INIY-1;	//	01
			p[atual].x[3] = INIX+2;	p[atual].y[3] = INIY-1;
		break;
		case 1:	p[atual].x[0] = INIX;	p[atual].y[0] = INIY-1;	//	01234
			p[atual].x[1] = INIX+1;	p[atual].y[1] = INIY-1;	
			p[atual].x[2] = INIX+2;	p[atual].y[2] = INIY-1;	
			p[atual].x[3] = INIX+3;	p[atual].y[3] = INIY-1;
		break;
		case 2:	p[atual].x[0] = INIX;	p[atual].y[0] = INIY;	//	123	
			p[atual].x[1] = INIX-1;	p[atual].y[1] = INIY-1;	//	 0
			p[atual].x[2] = INIX;	p[atual].y[2] = INIY-1;
			p[atual].x[3] = INIX+1;	p[atual].y[3] = INIY-1;
		break;
		case 3:	p[atual].x[0] = INIX+2;	p[atual].y[0] = INIY;	//	32	
			p[atual].x[1] = INIX+1;	p[atual].y[1] = INIY;	//	 10
			p[atual].x[2] = INIX+1;	p[atual].y[2] = INIY-1;
			p[atual].x[3] = INIX;	p[atual].y[3] = INIY-1;
		break;
		case 4:	p[atual].x[0] = INIX;	p[atual].y[0] = INIY; //	123	
			p[atual].x[1] = INIX;	p[atual].y[1] = INIY-1;	//	0
			p[atual].x[2] = INIX+1;	p[atual].y[2] = INIY-1;
			p[atual].x[3] = INIX+2;	p[atual].y[3] = INIY-1;
		break;
		case 5:	p[atual].x[0] = INIX;	p[atual].y[0] = INIY;	//	12
			p[atual].x[1] = INIX;	p[atual].y[1] = INIY-1;	//	03
			p[atual].x[2] = INIX+1;	p[atual].y[2] = INIY-1;
			p[atual].x[3] = INIX+1;	p[atual].y[3] = INIY;
		break;
		case 6:	p[atual].x[0] = INIX+2;	p[atual].y[0] = INIY;	//	123	
			p[atual].x[1] = INIX;	p[atual].y[1] = INIY-1;	//	  0
			p[atual].x[2] = INIX+1;	p[atual].y[2] = INIY-1;
			p[atual].x[3] = INIX+2;	p[atual].y[3] = INIY-1;
		break;
	}
	//INICIALIZA GIRO
	p[atual].giro = 0;
}

//todas as peças giram em sentido horario
//Passa-se o indice da peça atual e peça para ver qual giro ela está
void gira_peca(peca p[], short atual)
{
	//verifica peca de 0 a 6
	switch(atual)
	{
		case 0:
			//verifica qual giro tal peça esta
			if(p[atual].giro == 0 || p[atual].giro == 2)
			{
				p[atual].y[0] -= 2;
			//	p[atual].x[2] -= 1;
			//	p[atual].x[3] -= 1;
				p[atual].x[3] -= 2;
			}
			//se peça nao esta no giro 0 ou 2 fica no giro original
			else  
			{
				p[atual].y[0] += 2;
				p[atual].x[2] += 1;
				p[atual].x[3] += 1;
			}
		break;
		case 1:
			if(p[atual].giro == 0 || p[atual].giro == 2)
			{
				p[atual].x[1] -= 1;	p[atual].y[1] -=1;
				p[atual].x[2] -= 2;	p[atual].y[2] -=2;
				p[atual].x[3] -= 3;	p[atual].y[3] -=3;
			}
			else 
			{
				p[atual].x[1] += 1;	p[atual].y[1] +=1;
				p[atual].x[2] += 2;	p[atual].y[2] +=2;
				p[atual].x[3] += 3;	p[atual].y[3] +=3;
			}
		break;
		case 2:
			if(p[atual].giro == 0) {
				p[atual].x[3] -= 1;	p[atual].y[3] -= 1;
			}
			else if(p[atual].giro == 1) {
				p[atual].y[1] += 1;
				p[atual].x[3] += 1;	p[atual].y[3] += 2;
			}
			else if(p[atual].giro == 2) {
				p[atual].x[1] += 1;	p[atual].y[1] -= 2;
				p[atual].y[3] -= 1;
			}
			else if(p[atual].giro == 3) {
				p[atual].x[1] -= 1;	p[atual].y[1] += 1;
			}
		break;
		case 3:
			if(p[atual].giro == 0 || p[atual].giro == 2) 
			{
				p[atual].x[0] -= 2;	p[atual].y[1] -= 2; 
			}
			else 
			{
				p[atual].x[0] += 2;	p[atual].y[1] += 2; 
			}

		break;
		case 4:
			if(p[atual].giro == 0) {
				p[atual].y[0] -= 2;
				p[atual].x[1] += 1;	p[atual].y[1] -= 1;
				p[atual].x[3] -= 1;	p[atual].y[3] += 1;
			}
			else if(p[atual].giro == 1) {
				p[atual].y[0] += 2;
				p[atual].x[1] += 1;	p[atual].y[1] += 2;
				p[atual].x[2] += 1;	
			}
			else if(p[atual].giro == 2) {
				p[atual].y[0] -= 2;	
				p[atual].x[1] -= 2;
				p[atual].x[2] -= 2;
			}
			else if(p[atual].giro == 3) {
				p[atual].y[0] += 1;
				p[atual].x[2] += 2;
				p[atual].y[3] -= 1;
			}
		break;
		//peça cinco é um quadrado entao nao gira
		case 6:
			if(p[atual].giro == 0) {
				p[atual].x[0] -= 1;
				p[atual].y[1] += 1;	
				p[atual].x[3] -= 1;	p[atual].y[3] -= 1;
			}
			else if(p[atual].giro == 1) {
				p[atual].x[2] -= 1;
				p[atual].x[3] += 1;	p[atual].y[3] += 2;
			}
			else if(p[atual].giro == 2) {
				p[atual].y[0] -= 2;	
				p[atual].y[1] -= 1;
				p[atual].y[2] -= 2;
				p[atual].x[3] -= 2;
			}
			else if(p[atual].giro == 3) {
				p[atual].y[0] += 1;
				p[atual].x[1] -= 1;
				p[atual].y[2] += 1;
				p[atual].x[3] += 1;
			}
		break;
	}
}

int main()
{
	initscr();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(0);

	short i;
	//MAPA
	int map[WIDTH][HEIGHT] = 
	{1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1};

	//PEÇAS
	peca p[QTD];
	srand(time(0));//gera aleatorio
	short atual = rand() % QTD;//escolhedor de peça atual
	inicializa_peca(atual, p);
	short prox = rand() % QTD;//proxima peça

	short exit = 0;//exit loop
	while(!exit)
	{
		erase();
		srand(time(0));
		usleep(100000);
		//MOVE PEÇA E MUDA POSICAO
		switch(getch())
		{
			case 'w'://gira peça
				gira_peca(p, atual);
				p[atual].giro++;//conta o giro

				if(p[atual].giro >= 4) //reseta giro
					p[atual].giro = 0;
			break;
			case 's': //baixo
				for(i = 0;i < 4; i++)
					p[atual].y[i]++;
			break;
			case 'a'://esquerda
				for(i = 0;i < 4; i++)
					p[atual].x[i]--;
			break;
			case 'd'://direita
				for(i = 0;i < 4; i++)
					p[atual].x[i]++;
			break;
		}
	
		//COLISAO COM CHAO OU OUTRA PEÇA
		//map[][] = 2 igual '#' no mapa
		if(map[ p[atual].x[0] ] [ p[atual].y[0] ] == 2 ||
		map[ p[atual].x[1] ] [ p[atual].y[1] ] == 2 ||
		map[ p[atual].x[2] ] [ p[atual].y[2] ] == 2 ||
		map[ p[atual].x[3] ] [ p[atual].y[3] ] == 2)
		{
			
			for(i = 0 ;i < 4; i++)
				map[ p[atual].x[i] ] [ p[atual].y[i]-1 ] = 2;//marca posicao
			//da origem a proxima peça e prox é aleatória
			atual = prox;
			//inicializa posicao da nova peca
			inicializa_peca(atual, p);
			prox = rand() % QTD;
		}

		//TESTA COLISAO COM O FIM DO MAPA
		if(p[atual].y[0] == FIM ||
		p[atual].y[1] == FIM ||
		p[atual].y[2] == FIM ||
		p[atual].y[3] == FIM)
		{
			for(i = 0 ;i < 4; i++)
				map[ p[atual].x[i] ] [ p[atual].y[i] ] = 2;//marca posicao
			//da origem a proxima peça e prox é aleatória
			atual = prox;
			//inicializa posicao da nova peca
			inicializa_peca(atual, p);
			prox = rand() % QTD;
		}

		//confere se completou linha
		confere_linhas(map);

		//-------DRAW-----
		draw_map(map);
		draw_pecas(atual, p);
		//desce peça em 1
		for(i = 0;i < 4; i++)	p[atual].y[i]++;

		refresh();
	}

	endwin();
return 0;
}

void confere_linhas(int map[][HEIGHT]) 
{
	short completa = 0;	//completa é a flag para verificar se linha esta completa ou n
	short k = 0;		// indice do contador de linhas feitas
	int pos[4];		//vetor que armazena posicao das linhas completadas, no maximo 4 linhas podem ser completadas
	int i, j;

	//zera vetor de posicoes
	for(i = 0;i < 4; i++)
		pos[i] = -1;

	//confere cada linha por linha se esta completa(nao inclui a borda do mapa)(de baixo para cima)
	for(j = 1; j < HEIGHT-1; j++)
	{
		for(i = 1 ;i < WIDTH-1; i++)
		{
			//verifica se linha esta completa
			if(map[i][j] == 2)
				completa = 1;
			else {
				completa = 0;
				break;
			}
		}
		//se saiu do loop e linha esta completa grava posicao j da linha
		if(completa == 1)
		{
			pos[k] = j;	//grava posicao, k é o indice
			++k;
			completa = 0;
		}
	}
	//Se nenhuma linha foi completa sai da funcao
	if(pos[0] == -1) return ;

	//k é o numero de linhas feitas
	limpa_linhas(map, pos, k);
}

//limpa linhas que estao completas, recebe mapa e posicao 'j' das linhas completas
void limpa_linhas(int map[][HEIGHT], int pos[], int LINHAS)//LINHAS é o numero de linhas a serem limpas
{
	int i, j;
	short k = 0;

	//limpando linhas
	for(k = 0; k < LINHAS; k++)
	{
		for(i = 0;i < WIDTH; i++)
				map[i][pos[k]] = 0;
	}

	//abaixa o conteudo acima da parte que foi limpa
	abaixa_pecas(map, pos[LINHAS-1]);//ultima linha a ser limpa
}

//abaixa conteudo de cima depois de limpar linhas feitas
void abaixa_pecas(int map[][HEIGHT], int ULTIMA)// ultima é a ultima linha que foi limpa(de cima para baixo)
{
	int i;
	short sobe = ULTIMA - 1; // pega linha de cima em cima da ultima(começa na linha em cima da ultima)
				 // e substitui linha de cima para de baixo dela

	//descendo conteudo(linha por linha)
	while(sobe > 0)
	{
		//transfere linha de cima para baixo
		for(i = 1; i < WIDTH-1; i++)
		{
			if(map[i][sobe] == 2)
			{
				map[i][sobe+1] = 2;//pega peça de cima e coloca na linha de baixo
				map[i][sobe] = 0;//reseta posicao da peça na linha de cima(que foi movida para baixo)
			}
		}
		--sobe;//sobe para linha de cima
	}
}
