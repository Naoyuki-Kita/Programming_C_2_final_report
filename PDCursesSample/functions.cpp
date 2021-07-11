#include "Header.h"
#include <iostream>
#include <curses.h>
#include <Windows.h>
#include <time.h>
#include "string.h"


#define PATH 0
#define WALL 1
#define MAN 2
#define GOAL 3
#define OUT 9
#define BUFFSIZE 128

// 0が通路、1が壁、2が人、3がゴール、9が領域外

enum  Direction { UP, DOWN, RIGHT, LEFT };


void getCurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(BUFFSIZE, currentDirectory);
}

Maze init_maze(int height, int width) {

	Maze maze;

	maze.height = height;
	maze.width = width;
	maze.player_x = -1;
	maze.player_y = -1;
	maze.goal_x = -2;
	maze.goal_y = -2;

	for (int x = 0; x < MAX_HEIGHT; x++) {
		for (int y = 0; y < MAX_WIDTH; y++) {
			maze.data[x][y] = OUT;
		}
	}

	for (int x = 0; x < height; x++) {
		;
		for (int y = 0; y < width; y++) {
			set_num(&maze, x, y, WALL);
		}
	}
	return maze;
}


void set_num(Maze* maze, int x, int y, int num) {
	maze->data[x + 1][y + 1] = num;
}


int get_num(Maze* maze, int x, int y) {
	return maze->data[x + 1][y + 1];
}

int dig(Maze* maze, int x, int y) {

	int flag_ctr = 0;

	if (get_num(maze, x, y) != WALL) return 0;

	if (get_num(maze, x, y + 1) == OUT) return 0;
	if (get_num(maze, x, y - 1) == OUT) return 0;
	if (get_num(maze, x + 1, y) == OUT) return 0;
	if (get_num(maze, x - 1, y) == OUT) return 0;

	if (get_num(maze, x, y + 1) == WALL) flag_ctr++;
	if (get_num(maze, x, y - 1) == WALL) flag_ctr++;
	if (get_num(maze, x + 1, y) == WALL) flag_ctr++;
	if (get_num(maze, x - 1, y) == WALL) flag_ctr++;

	if (flag_ctr == 3) {
		set_num(maze, x, y, PATH);
		return 1;
	}
	else {
		return 0;
	}
}

void dig_maze(Maze* maze, int start_x, int start_y) {
	refresh();
	print_maze(*maze);

	set_num(maze, start_x, start_y, PATH);
	Sleep(1);

	int dir_array[] = { UP, DOWN, RIGHT, LEFT };
	shuffle(dir_array, 4, (unsigned int)clock());

	for (int i = 0; i < 4; i++) {
		erase();
		if (dir_array[i] == UP && dig(maze, start_x, (start_y - 1)) == 1) {
			dig_maze(maze, start_x, (start_y - 1));
		}
		if (dir_array[i] == DOWN && dig(maze, start_x, (start_y + 1)) == 1) {
			dig_maze(maze, start_x, (start_y + 1));
		}
		if (dir_array[i] == RIGHT && dig(maze, (start_x + 1), start_y) == 1) {
			dig_maze(maze, (start_x + 1), start_y);
		}
		if (dir_array[i] == LEFT && dig(maze, (start_x - 1), start_y) == 1) {
			dig_maze(maze, (start_x - 1), start_y);
		}
	}
}

void shuffle(int array[], int size, unsigned int seed) {
	srand(seed);
	for (int i = 0; i < size; i++) {
		int j = rand() % size;
		int tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}
}

void init_man(Maze* maze) {

	int x = -1;
	int y = -1;

	for (int sum = 0; sum < min(maze->height, maze->width); sum++) {
		for (x = 0; x < sum; x++) {
			y = sum - x;
			if (get_num(maze, x, y) == PATH) goto break_state;
		}
	}

break_state:
	maze->player_x = x;
	maze->player_y = y;
	set_num(maze, maze->player_x, maze->player_y, MAN);

	refresh();
	print_maze(*maze);
}

void set_goal(Maze* maze) {
	int x = -2;
	int y = -2;

	for (int sum = maze->height + maze->width; sum > 0; sum--) {
		for (x = maze->height; x > 0; x--) {
			y = sum - x;
			if (y < maze->width && get_num(maze, x, y) == PATH) goto break_state;
		}
	}
break_state:
	maze->goal_x = x;
	maze->goal_y = y;
	set_num(maze, maze->goal_x, maze->goal_y, GOAL);

	refresh();
	print_maze(*maze);
}

void print_maze(Maze maze) {
	erase();
	for (int x = 0; x < maze.height; x++) {
		for (int y = 0; y < maze.width; y++) {
			switch (get_num(&maze, x, y)) {
			case PATH:
				mvaddstr(x, 2 * y, "　");
				break;
			case WALL:
				mvaddstr(x, 2 * y, "■");
				break;
			case MAN:
				mvaddstr(x, 2 * y, "人");
				break;
			case GOAL:
				mvaddstr(x, 2 * y, "出");
				break;
			case OUT:
				mvaddstr(x, 2 * y, "？");
				break;
			}
		}
	}
	refresh();
}

void tprint_maze(Maze maze) {
	for (int x = 0; x < maze.height; x++) {
		for (int y = 0; y < maze.width; y++) {
			switch (get_num(&maze, x, y)) {
			case PATH:
				fprintf_s(stdout, "　");
				break;
			case WALL:
				fprintf_s(stdout, "■");
				break;
			case MAN:
				fprintf_s(stdout, "人");
				break;
			case GOAL:
				fprintf_s(stdout, "出");
				break;
			case OUT:
				fprintf_s(stdout, "？");
				break;
			}
		}
		fprintf_s(stdout, "\n");
	}
}

int move_man(Maze* maze, int key) {
	int prev_x = maze->player_x;
	int prev_y = maze->player_y;
	int answer = 1;
	switch (key) {
	case KEY_UP:
		maze->player_x--;
		break;
	case KEY_DOWN:
		maze->player_x++;
		break;
	case KEY_RIGHT:
		maze->player_y++;
		break;
	case KEY_LEFT:
		maze->player_y--;
		break;
	}
	if (get_num(maze, maze->player_x, maze->player_y) == WALL) {
		maze->player_x = prev_x;
		maze->player_y = prev_y;
		answer = 0;
	}
	set_num(maze, prev_x, prev_y, PATH);
	set_num(maze, maze->player_x, maze->player_y, MAN);
	refresh();
	print_maze(*maze);
	return answer;
}