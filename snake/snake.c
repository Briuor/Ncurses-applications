#include <ncurses.h>//parte grafica
#include <time.h>//time 0
#include <stdlib.h>//numero aleatorio
#include <unistd.h>//para usleep
#include <stdio.h>//arquivos score
#include <string.h>
#define MAX 2580//tamanho maximo da cobra
//tamanho do terminal
#define WIDTH 79
#define HEIGHT 20
//booleana
#define true 1
#define false 0

typedef struct Snake{char dir; int x,y;} snake;//direção e coordenadas 
typedef struct Food{int x,y;} food;//cordenadas da comida

int main()
{
	initscr();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(0);
	//gera NUMERO ALEATORIO
	srand(time(0));
	//SNAKE
	snake s;
	int score = 0;//score do jogador	
	int inix = rand() % (WIDTH-1), iniy = rand() % (HEIGHT-1);//posicao inicial
	int antx[MAX], anty[MAX], tam = 0, i, j;//corpo da cobra ou posicoes anteriores(tam é a variavel que contem o tamanho do array), i é controle de loop
	//preenche o array de posicoes anteriores com -1
	for(i = 0 ;i < MAX; i++){	antx[i] = -1;	anty[i] = -1;	};
		
	s.x = inix; s.y = iniy;
	s.dir = '0';//0 é nada, pode assumir 'r' right, 'l' left , 'u' up, 'd' down

	//FOOD
	food f;
	//posicao aleatoria 
	f.x = rand() % (WIDTH-1);
	f.y = rand() % (HEIGHT-1);

	short exit = false;
	while(!exit)
	{
		erase();
		//compara posicao da cabeça com a primeira parte depois da cabeça
		//atribui posicao do que ta a frente de cada uma das posicoes depois da primeira parte depois da cabeça
		for(i = tam-1;i >= 1; i--)
		{		
			if(anty[i-1] > -1)
			{
				anty[i] = anty[i-1];
				antx[i] = antx[i-1];
			}
		}
		anty[0] = s.y;
		antx[0] = s.x;
		srand(time(0));//gera numero novo a cada loop
		usleep(80000);
		switch(getch())
		{	//esquerda
			case 'a': s.dir = 'l';
			break;
			//direita
			case 'd': s.dir = 'r';
			break;
			//cima
			case 'w': s.dir = 'u';
			break;
			//baixo
			case 's': s.dir = 'd';
			break;
		}
		
		//movimenta snake
		switch(s.dir)
		{
			case 'l':	s.x--; break;
			case 'r':	s.x++; break;
			case 'u':	s.y--; break;
			case 'd':	s.y++; break;
			default: s.x = inix; s.y = iniy; break;
		}
		//------------------COLISAO PAREDE E CORPO----------
		//colisao com o corpo da cobra a partir de 1
		for(i = 1; i < tam; i++)
			if(s.x == antx[i] && s.y == anty[i])exit = true;	

		//colisao parede
		for(i = 0;i <= WIDTH; i++)
		{
			for(j = 0 ;j <= HEIGHT; j++)
			{
				if(i == 0 && s.x == i && s.y == j)exit = true;//borda da esquerda
				else if(i == WIDTH && s.x == i && s.y == j)exit = true;//borda da direita
				else if(j == 0 && s.y == j && s.x == i)exit = true;//borda de cima
				else if(j == HEIGHT && s.y == j && s.x == i)exit = true;//borda de baixo	
			}
		}
		
		//--------------------COME COMIDA-----------
		//se comer food grava a posicao anterior e gera novo food
		if(s.x == f.x && s.y == f.y)
		{
			antx[tam] = s.x;
			anty[tam] = s.y;
			f.x = (rand() % (WIDTH-2))+1;
			f.y = (rand() % (HEIGHT-2))+1;
			++tam;
			score += 100;
		}

		//----------DRAW-----------
		//cobra
		//cabeça
		mvprintw(s.y, s.x, "#");
		//corpo
		for(i = 0; i < tam ;i++)
			mvprintw(anty[i], antx[i], "#");

		//paredes
		for(i = 0;i <= WIDTH; i++)
		{
			for(j = 0;j <= HEIGHT; j++)
			{
				if(j == 0)mvprintw(j,i,"+");
				else if(i == 0)mvprintw(j,i,"+");
				else if(j == HEIGHT)mvprintw(j,i,"+");
				else if(i == WIDTH)mvprintw(j,i,"+");
			}
		}
		mvprintw(21, 60, "score %d",score);	
		//comida
		mvprintw(f.y, f.x, "O");
		refresh();
	}
			
	endwin();
	//grava score
	char nome[10];
	printf("Digite seu nome: ");
	scanf("%s",nome);

	FILE *score_file;
	score_file = fopen("scores.txt", "a");
	if(score_file == NULL) printf("Erro ao gravar\n");
	else
		fprintf(score_file,"%s : %d\n",nome, score);
	fclose(score_file);
	
return 0;
}
