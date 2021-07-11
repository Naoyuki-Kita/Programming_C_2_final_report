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

	int height = -1;
	int width = -1;
	int key = 0;
	int move_ctr = 0;
	char buff[BUFFSIZE];

	char currentDirectory[BUFFSIZE];
	getCurrentDirectory(currentDirectory);
	char section[BUFFSIZE];
	sprintf_s(section, "section1");
	char height_label[BUFFSIZE];
	sprintf_s(height_label, "height");
	char width_label[BUFFSIZE];
	sprintf_s(width_label, "width");
	char settingFile[BUFFSIZE];
	sprintf_s(settingFile, "%s\\settings.ini", currentDirectory);
	height = GetPrivateProfileInt(section, height_label, -1, settingFile);
	width = GetPrivateProfileInt(section, width_label, -1, settingFile);

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

	FILE* fp;
	errno_t error = fopen_s(&fp, "result.txt", "w");
	if (error != 0) {
		fprintf_s(stderr, "failed to open\n");
	}
	else
	{
		fprintf_s(stdout, "success to open\n");
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
	/*============================================================*/

	/*playing state===============================================*/
		keypad(stdscr, TRUE);
		while (true) {
			key = getch();
			if (move_man(&maze, key) != 0) move_ctr++;
			if (maze.player_x == maze.goal_x && maze.player_y == maze.goal_y) break;
		}
	/*============================================================*/

	/*end state===================================================*/
		endwin();
	
		sprintf_s(buff, "移動回数:  %d\n", move_ctr);
		fputs(buff, fp);

		fclose(fp);
	/*============================================================*/
	}
	


	
	
	


	
	

	
	

	tprint_maze(maze);

	return 0;
}
