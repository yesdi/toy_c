/*
 * 참고 코드 원저작자 : http://shalgorithmstd.tistory.com/255
 * Modified by :Jiwon Kim <email@jiwon.kim>
 * @title : 숫자퍼즐게임
 * @description : 	퍼즐 5x5, 1~24숫자 , 초기 가장 마지막 칸은 공백
 */

#include <iostream>
#include <time.h>

#include <conio.h>
using namespace std;

bool IsTheAnswer(int iNum[]);
bool doReplay();
int iNum[25] = {};
int sidx = 24;
int cnt = 0;

//공백 의미 0 사용
#define EMPTY	(0)

void swapBoard(int idx1, int idx2)
{
	int tmp;
	tmp = iNum[idx1];
	iNum[idx1] = iNum[idx2];
	iNum[idx2] = tmp;
	sidx = 24;
	cnt = 0;
}

void shufflePuzzle()
{
	//Shuffle
	int idx1, idx2;
	srand((unsigned int)time(0));
	for (int i = 0; i < 100; i++) {
		idx1 = rand() % 24;
		idx2 = rand() % 24;
		swapBoard(idx1, idx2);
	}
}

void initBoard()
{
	// 5x5, 1~24 + empty
	for (int i = 0; i < 24; i++) {
		iNum[i] = i + 1;
	}
	iNum[24] = EMPTY;
}


int tBoard[24] = {};
void SetTargetBoard()
{
	for (int i = 0; i < 24; i++) {
		iNum[i] = i + 1;
	}
	iNum[24] = EMPTY;
}

enum BoardType { NUMBER5X5, SIXCOLOR4 };
enum ProgMode { GAME, SOLVE };

BoardType curBoard = NUMBER5X5;

void drawTargetBoard()
{
	if (curBoard == SIXCOLOR4)
	{

	}
}



void drawBoard(int step)
{
	//일차원 배열을 이차원 배열처럼 사용

	printf("┌"); 
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 7; j++) printf("─"); if(i<4) printf("┬");
	}	printf("┐\n");

	for (int i = 0; i < 5; i++) {
		printf("│ ");
		for (int j = 0; j < 5; j++) {
			if (iNum[i * 5 + j] == EMPTY) {
				printf("*\t");
			}
			else cout << iNum[i * 5 + j] << "\t";
			printf("│ ");

		}cout << endl;

		if (i < 4) {
			printf("├");
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 7; j++) printf("─"); if (i < 4) printf("┼");
			}
			printf("┤\n");
		}
	}

	printf("└");
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 7; j++) printf("─"); if (i<4) printf("┴");
	}	printf("┘\n");


	printf("\n┌"); for (int i = 0; i < 39; i++) printf("─"); 	printf("┐\n");
	printf("│      ↑      w        step : %3d      │\n", step);
	printf("│    ←  →  a   d                      │\n");
	printf("│      ↓      s                        │\n");
	printf("│                                       │\n");
	printf("│  Q, q : 종료                          │\n");
	printf("└"); for (int i = 0; i < 39; i++) printf("─"); 	printf("┘\n");
}

enum COND1 { GT, LT, MODULO5, NEQMODULE5 };

typedef struct _action_map {
	char inputkey[3];
	int idx_offset;
	COND1 cond1;
	int cond2;
} ActionMAP;

enum DIR { UP =0 , DOWN =1 , LEFT =2 , RIGHT=3, NOTHING =4 };
ActionMAP actionMap[4] = {
	{{ 'w', 'W', 72}, -5, GT, 4 },
	{{ 's', 'S', 80}, 5, LT, 20 },
	{{ 'a', 'A', 75}, -1, MODULO5, 0},
	{{ 'd', 'D', 77}, 1, NEQMODULE5, 4}
};

void moveCursor(DIR dir)
{
	iNum[sidx] = iNum[sidx + actionMap[dir].idx_offset];
	iNum[sidx + actionMap[dir].idx_offset] = EMPTY;
	sidx += actionMap[dir].idx_offset;
}

bool canMove(DIR dir)
{
	switch (dir)
	{
	case UP: if (sidx > 4) return true;
		break;
	case DOWN: if (sidx < 20) return true;
		break;
	case LEFT: if (sidx % 5) return true;
		break;
	case RIGHT: if ((sidx % 5) != 4) return true;
		break;
	}
	return false;
}

bool isKeyMatched(const char (&keylist)[3], char input)
{
	for (int i = 0; i < 3; i++)
	{
		if (keylist[i] == input)
		{
			return true;
		}
	}
	return false;
}

int main() {
	int gameMode = GAME;
	
	initBoard();
	shufflePuzzle();

	// User Input step
	while (true) {
		// Refresh Screen
		system("cls");
		// Title
		printf("     *#*  5 X 5  Sliding Puzzle  *#*\n");

		drawTargetBoard();
		drawBoard(cnt);

		char cinput = _getch();

		if (cinput == 'q' || cinput == 'Q')
			break;
	
		// Searching direction
		for (int i = 0; i < 4; i++)
		{
			bool input_matched = isKeyMatched(actionMap[i].inputkey, cinput);
			if (input_matched)
			{
				if (canMove((DIR)i))
					moveCursor((DIR)i);
				break;
			}
		}

		cnt++;
		if (IsTheAnswer(iNum)) {
			if (!doReplay())
				return 0;
		}
		cout << endl;
	}

	printf("게임을 종료합니다. \nTry :: %d\n", cnt);
	system("pause");
	return 0;
}

bool doReplay()
{
	cout << "Correct  ! \nTry :: " << cnt << endl;
	bool chk = 1;
	while (chk) {
		cout << "If you want to play NEW GAME, press Y, or to quit, press Q.\n";
		char cinput2 = _getch();
		switch (cinput2) {
		case 'y':
		case 'Y':
			cout << "\n------------Let's play NEW GAME!------------\n";
			cnt = 0;
			chk = 0;
			break;
		case 'q':
		case 'Q':
			cout << "게임을 종료합니다. \n" << endl;
			system("pause");
			return false;
		default:
			cout << "You put the wrong Key.\n\n" << endl;
			break;
		}
	}
	return true;
}

bool IsTheAnswer(int iNum[]) {
	for (int i = 0; i < 24; i++) {
		if (iNum[i] != i + 1) {
			return false;
		}
	}
	return true;
}