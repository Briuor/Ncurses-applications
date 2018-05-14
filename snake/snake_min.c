#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

//map size
#define WIDTH 80
#define HEIGHT 25
//snake max size
#define MAX 2000

typedef struct Snake {
	int direction;		//direction (right,left,up,down)
	int x[MAX], y[MAX]; 	//coordinate of each snake part
	int qtd;		//snake current size 
} snake;

typedef struct Food {
	int x, y; //food coordinates
} food;

//fill the borders with 1 and the rest with 0
void create_map(int map[WIDTH][HEIGHT])
{
	int i, j;
	
	for(i = 0; i < WIDTH; i++)
	{
		for(j = 0 ;j < HEIGHT; j++)
		{
			//fill borders with 1
			if((i == 0) || (i == WIDTH-1) || (j == 0) || (j == HEIGHT-1))
				map[i][j] = 1;
			//fill rest with 0
			else	map[i][j] = 0;
		}
	}
}

//draw the map
void draw_map(int map[WIDTH][HEIGHT])
{
	int i, j;

	for(i = 0 ;i < WIDTH ; i++)
	{
		for(j = 0 ;j < HEIGHT; j++)
		{
			if(map[i][j] == 1)
				mvprintw(j, i, "#");

			else 	mvprintw(j, i, " ");
		}
	}
}

//----SNAKE FUNCITONS----
//initialize snake size with -1(because only position with value 1 can be displayed)
void init_snake(snake *s)
{
	int i;
	s->qtd = 1;	//size qtd = 1
	
	s->x[0] = s->y[0] = 5; //initial positions x:5 y:5
}

//get snake direction
void event(snake *s)
{
	switch(getch()) 
	{
		//up
		case 'w': s->direction = 'u';
		break;
		//down
		case 's': s->direction = 'd';
		break;
		//left
		case 'a': s->direction = 'l';
		break;
		//right
		case 'd': s->direction = 'r';
		break;
	}
}

//draw snake
void draw_snake(snake s)
{
	int i;
	for(i = 0;i < s.qtd; i++)
		mvprintw(s.y[i], s.x[i], "$");	
}

//-FOOD INIT----
void init_food(food *f)
{
	f->x = (rand() % WIDTH-2) + 2;
	f->y = (rand() % HEIGHT-2) + 2;
}

//collision snake and food
int collisionSF(snake *s, food *f)
{
	int i;
	if(s->x[0] == f->x && s->y[0] == f->y)
	{
		//increase snake size
		s->qtd ++;
		//create another food
		init_food(f);
		return TRUE;
	}
	return FALSE;
}

//order the snake positions
void moveSnake(snake *s)
{
	int i;

	//odering the positions the previus is equal the next
	for(i = s->qtd ;i > 0; i--)	
	{
		s->x[i] = s->x[i-1];	
		s->y[i] = s->y[i-1];	
	}
	switch(s->direction)
	{
		case 'u': s->y[0]--;
		break;
		case 'd': s->y[0]++;
		break;
		case 'l': s->x[0]--;
		break;
		case 'r': s->x[0]++;
		break;
	}
}

//----FOOD FUNCTIONS----
void draw_food(food f)
{
	mvprintw(f.y, f.x, "0");
}

int main() 
{
	//ncurses initialization
	initscr();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(0); 

	//create map
	int map[WIDTH][HEIGHT];
	create_map(map);

	//create snake
	snake s;
	init_snake(&s);

	//create food
	food f;
	init_food(&f);

	short exit = FALSE;
	while(!exit)
	{
		usleep(100000); // delay
		
		event(&s); // get keyEvent
		
		erase(); // clean screen

		// move snake
		moveSnake(&s);

		// colision
		collisionSF(&s, &f);

		// draw
		draw_map(map);
		draw_food(f);
		draw_snake(s);

		// update
		refresh();
	}
	endwin();

return 0;
}

