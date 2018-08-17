/*
 * main.h
 *
 *  Created on: 12 באוג 2018
 *      Author: arian
 */

#ifndef MAIN_H_
#define MAIN_H_

typedef struct SudokuCell
{
	int content;
	int isFixed;
	int isError;
} SudokuCell;

typedef struct SudokuBoard
{
	int n;
	int m;
	SudokuCell **board;
} SudokuBoard;
typedef struct SudokuGame
{
	SudokuBoard *board;
	int gameMode; //0-init 1-solve 2-edit
	int markErrors;

} SudokuGame;


int startGame();

int main ();
#endif /* MAIN_H_ */
