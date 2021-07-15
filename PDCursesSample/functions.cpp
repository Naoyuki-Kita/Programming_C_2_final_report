#include "Header.h"
#include <iostream>
#include <curses.h>
#include <Windows.h>
#include <time.h>
#include "string.h"


#define PATH 0			//通路を示す数
#define WALL 1			//壁を示す数
#define MAN 2			//人間を示す数
#define GOAL 3			//ゴールを示す数
#define OUT 9			//領域外を示す数
#define BUFFSIZE 128	//最大文字列長

enum  Direction { UP, DOWN, RIGHT, LEFT };	//方向に関する定義

/*	外部設計の見方
(関数名):	(何をする関数か一言で説明)

inputs: (引数)
	(引数名):(データ型)		(引数の説明)

return:	(返却値、無ければvoid)
	(引数名、名前が無ければ(value)):(データ型)		(返却値の説明)

<remarks> (備考)

<reference> (参考)

!!WARNING!! (注意)
*/


/*
getCurrrentDirectory:	カレントディレクトリを取得する

inputs:
	currentDirectory:char*	取得したカレントディレクトリ(文字列)を格納するポインタ

return:
	void
*/
void getCurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(BUFFSIZE, currentDirectory);
}

/*
init_maze:	初期化されたMaze型構造体を取得する(初期化関数)

inputs:
	height:int	迷路の縦のマスの数
	width:int	迷路の横のマスの数

return:
	maze:Maze	初期化された迷路

!!WARNING!!
	・height, widthは、最大でもMAX_HEIGHT=50、MAX_WIDTH=80を越えないようにすること
	　(推奨: height<=30, width<=60)
*/
Maze init_maze(int height, int width) {

	Maze maze;		//Maze型構造体の宣言

	/*
	各種属性への代入
		　人間の位置については、init_man関数に不具合が生じ
		変数が代入されなかったときに分かりやすいように, 
		存在しえない座標である(-1, -1)を初期値として代入

		　同様に、ゴール位置にも(-2, -2)を初期値として代入
	*/
	maze.height = height;
	maze.width = width;
	maze.player_x = -1;	
	maze.player_y = -1;
	maze.goal_x = -2;
	maze.goal_y = -2;

	/*
	dataを一旦OUT(領域外を示す数)で埋める
	この操作は迷路として使用する範囲外も含めるので、set_num関数は使用できない
	*/
	for (int x = 0; x < MAX_HEIGHT; x++) {
		for (int y = 0; y < MAX_WIDTH; y++) {
			maze.data[x][y] = OUT;
		}
	}

	/*
	迷路として使用する範囲をWALL(壁を示す数)で埋める
	*/
	for (int x = 0; x < height; x++) {
		;
		for (int y = 0; y < width; y++) {
			set_num(&maze, x, y, WALL);
		}
	}
	return maze;
}

/*
set_num:	Maze型構造体が持つ2次元配列属性dataの指定した地点に指定した数を書き込む

inputs:
	maze:Maze*	書き込み元のMaze型構造体(ポインタ渡し)
	x:int	迷路の縦の座標
	y:int	迷路の横の座標
	num:int	書き込む数

return:
	void

!!WARNING!!
	・第3引数numには必ずdefineした数(PATH, WALL, MAN, GOAL, OUT)のいずれかを指定すること
*/
void set_num(Maze* maze, int x, int y, int num) {
	//迷路として使う範囲の周りを範囲外の領域で囲む設計なので、1ずらす
	maze->data[x + 1][y + 1] = num;
}

/*
get_num:	Maze型構造体が持つ2次元配列属性dataの指定した地点の値を取得する(アクセサ)

inputs:
	maze:Maze*	Maze型構造体(取得するだけなので参照渡しでも可能だが、統一性のためポインタ渡しとしている)
	x:int	迷路の縦の座標
	y:int	迷路の横の座標

return:
	(value):int	取得した値
*/
int get_num(Maze* maze, int x, int y) {
	return maze->data[x + 1][y + 1];
}

/*
dig:	穴掘り法に基づき、指定された座標のマスを掘ることを試行する

inputs:
	maze:Maze*	Maze型構造体(ポインタ渡し)
	x:int	迷路の縦の座標
	y:int	迷路の横の座標

return:
	(value):int	掘ることができたら1, できなかったら0

<reference>
	・http://www.ced.is.utsunomiya-u.ac.jp/lecture/2009/prog/p3/kadai4/5.html
*/
int dig(Maze* maze, int x, int y) {
	//掘ることのできるマスの条件についてはREADME.txtを参照

	int wall_ctr = 0;		//掘りたいマスの周りの壁を数える変数

	if (get_num(maze, x, y) != WALL) return 0;	//掘るマスが壁でなかったら掘らずに終了

	//周りの4方のうちいずれかが領域外(=掘るマスが領域の縁)の場合掘らずに終了
	if (get_num(maze, x, y + 1) == OUT) return 0;
	if (get_num(maze, x, y - 1) == OUT) return 0;
	if (get_num(maze, x + 1, y) == OUT) return 0;
	if (get_num(maze, x - 1, y) == OUT) return 0;

	if (isDead_end(maze, x, y) == 1) {
		set_num(maze, x, y, PATH);
		return 1;
	}
	else return 0;
}

/*
dig_maze:	穴掘り法に基づき、開始地点から可能な限り穴を掘る

inputs:
	maze:Maze*	Maze型構造体(ポインタ渡し)
	start_x:int	開始地点の縦の座標
	start_y:int	開始地点の横の座標

return:
	void

<remarks>
	・詳しくはREADME.txtを参照

<reference>
	・http://www.ced.is.utsunomiya-u.ac.jp/lecture/2009/prog/p3/kadai4/5.html

!!WARNING!!
	・PATHのマスを指定すること(WALLのマスを指定すると何もせず終了する)
*/
void dig_maze(Maze* maze, int start_x, int start_y) {
	refresh();
	print_maze(*maze);

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

/*
shuffle:	配列をシャッフルする

inputs:
	array:int[]			配列(今回はint型)
	size:int			配列の要素数
	seed:unsigned int	ランダムシード値

return:
	void

<remarks>
	・C言語において配列はポインタの一種であるため、配列渡しはポインタ渡しと同義である
	　よって、この関数は返却値を持たないが、関数実行後第1引数arrayはシャッフルされた状態になる

<reference>
	・http://shopping2.gmobb.jp/htdmnr/www08/c01/algorithm/shuffle.html
*/
void shuffle(int array[], int size, unsigned int seed) {
	srand(seed);
	for (int i = 0; i < size; i++) {
		int j = rand() % size;
		int tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}
}

/*
init_man:	人間を配置する

inputs:
	maze:Maze*	人間を配置するMaze型構造体

return:
	void

<remarks>
	・配置位置は左上になる

*/

void init_man(Maze* maze) {

	int x = -1;
	int y = -1;

	for (int sum = 0; sum < min(maze->height, maze->width); sum++) {
		for (x = 0; x < sum; x++) {
			y = sum - x;
			if (get_num(maze, x, y) == PATH && isDead_end(maze, x, y) == 1) goto break_state;
		}
	}

break_state:
	maze->player_x = x;
	maze->player_y = y;
	set_num(maze, maze->player_x, maze->player_y, MAN);

	refresh();
	print_maze(*maze);
}

/*
set_goal:	ゴールを配置する

inputs:
	maze:Maze*	人間を配置するMaze型構造体

return:
	void

<remarks>
	・配置位置は最も右下の道上になる

*/
void set_goal(Maze* maze) {
	int x = -2;
	int y = -2;

	for (int sum = maze->height + maze->width; sum > 0; sum--) {
		for (x = maze->height; x > 0; x--) {
			y = sum - x;
			if (y < maze->width && get_num(maze, x, y) == PATH && isDead_end(maze, x, y) == 1) goto break_state;
		}
	}
break_state:
	maze->goal_x = x;
	maze->goal_y = y;
	set_num(maze, maze->goal_x, maze->goal_y, GOAL);

	refresh();
	print_maze(*maze);
}

/*
print_maze:	Maze型構造体の2次元配列属性dataを元にコンソール上に出力する

inputs:
	maze:Maze	データ参照元のMaze型構造体

return:
	void
*/
void print_maze(Maze maze) {
	erase();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);   //色1 は黒地に白文字
	init_pair(2, COLOR_RED, COLOR_BLACK);	// 色2 は黒地に赤文字
	init_pair(3, COLOR_WHITE, COLOR_BLUE);	// 色3 は黒地に青文字
	mvaddstr(0, 0, "十字キーで移動");
	for (int x = 0; x < maze.height; x++) {
		for (int y = 0; y < maze.width; y++) {
			switch (get_num(&maze, x, y)) {
			case PATH:
				attrset(COLOR_PAIR(1));
				mvaddstr(x + 1, 2 * y, "　");
				break;
			case WALL:
				attrset(COLOR_PAIR(1));
				mvaddstr(x + 1, 2 * y, "■");
				break;
			case MAN:
				attrset(COLOR_PAIR(2));
				mvaddstr(x + 1, 2 * y, "人");
				break;
			case GOAL:
				attrset(COLOR_PAIR(3));
				mvaddstr(x + 1, 2 * y, "出");
				break;
			case OUT:
				attrset(COLOR_PAIR(1));
				mvaddstr(x, 2 * y, "？");
				break;
			}
		}
	}
	refresh();
}

/*
move_man:	人間を移動させる

inputs:
	maze:Maze*	データ参照元のMaze型構造体
	key:int		人間の移動方向

return:
	answer:int	移動に成功したら1, 失敗したら0

!!WARNING!!
	・第2引数keyにはKEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFTのいずれかを指定すること
*/
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

/*
isDead_end:	指定したマスが行き止まりかどうかを判断する

inputs:
	maze:Maze*	データ参照元のMaze型構造体
	x:int		マスの縦の座標
	y:int		マスの横の座標

return:
	(value):int	行き止まりなら1, そうでなければ0

!!WARNING!!
	・指定したマスが壁でないことをあえて確認していないので、必要に応じて壁でないかどうか判断させること
*/
int isDead_end(Maze* maze, int x, int y) {
	int wall_ctr = 0;
	
	//周りの4方の壁を数える
	if (get_num(maze, x, y + 1) == WALL) wall_ctr++;
	if (get_num(maze, x, y - 1) == WALL) wall_ctr++;
	if (get_num(maze, x + 1, y) == WALL) wall_ctr++;
	if (get_num(maze, x - 1, y) == WALL) wall_ctr++;

	if (wall_ctr == 3) {
		return 1;
	}
	else {
		return 0;
	}
}