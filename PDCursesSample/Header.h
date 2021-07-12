#pragma once

#define MAX_HEIGHT 50		//���H�̍ő�̏c�̒���
#define MAX_WIDTH 80		//���H�̍ő�̉��̒���

/*
	Maze�^�\���̒�`
��ɗv�f������܂���int�^�񎟌��z��data�Ƃ��ă��������m�ۂ��Ă����A
���������ɖ��H�̏��Ƃ��ăR���\�[���o�͂���͈͂����߂�
*/
struct Maze {
	int height;							//���H�̏c�̒���
	int width;							//���H�̉��̒���
	int player_x;						//�l�Ԃ�����ʒu�̏c�̍��W
	int player_y;						//�l�Ԃ�����ʒu�̉��̍��W
	int goal_x;							//�S�[���ʒu�̏c�̍��W
	int goal_y;							//�S�[���ʒu�̉��̍��W
	int data[MAX_HEIGHT][MAX_WIDTH];	//���H�̏��
};

//�e��֐��̃v���g�^�C�v�錾
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
