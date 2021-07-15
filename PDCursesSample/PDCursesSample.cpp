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

	//各種変数の初期化
	int height = -1;	//迷路の縦のマス
	int width = -1;		//迷路の横のマス
	int key = 0;		//キー入力を受け付ける変数
	int move_ctr = 0;	//移動回数を数える変数
	int play_ms = 0;		//プレイ時間を計測する変数(単位：ミリ秒)
	double play_time = 0.0;	//プレイ時間を計測する変数(単位：秒)
	char buff[BUFFSIZE];	//文字列保存用の配列
	int play_ctr = 1;		//プレイした回数のカウンタ

	//制御端末の開始
	initscr();
	noecho();
	cbreak();

	/*start state=================================================*/
start_state:
	mvaddstr(10, 10, "MAZE by K.N.");
	mvaddstr(11, 10, "SELECT MODE");
	mvaddstr(12, 10, "1. EASY");
	mvaddstr(13, 10, "2. NORMAL");
	mvaddstr(14, 10, "3. HARD");
	refresh();
	while (1) {	//"1", "2", "3"のいずれかが入力されるまで待機
		key = getch();
		if (key == '1' || key == '2' || key == '3') {
			break;
		}
	}
	/*============================================================*/
	//設定ファイルから各種設定を読み込む
	char currentDirectory[BUFFSIZE];
	getCurrentDirectory(currentDirectory);
	char section[BUFFSIZE];
	if(key == '1') sprintf_s(section, "section1");
	if(key == '2') sprintf_s(section, "section2");
	if(key == '3') sprintf_s(section, "section3");
	char height_label[BUFFSIZE];
	sprintf_s(height_label, "height");
	char width_label[BUFFSIZE];
	sprintf_s(width_label, "width");
	char settingFile[BUFFSIZE];
	sprintf_s(settingFile, "%s\\settings.ini", currentDirectory);
	height = GetPrivateProfileInt(section, height_label, -1, settingFile);
	width = GetPrivateProfileInt(section, width_label, -1, settingFile);


	/*creating maze===============================================*/
	//実行時間を用いて乱数を初期化
	srand((unsigned int)clock());

	//ランダムに穴掘り法の開始位置を決定
	int start_x = 2 * (rand() % ((height / 2) - 1)) + 1;
	int start_y = 2 * (rand() % ((width / 2) - 1)) + 1;

	//Mazeの初期化
	Maze maze = init_maze(height, width);

	//穴掘りの開始
	set_num(&maze, start_x, start_y, PATH);
	dig_maze(&maze, start_x, start_y);

	//プレイヤー、ゴールの配置
	init_man(&maze);
	set_goal(&maze);

	
	FILE* fp;
	//初回のみ出力ファイル"result.txt"の内容を消去
	if (play_ctr == 1) {
		errno_t tmp_error = fopen_s(&fp, "result.txt", "w");
		fclose(fp);
	}

	//出力ファイルへの書き込み(複数回プレイする場合のために追加モードで出力)
	errno_t error = fopen_s(&fp, "result.txt", "a");
	if (error != 0) {
		fprintf_s(stderr, "failed to open\n");
	}
	else
	{
		sprintf_s(buff, "%d回目:\n", play_ctr);
		fputs(buff, fp);
		//迷路の形状を出力
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
		keypad(stdscr, TRUE);	//十字キーの入力を受け付け
		play_ms = clock();
		while (true) {
			key = getch();
			if (move_man(&maze, key) != 0) move_ctr++;	//移動したらインクリメント
			if (maze.player_x == maze.goal_x && maze.player_y == maze.goal_y) {
				play_ms = clock() - play_ms;
				break;
			}	//ゴールしたら終了
		}
	/*============================================================*/

	/*end state===================================================*/
		erase();
		refresh();
		mvaddstr(9, 10, "Game Clear");
		play_time = (double)play_ms / 1000;

		//プレイ時間の表示, 書き込み
		sprintf_s(buff, "プレイ時間:\t%.2f秒\n", play_time);
		mvaddstr(10,10,buff);
		fputs(buff, fp);

		//移動回数の表示, 書き込み
		sprintf_s(buff, "移動回数:\t%d回\n\n", move_ctr);
		mvaddstr(11, 10, buff);
		fputs(buff, fp);

		//もう一度プレイするか選択(eかcのみ受け付け)
		mvaddstr(12, 10, "PRESS C TO CONTINUE");
		mvaddstr(13, 10, "      E TO EXIT");
		fclose(fp);		//プレイする場合fopenでエラーを吐かないように一旦閉じる
		refresh();
		while (1) {
			key = getch();
			if (key == 'c') {
				erase();
				refresh();
				play_ctr++;			//プレイ回数に+1
				goto start_state;	//start_stateからやり直す
			}
			else if (key == 'e') {
				break;
			}
		}
		endwin();
	}
	/*============================================================*/
	

	return 0;
}
