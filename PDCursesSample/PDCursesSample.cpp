// PDCursesSample.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。

#include <stdio.h>
#include <curses.h>
#include <iostream>
#include "Header.h"
#include <Windows.h>
#include <time.h>

#define BUFFSIZE 128
#define PATH 0
#define WALL 1
#define MAN 2
#define GOAL 3
#define OUT 9


int main() {

	/*char currentDirectory[BUFFSIZE];
	getCurrentDirectory(currentDirectory);
	fprintf_s(stdout, currentDirectory);
	char section[BUFFSIZE];
	sprintf_s(section, "section1");
	char height_label[BUFFSIZE];
	sprintf_s(height_label, "height");
	char width_label[BUFFSIZE];
	sprintf_s(width_label, "label");
	char settingFile[BUFFSIZE];
	sprintf_s(settingFile, "%s¥¥setting.ini", currentDirectory);
    int height = GetPrivateProfileInt(section, height_label, -1, settingFile);
	int width = GetPrivateProfileInt(section, width_label, -1, settingFile);*/

	const int height = 30;
	const int width = 30;

	int key = 0;

	initscr();
	noecho();
	cbreak();

	/*start state=================================================*/
	mvaddstr(10, 10, "MAZE by K.N.");
	mvaddstr(11, 10, "PRESS ANY KEY");
	refresh();
	getch();
	/*============================================================*/


	/*creating maze===============================================*/
	srand((unsigned int)clock());

	int start_x = 2 * (rand() % ((height / 2) - 1)) + 1;
	int start_y = 2 * (rand() % ((width / 2) - 1)) + 1;

	Maze maze = init_maze(height, width);
	dig_maze(&maze, start_x, start_y);
	init_man(&maze);
	set_goal(&maze);
	/*============================================================*/
	

	/*playing state===============================================*/
	keypad(stdscr, TRUE);
	while (true) {
		key = getch();
		move_man(&maze, key);
		if (maze.player_x == maze.goal_x && maze.player_y == maze.goal_y) break;
	}
	/*============================================================*/


	/*end state===================================================*/
	endwin();
	fprintf_s(stdout, "maze status:\n");
	fprintf_s(stdout, "\tgoal_x\t%d\n", maze.goal_x);
	fprintf_s(stdout, "\tgoal_y\t%d\n", maze.goal_y);

	/*FILE* fp;
	errno_t error = fopen_s(&fp, "result.txt", "w");
	if (error != 0) {
		fprintf_s(stderr, "failed to open");
	}
	else
	{
		fprintf_s(stdout, "success to open");
		for (int x = 0; x < maze.height; x++) {
			for (int y = 0; y < maze.width; y++) {
				switch (get_num(&maze, x, y)) {
				case PATH:
					fputs("　", fp);
					break;
				case WALL:
					fputs("■", fp);
					break;
				case MAN:
					fputs("人", fp);
					break;
				case GOAL:
					fputs("出", fp);
					break;
				case OUT:
					fputs("？", fp);
					break;
				}
			}
			fputs("\n", fp);
		}
		fclose(fp);
	}*/
	/*============================================================*/

	tprint_maze(maze);

	return 0;
}
