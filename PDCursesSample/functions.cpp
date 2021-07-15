#include "Header.h"
#include <iostream>
#include <curses.h>
#include <Windows.h>
#include <time.h>
#include "string.h"


#define PATH 0			//�ʘH��������
#define WALL 1			//�ǂ�������
#define MAN 2			//�l�Ԃ�������
#define GOAL 3			//�S�[����������
#define OUT 9			//�̈�O��������
#define BUFFSIZE 128	//�ő啶����

enum  Direction { UP, DOWN, RIGHT, LEFT };	//�����Ɋւ����`

/*	�O���݌v�̌���
(�֐���):	(��������֐����ꌾ�Ő���)

inputs: (����)
	(������):(�f�[�^�^)		(�����̐���)

return:	(�ԋp�l�A�������void)
	(�������A���O���������(value)):(�f�[�^�^)		(�ԋp�l�̐���)

<remarks> (���l)

<reference> (�Q�l)

!!WARNING!! (����)
*/


/*
getCurrrentDirectory:	�J�����g�f�B���N�g�����擾����

inputs:
	currentDirectory:char*	�擾�����J�����g�f�B���N�g��(������)���i�[����|�C���^

return:
	void
*/
void getCurrentDirectory(char* currentDirectory) {
	GetCurrentDirectory(BUFFSIZE, currentDirectory);
}

/*
init_maze:	���������ꂽMaze�^�\���̂��擾����(�������֐�)

inputs:
	height:int	���H�̏c�̃}�X�̐�
	width:int	���H�̉��̃}�X�̐�

return:
	maze:Maze	���������ꂽ���H

!!WARNING!!
	�Eheight, width�́A�ő�ł�MAX_HEIGHT=50�AMAX_WIDTH=80���z���Ȃ��悤�ɂ��邱��
	�@(����: height<=30, width<=60)
*/
Maze init_maze(int height, int width) {

	Maze maze;		//Maze�^�\���̂̐錾

	/*
	�e�푮���ւ̑��
		�@�l�Ԃ̈ʒu�ɂ��ẮAinit_man�֐��ɕs�������
		�ϐ����������Ȃ������Ƃ��ɕ�����₷���悤��, 
		���݂����Ȃ����W�ł���(-1, -1)�������l�Ƃ��đ��

		�@���l�ɁA�S�[���ʒu�ɂ�(-2, -2)�������l�Ƃ��đ��
	*/
	maze.height = height;
	maze.width = width;
	maze.player_x = -1;	
	maze.player_y = -1;
	maze.goal_x = -2;
	maze.goal_y = -2;

	/*
	data����UOUT(�̈�O��������)�Ŗ��߂�
	���̑���͖��H�Ƃ��Ďg�p����͈͊O���܂߂�̂ŁAset_num�֐��͎g�p�ł��Ȃ�
	*/
	for (int x = 0; x < MAX_HEIGHT; x++) {
		for (int y = 0; y < MAX_WIDTH; y++) {
			maze.data[x][y] = OUT;
		}
	}

	/*
	���H�Ƃ��Ďg�p����͈͂�WALL(�ǂ�������)�Ŗ��߂�
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
set_num:	Maze�^�\���̂�����2�����z�񑮐�data�̎w�肵���n�_�Ɏw�肵��������������

inputs:
	maze:Maze*	�������݌���Maze�^�\����(�|�C���^�n��)
	x:int	���H�̏c�̍��W
	y:int	���H�̉��̍��W
	num:int	�������ސ�

return:
	void

!!WARNING!!
	�E��3����num�ɂ͕K��define������(PATH, WALL, MAN, GOAL, OUT)�̂����ꂩ���w�肷�邱��
*/
void set_num(Maze* maze, int x, int y, int num) {
	//���H�Ƃ��Ďg���͈͂̎����͈͊O�̗̈�ň͂ސ݌v�Ȃ̂ŁA1���炷
	maze->data[x + 1][y + 1] = num;
}

/*
get_num:	Maze�^�\���̂�����2�����z�񑮐�data�̎w�肵���n�_�̒l���擾����(�A�N�Z�T)

inputs:
	maze:Maze*	Maze�^�\����(�擾���邾���Ȃ̂ŎQ�Ɠn���ł��\�����A���ꐫ�̂��߃|�C���^�n���Ƃ��Ă���)
	x:int	���H�̏c�̍��W
	y:int	���H�̉��̍��W

return:
	(value):int	�擾�����l
*/
int get_num(Maze* maze, int x, int y) {
	return maze->data[x + 1][y + 1];
}

/*
dig:	���@��@�Ɋ�Â��A�w�肳�ꂽ���W�̃}�X���@�邱�Ƃ����s����

inputs:
	maze:Maze*	Maze�^�\����(�|�C���^�n��)
	x:int	���H�̏c�̍��W
	y:int	���H�̉��̍��W

return:
	(value):int	�@�邱�Ƃ��ł�����1, �ł��Ȃ�������0

<reference>
	�Ehttp://www.ced.is.utsunomiya-u.ac.jp/lecture/2009/prog/p3/kadai4/5.html
*/
int dig(Maze* maze, int x, int y) {
	//�@�邱�Ƃ̂ł���}�X�̏����ɂ��Ă�README.txt���Q��

	int wall_ctr = 0;		//�@�肽���}�X�̎���̕ǂ𐔂���ϐ�

	if (get_num(maze, x, y) != WALL) return 0;	//�@��}�X���ǂłȂ�������@�炸�ɏI��

	//�����4���̂��������ꂩ���̈�O(=�@��}�X���̈�̉�)�̏ꍇ�@�炸�ɏI��
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
dig_maze:	���@��@�Ɋ�Â��A�J�n�n�_����\�Ȍ��茊���@��

inputs:
	maze:Maze*	Maze�^�\����(�|�C���^�n��)
	start_x:int	�J�n�n�_�̏c�̍��W
	start_y:int	�J�n�n�_�̉��̍��W

return:
	void

<remarks>
	�E�ڂ�����README.txt���Q��

<reference>
	�Ehttp://www.ced.is.utsunomiya-u.ac.jp/lecture/2009/prog/p3/kadai4/5.html

!!WARNING!!
	�EPATH�̃}�X���w�肷�邱��(WALL�̃}�X���w�肷��Ɖ��������I������)
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
shuffle:	�z����V���b�t������

inputs:
	array:int[]			�z��(�����int�^)
	size:int			�z��̗v�f��
	seed:unsigned int	�����_���V�[�h�l

return:
	void

<remarks>
	�EC����ɂ����Ĕz��̓|�C���^�̈��ł��邽�߁A�z��n���̓|�C���^�n���Ɠ��`�ł���
	�@����āA���̊֐��͕ԋp�l�������Ȃ����A�֐����s���1����array�̓V���b�t�����ꂽ��ԂɂȂ�

<reference>
	�Ehttp://shopping2.gmobb.jp/htdmnr/www08/c01/algorithm/shuffle.html
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
init_man:	�l�Ԃ�z�u����

inputs:
	maze:Maze*	�l�Ԃ�z�u����Maze�^�\����

return:
	void

<remarks>
	�E�z�u�ʒu�͍���ɂȂ�

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
set_goal:	�S�[����z�u����

inputs:
	maze:Maze*	�l�Ԃ�z�u����Maze�^�\����

return:
	void

<remarks>
	�E�z�u�ʒu�͍ł��E���̓���ɂȂ�

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
print_maze:	Maze�^�\���̂�2�����z�񑮐�data�����ɃR���\�[����ɏo�͂���

inputs:
	maze:Maze	�f�[�^�Q�ƌ���Maze�^�\����

return:
	void
*/
void print_maze(Maze maze) {
	erase();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);   //�F1 �͍��n�ɔ�����
	init_pair(2, COLOR_RED, COLOR_BLACK);	// �F2 �͍��n�ɐԕ���
	init_pair(3, COLOR_WHITE, COLOR_BLUE);	// �F3 �͍��n�ɐ���
	mvaddstr(0, 0, "�\���L�[�ňړ�");
	for (int x = 0; x < maze.height; x++) {
		for (int y = 0; y < maze.width; y++) {
			switch (get_num(&maze, x, y)) {
			case PATH:
				attrset(COLOR_PAIR(1));
				mvaddstr(x + 1, 2 * y, "�@");
				break;
			case WALL:
				attrset(COLOR_PAIR(1));
				mvaddstr(x + 1, 2 * y, "��");
				break;
			case MAN:
				attrset(COLOR_PAIR(2));
				mvaddstr(x + 1, 2 * y, "�l");
				break;
			case GOAL:
				attrset(COLOR_PAIR(3));
				mvaddstr(x + 1, 2 * y, "�o");
				break;
			case OUT:
				attrset(COLOR_PAIR(1));
				mvaddstr(x, 2 * y, "�H");
				break;
			}
		}
	}
	refresh();
}

/*
move_man:	�l�Ԃ��ړ�������

inputs:
	maze:Maze*	�f�[�^�Q�ƌ���Maze�^�\����
	key:int		�l�Ԃ̈ړ�����

return:
	answer:int	�ړ��ɐ���������1, ���s������0

!!WARNING!!
	�E��2����key�ɂ�KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT�̂����ꂩ���w�肷�邱��
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
isDead_end:	�w�肵���}�X���s���~�܂肩�ǂ����𔻒f����

inputs:
	maze:Maze*	�f�[�^�Q�ƌ���Maze�^�\����
	x:int		�}�X�̏c�̍��W
	y:int		�}�X�̉��̍��W

return:
	(value):int	�s���~�܂�Ȃ�1, �����łȂ����0

!!WARNING!!
	�E�w�肵���}�X���ǂłȂ����Ƃ������Ċm�F���Ă��Ȃ��̂ŁA�K�v�ɉ����ĕǂłȂ����ǂ������f�����邱��
*/
int isDead_end(Maze* maze, int x, int y) {
	int wall_ctr = 0;
	
	//�����4���̕ǂ𐔂���
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