#pragma once

#define BUFFSIZE 128
#define MAX_HEIGHT 50
#define MAX_WIDTH 50


struct Maze {
	int height;
	int width;
	int player_x;
	int player_y;
	int goal_x;
	int goal_y;
	int data[MAX_HEIGHT][MAX_WIDTH];
};

void getCurrentDirectory(char* currentDirectory);

Maze init_maze(int height, int width);

void set_num(Maze* maze, int x, int y, int num);

int get_num(Maze* maze, int x, int y);

int dig(Maze* maze, int x, int y);

void dig_maze(Maze* maze, int start_x, int start_y);

void shuffle(int array[], int size, unsigned int seed);

void init_man(Maze* maze);

void set_goal(Maze* maze);

void print_maze(Maze maze);

void tprint_maze(Maze maze);

int move_man(Maze* maze, int key);
