// PDCursesSample.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。

#include <stdio.h>
#include <curses.h>
#include <iostream>
#include "Header.h"
#include <Windows.h>
#include <time.h>

#define BUFFSIZE 128
#define WALL 1
#define PATH 0
#define OUT 9
#define MAN 2


int main() {

	const int width = 30;
	const int height = 30;

	initscr();

	Maze maze = init_maze(width, height);
	dig_maze(&maze, 2, 3);
	//init_man(maze);

    //print_maze(maze);

	print_maze(maze);

	getch();
	endwin();

	return 0;
}
