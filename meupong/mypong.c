#include <ncurses.h>
#include <unistd.h>

//Variavel elemento com coordenadas x e y
typedef struct element{	int x;	int y;}	el;

int main()
{
	initscr();
	cbreak();
	noecho();//para nao digitar na tela
	curs_set(0);//tira cursor da tela
	nodelay(stdscr,TRUE);//Faz com que pule getch() se nada pressionado
	keypad(stdscr,TRUE);//disponibiliza controle de setas e outros
	start_color();

	init_pair(1,COLOR_YELLOW,COLOR_BLACK);
	//bola
	el ball;
	ball.x = 40;
	ball.y = 10;

	//paddle1
	el paddle1;
	paddle1.x = 78;
	paddle1.y = 9;
	unsigned short int paddle1_points = 0;

	//paddle2
	el paddle2;
	paddle2.x = 2;
	paddle2.y = 9;
	unsigned short int paddle2_points = 0;
	int i=0;

	//inverte direcao
	bool inverte_y=false, inverte_x=false;


	//LOOP DO JOGO
	while(1)
	{
		erase();//apaga tela anterior
		usleep(100000);//delay
		switch(getch())
		{	
			//paddle 2
			case 's':
			paddle2.y +=1;
			break;

			case 'w':
			paddle2.y -=1;
			break;
			//paddle 1
			case KEY_DOWN:
			paddle1.y +=1;
			break;

			case KEY_UP:
			paddle1.y -=1;
			break;
		}

		//---------------COLISAO----------------
		//colisao bola parede
		if(ball.y == 0)	inverte_y = false;
		else if(ball.y == 24)	inverte_y = true;	

		//colisao bola paddle
		if(ball.x == paddle1.x && ball.y == paddle1.y) inverte_x = true;
		if(ball.x == paddle1.x && ball.y == paddle1.y+1) inverte_x = true;
		if(ball.x == paddle1.x && ball.y == paddle1.y+2) inverte_x = true;

		else if(ball.x == paddle2.x && ball.y == paddle2.y) inverte_x = false;
		if(ball.x == paddle2.x && ball.y == paddle2.y+1) inverte_x = false;
		if(ball.x == paddle2.x && ball.y == paddle2.y+2) inverte_x = false;

		if(inverte_y == true)--ball.y;
		else if(inverte_y == false)++ball.y;

		if(inverte_x == true) --ball.x;
		else if(inverte_x == false) ++ball.x;
		//--------------------------------------

		//DESENHA PONTOS, PADLLE E BALL
		mvprintw(0,38,"%d",paddle2_points);
		mvprintw(0,40,"%d",paddle1_points);
		attron(COLOR_PAIR(1));
		for(i = 0; i < 3 ; i++)
		mvprintw(paddle1.y+i, paddle1.x,"|");		
		for(i = 0; i < 3; i++)
		mvprintw(paddle2.y+i, paddle2.x,"|");		
		mvprintw(ball.y, ball.x,"o");	
		attroff(COLOR_PAIR(1));

		//Conta pontos
		if(ball.x < 0)
		{	++paddle2_points;
			ball.y = 10; ball.x = 40;
		}
		else if(ball.x > 80)
		{	++paddle1_points;
			ball.y = 10; ball.x = 40;
		}

		refresh();//atualiza tela
	}

	endwin();
return 0;}
