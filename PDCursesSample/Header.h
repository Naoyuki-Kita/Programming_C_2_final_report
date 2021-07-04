#pragma once

#define BUFFSIZE 128
#define MAX_WIDTH 50
#define MAX_HEIGHT 50


struct Coordinate {
	int x;
	int y;
};

struct Digger {
	Coordinate* current_coordinate;			//åªç›ÇÃç¿ïW
	Coordinate* previous_coordinate;		//ëOÇÃç¿ïW
};

struct Maze {
	int width;
	int height;
	int data[MAX_WIDTH][MAX_HEIGHT];
};

Maze init_maze(int width, int height);

Coordinate init_coordinate(int x, int y);

void set_num(Maze* maze, int x, int y, int num);

int get_num(Maze* maze, int x, int y);

int dig(Maze* maze, Coordinate coordinate);

void dig_maze(Maze* maze, int start_x, int start_y);

void shuffle(int array[], int size, unsigned int seed);

//void init_man(Maze maze);

void print_maze(Maze maze);

