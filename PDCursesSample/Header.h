#pragma once

#define MAX_HEIGHT 50		//迷路の最大の縦の長さ
#define MAX_WIDTH 80		//迷路の最大の横の長さ

/*
	Maze型構造体定義
先に要素数が定まったint型二次元配列dataとしてメモリを確保しておき、
初期化時に迷路の情報としてコンソール出力する範囲を決める
*/
struct Maze {
	int height;							//迷路の縦の長さ
	int width;							//迷路の横の長さ
	int player_x;						//人間がいる位置の縦の座標
	int player_y;						//人間がいる位置の横の座標
	int goal_x;							//ゴール位置の縦の座標
	int goal_y;							//ゴール位置の横の座標
	int data[MAX_HEIGHT][MAX_WIDTH];	//迷路の情報
};

//各種関数のプロトタイプ宣言
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

int move_man(Maze* maze, int key);

int isDead_end(Maze* maze, int x, int y);
