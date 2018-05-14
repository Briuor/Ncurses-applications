#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include "map.h"

int main()
{
	initscr();
	noecho();//sem digitar na tela
	nodelay(stdscr, TRUE);//sem para com getch()
	curs_set(0);//sem cursor na tela
	start_color();//enable cor
	init_pair(1, COLOR_BLUE, COLOR_BLACK);//par de cores azul e preto

	
	/*Player informacoes*/
	player pl;
	//posicao inicial
	pl.x = 60;
	pl.y = 10;
	//posicoes anteriores
	int ant_x;
	int ant_y;

	//ALOCA MAPA	
	int **map, i, bit = 0;//bit verifica colisao
	map = (int **)malloc(sizeof(int *) * MAPL);
		for(i = 0;i < MAPL; i++)
			map[i] = (int *)malloc(sizeof(int) * MAPC);

	//------------------------------------LOOOPP------------------------------
	while(1)
	{

	ant_x = pl.x;
	ant_y = pl.y;
	
	switch(getch())
	{
		case 'a':
		pl.x -= 1;
		break;
	
		case 'd':
		pl.x += 1;
		break;

		case 'w':
		pl.y -= 1;
		break;

		case 's':
		pl.y += 1;
		break;
	}
	
	erase();//apaga tela antiga
	
	draw_map(map, &pl, &bit);//recebe posicao e bit de coferencia de colisao
	//------------------------------------COLISAO-----------------------------------------		
		if(bit == 1)
		{
			if(ant_x < pl.x)//se para direita volta 1 para esquerda
				pl.x -= 1;
			
			else if(ant_x > pl.x)//se para esquerda volta 1 para direita
				pl.x += 1;
			
			else if(ant_y < pl.y)//se para baixo volta 1 para cima
				pl.y -= 1;
			
			else if(ant_y > pl.y)//se para cima volta 1 para baixo
				pl.y += 1;

			bit = 0;//volta bit para ,caso contrario sera sempre 1
		}

		//DESENHA PLAYER
		attron(COLOR_PAIR(1));
		mvprintw(pl.y, pl.x, "@");
		attroff(COLOR_PAIR(1));

		refresh();
	}

	endwin();

return 0;
}

