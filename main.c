#include <stdio.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define BOARD_SIZE_X 15
#define BOARD_SIZE_Y 10

typedef struct {
	int x;
	int y;
} snake_position, food_position;


void print_board(char board[BOARD_SIZE_Y][BOARD_SIZE_X])
{
	char buffer[(100 + BOARD_SIZE_Y * BOARD_SIZE_X) + (BOARD_SIZE_Y + BOARD_SIZE_X + 2 + (2 * (BOARD_SIZE_X + 3)))] = { 0 };

	char top_layer[BOARD_SIZE_X + 3];
	
	for (int i = 0; i < BOARD_SIZE_X + 3; i++)
	{

		if (i == BOARD_SIZE_X + 3 - 1)
		{
			top_layer[i] = '\0';
		}
		else if (i == BOARD_SIZE_X + 3 -2)
		{
			top_layer[i] = '\n';
		}
		else
		{
			top_layer[i] = '_';
		}
		
	}

	strcat_s(buffer, sizeof buffer, top_layer);

	for (unsigned int i = 0; i < BOARD_SIZE_Y; i++)
	{

		char line[BOARD_SIZE_X + 2] = "\0";

		strcat_s(line, sizeof line, "|");
		strcat_s(line, sizeof line, board[i]);
		

		strcat_s(buffer, sizeof buffer, line);
		strcat_s(buffer, sizeof buffer, "|\n");
	}

	char bottom_layer[BOARD_SIZE_X + 3];

	for (int i = 0; i < BOARD_SIZE_X + 3; i++)
	{

		if (i == BOARD_SIZE_X + 3 - 1)
		{
			bottom_layer[i] = '\0';
		}
		else if (i == BOARD_SIZE_X + 3 - 2)
		{
			bottom_layer[i] = '\n';
		}
		else
		{
			bottom_layer[i] = '-';
		}

	}

	strcat_s(buffer, sizeof buffer, bottom_layer);
	
	printf("%s", buffer);
}

void clear_board(char board[BOARD_SIZE_Y][BOARD_SIZE_X])
{
	for (unsigned int i = 0; i < BOARD_SIZE_Y; i++)
	{

		for (unsigned int y = 0; y < BOARD_SIZE_X; y++)
		{
			if (y == sizeof(board[0]) - 1)
			{
				board[i][y] = '\0';
			}
			else
			{
				board[i][y] = ' ';
			}
		}
	}
}

void update_snake_pos(int x, int y, snake_position snake[], size_t len)
{
	snake_position head = snake[0];

	snake_position prev = head;
	snake[0].x += x;
	snake[0].y += y;


	for (int i = 1; i < len; i++) {

		snake_position current_dot = snake[i];

		snake[i].x = prev.x;
		snake[i].y = prev.y;

		prev = current_dot;
	}
}


void write_snake_pos(char board[BOARD_SIZE_Y][BOARD_SIZE_X], snake_position snake[], size_t len)
{
	for (int i = 0; i < len; i++) {
		snake_position the_snake = snake[i];
		board[snake[i].y][snake[i].x] = '*';
	}
}

snake_position* grow_snake(snake_position* snake, int current_size) {

	snake_position* new_snake = (snake_position*)realloc(snake, sizeof(snake_position) * (current_size + 1));

	new_snake[current_size].x = snake->x;
	new_snake[current_size].y = snake->y - 1;

	return new_snake;
}

void write_food_pos(char board[BOARD_SIZE_Y][BOARD_SIZE_X], food_position food) {
	board[food.y][food.x] = '#';
}

void update_food_position(food_position * food, snake_position snake[], int snake_size)
{
	int can_be_there = 1;

	int x;
	int y;

	do
	{
		can_be_there = 1;
		
		x = rand() % (BOARD_SIZE_X - 1);
		y = rand() % (BOARD_SIZE_Y - 1);

		for (int i = 0; i < snake_size; i++)
		{
			if (snake[i].x == x && snake[i].y == y)
			{
				can_be_there = 0;
			}
		}
		
	} while (!can_be_there);

	food->x = x;
	food->y = y;
	
	/*while(!can_be_there)
	{
	
		//food->x = rand() % (BOARD_SIZE_X - 1);
		//food->y = rand() % (BOARD_SIZE_Y - 1);
	}*/
	
	
}

int out_of_bound(snake_position head, snake_position next_pos)
{
	return head.x + next_pos.x > BOARD_SIZE_X - 2 || head.x + next_pos.x < 0 || head.y + next_pos.y > BOARD_SIZE_Y - 1|| head.y + next_pos.y < 0;
}

int eat_itself(snake_position * snake, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		for (int y = 0; y < size; y++)
		{
			if ((snake_position*) &snake[i] != (snake_position*) &snake[y] && snake[i].x == snake[y].x && snake[i].y == snake[y].y)
			{
				return 1;
			}
		}
	}

	return  0;
}

int game ()
{
	char board[BOARD_SIZE_Y][BOARD_SIZE_X] = { {0} };


	int size = 1;
	int exit = 0;


	snake_position* snake = malloc(1 * sizeof(snake_position));

	snake[0].x = 2;
	snake[0].y = 2;

	snake_position snake_movement = { 0,0 };
	food_position food_item = { 5, 5 };


	while (exit != -1)
	{

		system("cls");


		int key_pressed = 0;

		if (_kbhit())
		{
			key_pressed = _getch();
		}

		switch (key_pressed)
		{
		case 'w':
			snake_movement.x = 0;
			snake_movement.y = -1;
			break;
		case 'a':
			snake_movement.x = -1;
			snake_movement.y = 0;
			break;
		case 's':
			snake_movement.x = 0;
			snake_movement.y = 1;
			break;
		case 'd':
			snake_movement.x = 1;
			snake_movement.y = 0;
			break;
		}

		if (snake[0].x == food_item.x && snake[0].y == food_item.y) {
			snake = grow_snake(snake, size);
			update_food_position(&food_item, snake, size);
			size++;
		}

		if (out_of_bound(snake[0], snake_movement))
		{
			return -1;
		}

		clear_board(board);
		update_snake_pos(snake_movement.x, snake_movement.y, snake, size);
		write_food_pos(board, food_item);
		write_snake_pos(board, snake, size);

		if (eat_itself(snake, size))
		{
			return -1;
		}

		char snake_pos_x_s[100];
		char snake_pos_y_s[100];
		char snake_pos_size_s[100];

		_itoa_s(snake[0].x, snake_pos_x_s, _countof(snake_pos_x_s), 10);
		_itoa_s(snake[0].y, snake_pos_y_s, _countof(snake_pos_y_s), 10);
		_itoa_s(size, snake_pos_size_s, _countof(snake_pos_size_s), 10);


		printf("X : %s Y : %s Size: %s \n", snake_pos_x_s, snake_pos_y_s, snake_pos_size_s);

		print_board(board);

		Sleep(200);

	}

	free(snake);

	return 0;
}

void wait_screen()
{


	
	
}

int main()
{

	wait_screen();
	
	int status = game();

	if (status == -1)
	{
		printf("You lost !");
	}

	return 0;
	
}
