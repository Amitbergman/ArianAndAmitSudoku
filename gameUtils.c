/*
 * gameUtils.c
 *
 *  Created on: Aug 17, 2018
 *      Author: User
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "main.h"
#include "printer.h"
#include "ActionsHistory.h"
#include "gurobi.h"
#include "gameUtils.h"


void loadBoardFromFile(SudokuGame* game, char* fileToOpen, int mode){
/* TODO - function to free all memory of previous game
 *
 */
	FILE * fp;
	int n,m,N,i,j, curCellContent;
	char* curChar;
	SudokuBoard* resBoard = newEmptyBoard();
	(*game).gameMode = mode;
	(*game).markErrors = 0;

	fp = fopen (fileToOpen, "r");
	if (!fp){
		printf("Error: file doesn't exist or cannot be opened\n");
		return;
	}

	/* Now the Buffer Has all the data from the file
	 *
	 */
	n =0;
	m =0 ;
	fscanf(fp, "%d %d\n", &m,  &n);
	N = n*m;
	(*resBoard).m=m;
	(*resBoard).n=n;

	curChar = (char *)calloc(1,sizeof(char));
	curCellContent=0;
	i=0;
	for (;i<N;i++){
		j=0;
		for (;j<N;j++){
			fscanf(fp, "%d", &curCellContent);
			(*resBoard).board[i][j].content=curCellContent;
			(*resBoard).board[i][j].isError=0;
			fscanf(fp, "%c", curChar);
			if (strcmp(curChar, ".")==0){
				(*resBoard).board[i][j].isFixed=1;
				fscanf(fp, "%c", curChar);
				}
			else{
				(*resBoard).board[i][j].isFixed=0;
			}
		}
	}
	fclose(fp);

	(*((*((*game).curBoard)).board))= *resBoard;
	game->curBoard->next=NULL;
	game->curBoard->prev = NULL;
	game->history->head = game->curBoard;
	sudokuBoardPrinter(game->curBoard->board);

}
void setBoard(SudokuGame* game, SudokuBoard* newBoard){

	Node* node;
	printf("m=%d, n=%d\n",newBoard->m,newBoard->n);

	cleanNextNodes(game->curBoard->next); /* free proceeding nodes in history list */
	node=GetNewNode(newBoard); /* create new node for new board */
	node->prev=game->curBoard; /* update prev and next */
	game->curBoard->next=node;
	game->curBoard=node;
	/*sudokuBoardPrinter(game->curBoard->board);*/

}
void setXYZ(SudokuGame* game, int* a){

	SudokuBoard* newBoard;
	Node* node;
	if ((game->gameMode==1)&&(game->curBoard->board->board[a[0]-1][a[1]-1].isFixed==1)){ /* value fixed in solve mode */
		printf("Error: cell is fixed\n");
		return;
	}

	/* TODO validate(game);
	 *
	 */
	newBoard=duplicateBoard(game->curBoard->board);
	newBoard->board[a[0]-1][a[1]-1].content=a[2]; /* set board[x][y]=z */

	cleanNextNodes(game->curBoard->next); /* free proceeding nodes in history list */
	node=GetNewNode(newBoard); /* create new node for new board */
	node->prev=game->curBoard; /* update prev and next */
	game->curBoard->next=node;
	game->curBoard=node;
	sudokuBoardPrinter(game->curBoard->board); /*print */


	/* TODO if ((game.mode==1)&&(validate(game)==0)||boardIsFull(game)){print puzzle solutions successfully/erroneous}
	 * TODO checkIfSolved(game);
	 */

	return;
}
void validate(SudokuBoard* board){
	if(doesBoardHaveErrors(board)){
		printf("Error: board contains erroneous values\n");
		return;
	}
	if (gurobi(board)==NULL){
		printf("Validation failed: board is unsolvable\n");
	}
	else{
		printf("Validation passed: board is solvable\n");
	}
}
void hintXY(SudokuBoard* board, int x, int y){
	SudokuBoard* solvedBoard=NULL;
	if(doesBoardHaveErrors(board)){
		printf("Error: board contains erroneous values\n");
		return;
	}
	if(board->board[x-1][y-1].isFixed==1){
		printf("Error: cell is fixed\n");
		return;
	}
	if(board->board[x-1][y-1].content>0){
		printf("Error: cell already contains a value\n");
		return;
	}
	solvedBoard=gurobi(board);
	if (solvedBoard==NULL){
		printf("Error: board is unsolvable\n");
	}
	else{
		printf("Hint: set cell to %d\n",solvedBoard->board[x-1][y-1].content);
	}
}
int doesBoardHaveErrors(SudokuBoard* board){
	int n,m,N,i,j;
	n = board->n;
	m = board->m;
	N = n*m;
	i=0;
	for (; i<N;i++){
		j=0;
		for (;j<N;j++){
			if (board->board[i][j].isError==1){
				return 1;
			}
		}
	}
	return 0;
}

void saveBoardToFile(SudokuGame* game, char* fileToOpen){
	int m,n,N,i,j,a;
	FILE * fp;
	m = game->curBoard->board->m;
	n = game->curBoard->board->n;
	N = n*m;
	a = doesBoardHaveErrors(game->curBoard->board);
	if (a==1 && game->gameMode==2){
		printf("Error: board contains erroneous values\n");
	}

	if (game->gameMode==2){
		/* validateTheBoard and exit if not valid after printing not valid
		 * Error: board validation failed\n
		 *
		 */
	}

	fp = fopen(fileToOpen, "w");
	if (!fp){
		printf("Error: File cannot be created or modified\n");
		return;
	}

	fprintf(fp, "%d %d\n",m,n );
	i=0;
	for (; i<N;i++){
		j=0;
		for (; j<N;j++){
			fprintf(fp, "%d", (game->curBoard->board->board)[i][j].content);
			if ((game->curBoard->board->board)[i][j].isFixed){
				fprintf(fp, ".");
			}
			if (j==N-1){
				fprintf(fp, "\n");
			}else{
				fprintf(fp, " ");
			}
		}
	}
	fclose(fp);
	sudokuBoardPrinter(game->curBoard->board);
}

SudokuGame* initGameInInitMode(){
	SudokuGame* game = (SudokuGame*)calloc(1,sizeof(SudokuGame));
	game->gameMode=0; /* init */
	game->markErrors=0;
	game->history=(List*)calloc(1,sizeof(List));
	game->curBoard=GetNewNode(newEmptyBoard());

	return game;
}
void changeToEmptyGameInEditMode(SudokuGame* game){

	game->gameMode=2; /* 0-init 1-solve 2-edit */
	InsertAtHead(game->history,newEmptyBoard());
	game->curBoard=game->history->head;
	sudokuBoardPrinter(game->curBoard->board);
}
int getNumOfLegalValuesToPlaceInCell(SudokuBoard* board, int row, int col){
	int i=1;
	int counter = 0;
	int m = board->m;
	int n = board->n;
	int N = n*m;
	for (;i<=N;i++){
		 counter += isLegalValue(board, row, col, i);
		}
	return counter;
	}

int getSingleValueToInsert(SudokuBoard* board, int row, int col){
	int i=1;

	int m = board->m;
	int n = board->n;
	int N = n*m;
	for (;i<=N;i++){
		 if (isLegalValue(board, row, col, i)==1){
			 return i;
		}

	}
	return -1;
}
int isLegalValue(SudokuBoard * board, int row, int col, int valueToCheck){
	int n = board->n;
	int m = board->m;
	int N = n*m;
	for (int i=0;i<N;i++){
		if ((board->board[row][i].content==valueToCheck)&&(!(i==col))){
			return 0;
		}
	}
	for (int i=0;i<N;i++){
		if ((board->board[i][col].content==valueToCheck)&&(!(i==row))){

int isLegalValue(SudokuBoard * board, int col, int row, int valueToCheck){
	int n,m,N,i;
	n = board->n;
	m = board->m;
	N = n*m;
	for (i=0;i<N;i++){
		if ((board->board[col][i].content==valueToCheck)&&(!(i==row))){
			return 0;
		}
	}
	for (i=0;i<N;i++){
		if ((board->board[i][row].content==valueToCheck)&&(!(i==col))){

			return 0;
		}
	}

	int x= checkValidInBox(board, row, col, n, m, valueToCheck);
	return x;


}
int checkValidInBox(SudokuBoard* board, int row, int col, int n, int m, int valueToCheck){
	 int startCol = (col/m)*m;
	 int startRow = (row/n)*n;
	 int i = startRow;
	 int j = startCol;
	 for (;i<startRow+n;i++){
		 j=startCol;
		 for (;j<startCol+m;j++){
			 if (board->board[i][j].content==valueToCheck&&((i!=row)||j!=col)){
				 return 0;
			 }
		 }
	 }
	 return 1;

}
void autofill(SudokuGame* game){
	if (boardHasErrors(game->curBoard->board)==1){
		printf("Error: board contains erroneous values\n");
		return;
	}

	int n = game->curBoard->board->n;
	int m = game->curBoard->board->m;
	int N = n*m;
	SudokuBoard* newBoard;
	Node* node;
	newBoard=duplicateBoard(game->curBoard->board);

	int somethingChanged=0;
	for (int row = 0;row<N;row++){
		for (int col = 0 ; col<N; col++){
			if ((newBoard->board[row][col]).content==0)
				{int x = getNumOfLegalValuesToPlaceInCell(game->curBoard->board, row,col);
				//not sure about that - if there is 0 things to put here, does that counts for an erroneous?

				if (x==1){
						newBoard->board[row][col].content=getSingleValueToInsert(game->curBoard->board, row,col);
						somethingChanged = 1;
					}

				}
			}
		}
	//nothing is changed, just print the board and no other work
	if (somethingChanged==0){
		sudokuBoardPrinter(game->curBoard->board);
		return;
	}
	printDiffsAutoFill(game->curBoard->board, newBoard);
	cleanNextNodes(game->curBoard->next); /* free proceeding nodes in history list */
	node=GetNewNode(newBoard); /* create new node for new board */
	node->prev=game->curBoard; /* update prev and next */
	game->curBoard->next=node;
	game->curBoard=node;

	sudokuBoardPrinter(game->curBoard->board);
}

int boardHasErrors(SudokuBoard* board){
	int n = board->n;
	int m = board->m;
	int N = n*m;
	for (int i = 0;i<N;i++){
		for (int j = 0; j<N;j++){
			if (board->board[i][j].isError ==1){
						return 0;
					}
		}
	}
return 1;}




