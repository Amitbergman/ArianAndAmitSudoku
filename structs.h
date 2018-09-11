/*
 * structs.h
 *
 *  Created on: 17 ×‘×�×•×’ 2018
 *      Author: arian
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

/*type of a cell to be used inside the board */
typedef struct SudokuCell
{
	int content;
	int isFixed;
	int isError;
} SudokuCell;

/*type of a sudoku board - contains the size of n and m
 * and the actual array of cells */
typedef struct SudokuBoard
{
	int n;
	int m;
	SudokuCell **board;
} SudokuBoard;

/*type of a node in the history list */
typedef struct Node  {
	SudokuBoard* board;
	struct Node* next;
	struct Node* prev;
}Node;

/*type of a list
 * we use it to save the history for undo and redo */
typedef struct List{
	Node* head;
} List;

/*type of a game
 * has all the attributes of the game like the board and the current mode */
typedef struct SudokuGame
{
	int gameMode; /* 0-init 1-solve 2-edit */
	int markErrors;
	List *history;
	Node *curBoard;

} SudokuGame;

/*type of a stack node
 * we use it to implement iterative num solutions
 * instead of a recursive version */
typedef struct stackNode{
	int col;
	int row;
	int numToCheck;
}stackNode;


/*type of a stack
 * we use it to implement iterative num solutions
 * instead of a recursive version */
typedef struct stack{
	int max_num;
	stackNode* array;
	int numOfElements;

}stack;


#endif /* STRUCTS_H_ */
