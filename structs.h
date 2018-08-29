/*
 * structs.h
 *
 *  Created on: 17 ×‘×�×•×’ 2018
 *      Author: arian
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_


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

typedef struct Node  {
	SudokuBoard* board;
	struct Node* next;
	struct Node* prev;
}Node;

typedef struct List{
	Node* head;
} List;

typedef struct SudokuGame
{
	int gameMode; /* 0-init 1-solve 2-edit */
	int markErrors;
	List *history;
	Node *curBoard;
	int onlyUndoAfterSolvedWithErrors;

} SudokuGame;
typedef struct stackNode{
	int col;
	int row;
	int numToCheck;
}stackNode;

typedef struct stack{
	int max_num;
	stackNode* array;
	int numOfElements;

}stack;


#endif /* STRUCTS_H_ */
