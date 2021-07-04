#include "Header.h"
#include <iostream>
#include <curses.h>
#include <Windows.h>
#include <time.h>


#define PATH 0
#define WALL 1
#define MAN 2
#define OUT 9

// 0が通路、1が壁、2が人、9が領域外

enum  Direction { UP, DOWN, RIGHT, LEFT };


Maze init_maze(int width, int height) {

	Maze maze;

	maze.width = width;
	maze.height = height;

	for (int x = 0; x < MAX_WIDTH; x++) {
		for (int y = 0; y < MAX_HEIGHT; y++) {
			maze.data[x][y] = OUT;
		}
	}
	
	for (int x = 0; x < width; x++) {;
		for (int y = 0; y < height; y++) {
			set_num(&maze, x, y, WALL);
		}
	}
	return maze;
}

Coordinate init_coordinate(int x, int y) {
	Coordinate coordinate;
	coordinate.x = x;
	coordinate.y = y;
	return coordinate;
}


void set_num(Maze* maze, int x, int y, int num) {
	maze->data[x+1][y+1] = num;
}


int get_num(Maze* maze, int x, int y) {
	return maze->data[x+1][y+1];
}

int dig(Maze* maze, Coordinate coordinate) {

	int x = coordinate.x;
	int y = coordinate.y;
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
	//Sleep()で実行時間を変化させランダム性を持たせている状態
	//表示まで10秒弱かかる
	//dig_maze中にprintして変化の様子を可視化することで実行時間を稼ぎたい
	
	//erase();
	//print_maze(*maze);
	//init_rand();

	set_num(maze, start_x, start_y, PATH);
	Sleep(1);

	int dir_array[] = { UP, DOWN, RIGHT, LEFT };
	shuffle(dir_array, 4, (unsigned int)clock());

	for (int i = 0; i < 4; i++) {
		if (dir_array[i] == UP && dig(maze, init_coordinate(start_x, (start_y - 1))) == 1) dig_maze(maze, start_x, (start_y - 1));
		if (dir_array[i] == DOWN && dig(maze, init_coordinate(start_x, (start_y + 1))) == 1) dig_maze(maze, start_x, (start_y + 1));
		if (dir_array[i] == RIGHT && dig(maze, init_coordinate((start_x + 1), start_y)) == 1) dig_maze(maze, (start_x + 1), start_y);
		if (dir_array[i] == LEFT && dig(maze, init_coordinate((start_x - 1), start_y)) == 1) dig_maze(maze, (start_x - 1), start_y);
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

//void init_man(Maze maze) {
//	init_rand();
//
//	int start_x = 0;
//	int start_y = 0;
//
//	while (get_num(maze, start_x, start_y) != PATH) {
//		start_x = 2 * (rand() % (maze.width / 2)) + 1;
//		start_y = 2 * (rand() % (maze.height / 2)) + 1;
//	}
//
//	set_num(maze, start_x, start_y, MAN);
//}

void print_maze(Maze maze) {
	erase();
	for (int x = 0; x < maze.width; x++) {
		for (int y = 0; y < maze.height; y++) {
			switch (get_num(&maze, x, y)) {
			case PATH:
				mvaddstr(x, 2*y, "　");
				break;
			case WALL:
				mvaddstr(x, 2*y, "■");
				break;
			case MAN:
				mvaddstr(x, 2*y, "○");
				break;
			case OUT:
				mvaddstr(x, 2*y, "？");
				break;
			}
		}
	}
	refresh();
}