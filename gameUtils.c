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

	FILE * fp;
	int n,m,N,i,j, curCellContent;
	char* curChar;
	SudokuBoard* resBoard = newEmptyBoard();


	fp = fopen (fileToOpen, "r");
	if (!fp){
		printf("Error: file doesn't exist or cannot be opened\n");
		return;
	}

	n =0;
	m =0 ;
	fscanf(fp, "%d %d\n", &m,  &n);
	N = n*m;
	(*resBoard).m=m;
	(*resBoard).n=n;

	curChar = (char *)calloc(1,sizeof(char));
	curCellContent=0;
	for (j=0;j<N;j++){
		for (i=0;i<N;i++){
			fscanf(fp, "%d", &curCellContent);
			(*resBoard).board[i][j].content=curCellContent;
			(*resBoard).board[i][j].isError=0;
			fscanf(fp, "%c", curChar);
			if (strcmp(curChar, ".")==0&&game->gameMode!=2){
				(*resBoard).board[i][j].isFixed=1;
				fscanf(fp, "%c", curChar);
			}
			else{
				(*resBoard).board[i][j].isFixed=0;
			}
		}
	}
	fclose(fp);

	(*game).gameMode = mode;
	cleanNextNodes(game->history->head); /*free history */
	game->curBoard=GetNewNode(resBoard);

	/* (*((*((*game).curBoard)).board))= *resBoard;
	game->curBoard->next=NULL;
	game->curBoard->prev = NULL; */
	game->history->head = game->curBoard;
	updateErrorsInBoard(game->curBoard->board);
	/*sudokuBoardPrinter(game);*/
	free(curChar);
}

void setXYZ(SudokuGame* game, int* a){

	SudokuBoard* newBoard;

	if ((game->gameMode==1)&&(game->curBoard->board->board[a[0]-1][a[1]-1].isFixed==1)){ /* value fixed in solve mode */
		printf("Error: cell is fixed\n");
		return;
	}

	/* TODO validate(game);
	 *
	 */
	newBoard=duplicateBoard(game->curBoard->board);
	newBoard->board[a[0]-1][a[1]-1].content=a[2]; /* set board[x][y]=z */

	updateErrorsInBoard(newBoard);

	InsertBoardNextNode(game,newBoard);

	sudokuBoardPrinter(game); /*print */

	if (game->gameMode==1){
		if (boardIsFull(game->curBoard->board)==1){
			dealWithFullBoard(game);
		}
	}


	/* TODO if ((game.mode==1)&&(validate(game)==0)||boardIsFull(game)){print puzzle solutions successfully/erroneous}
	 * TODO checkIfSolved(game);
	 */

	return;
}
void validate(SudokuBoard* board){
	if(boardHasErrors(board)){
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
void hintXY(SudokuBoard* board, int col, int row){
	SudokuBoard* solvedBoard=NULL;

	if(boardHasErrors(board)){
		printf("Error: board contains erroneous values\n");
		return;
	}
	if(board->board[col-1][row-1].isFixed==1){
		printf("Error: cell is fixed\n");
		return;
	}
	if(board->board[col-1][row-1].content>0){
		printf("Error: cell already contains a value\n");
		return;
	}
	solvedBoard=gurobi(board);
	if (solvedBoard==NULL){
		printf("Error: board is unsolvable\n");
	}
	else{
		printf("Hint: set cell to %d\n",solvedBoard->board[col-1][row-1].content);
	}
}


void saveBoardToFile(SudokuGame* game, char* fileToOpen){
	int m,n,N,i,j,a;
	FILE * fp;
	m = game->curBoard->board->m;
	n = game->curBoard->board->n;
	N = n*m;
	a = boardHasErrors(game->curBoard->board);
	if (a==1 && game->gameMode==2){
		printf("Error: board contains erroneous values\n");
		return;
	}

	if ((game->gameMode==2)&&(gurobi(game->curBoard->board)==NULL)){
		printf("Error: board validation failed\n");
		return;
	}


	fp = fopen(fileToOpen, "w");
	if (!fp){
		printf("Error: File cannot be created or modified\n");
		return;
	}

	fprintf(fp, "%d %d\n",m,n );
	for (i=0;i<N;i++){
		for (j=0;j<N;j++){
			fprintf(fp, "%d", (game->curBoard->board->board)[i][j].content);
			if ((game->curBoard->board->board)[i][j].isFixed || (game->gameMode==2 && game->curBoard->board->board[i][j].content!=0)){
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
	printf("Saved to: %s\n",fileToOpen);
	sudokuBoardPrinter(game);
}

SudokuGame* initGameInInitMode(){
	SudokuGame* game = (SudokuGame*)calloc(1,sizeof(SudokuGame));
	game->gameMode=0; /* init */
	game->markErrors=0;
	game->history=(List*)calloc(1,sizeof(List));
	game->curBoard=GetNewNode(newEmptyBoard());
	game->onlyUndoAfterSolvedWithErrors=0;

	return game;
}
void freeGame(SudokuGame* game){
	cleanNextNodes(game->history->head); /* clear and free history */
	free(game->history);
	free(game);
}
void changeToEmptyGameInEditMode(SudokuGame* game){

	game->gameMode=2; /* 0-init 1-solve 2-edit */
	cleanNextNodes(game->history->head);
	/* InsertAtHead(game->history,newEmptyBoard()); */
	game->history->head=GetNewNode(newEmptyBoard());
	game->curBoard=game->history->head;
	sudokuBoardPrinter(game);
}
int getNumOfLegalValuesToPlaceInCell(SudokuBoard* board, int col, int row){
	int i=1;
	int counter = 0;
	int m = board->m;
	int n = board->n;
	int N = n*m;
	for (;i<=N;i++){
		counter += isLegalValue(board, col, row, i);
	}
	return counter;
}

int getSingleValueToInsert(SudokuBoard* board, int col, int row){
	int i=1;

	int m = board->m;
	int n = board->n;
	int N = n*m;
	for (;i<=N;i++){
		if (isLegalValue(board, col, row, i)==1){
			return i;
		}

	}
	return -1;
}
int isLegalValue(SudokuBoard * board, int col, int row, int valueToCheck){
	int n,m,N,i,x;
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

	x= checkValidInBox(board, col, row, n, m, valueToCheck);
	return x;

}
int checkValidInBox(SudokuBoard* board, int col, int row, int n, int m, int valueToCheck){
	int startCol = (col/m)*m;
	int startRow = (row/n)*n;
	int i = startCol;
	int j = startRow;
	for (;i<startCol+n;i++){
		j=startRow;
		for (;j<startRow+m;j++){
			if (board->board[i][j].content==valueToCheck&&((i!=col)||j!=row)){
				return 0;
			}
		}
	}
	return 1;

}
void autofill(SudokuGame* game){
	int n,m,N, somethingChanged, col, row,x;
	SudokuBoard* newBoard;
	Node* node;

	if (boardHasErrors(game->curBoard->board)==1){
		printf("Error: board contains erroneous values\n");
		return;
	}

	n = game->curBoard->board->n;
	m = game->curBoard->board->m;
	N = n*m;

	newBoard=duplicateBoard(game->curBoard->board);

	somethingChanged=0;
	for (row = 0 ; row<N; row++){
		for (col = 0;col<N;col++){
			if ((newBoard->board[col][row]).content==0)
			{
				x = getNumOfLegalValuesToPlaceInCell(game->curBoard->board, col,row);
				/*not sure about that - if there is 0 things to put here, does that counts for an erroneous? */

				if (x==1){
					newBoard->board[col][row].content=getSingleValueToInsert(game->curBoard->board, col,row);
					somethingChanged = 1;
				}

			}
		}
	}
	/* nothing is changed, just print the board and no other work */
	if (somethingChanged==0){
		sudokuBoardPrinter(game);
		return;
	}
	printDiffsAutoFill(game->curBoard->board, newBoard);
	cleanNextNodes(game->curBoard->next); /* free proceeding nodes in history list */
	node=GetNewNode(newBoard); /* create new node for new board */
	node->prev=game->curBoard; /* update prev and next */
	game->curBoard->next=node;
	game->curBoard=node;

	updateErrorsInBoard(game->curBoard->board);
	sudokuBoardPrinter(game);
	if (boardIsFull(game->curBoard->board)){
		dealWithFullBoard(game);
	}
}

int boardHasErrors(SudokuBoard* board){
	int i,j,n,m,N;
	n = board->n;
	m = board->m;
	N = n*m;
	for (i = 0;i<N;i++){
		for (j = 0; j<N;j++){
			if (board->board[i][j].isError ==1){
				return 1;
			}
		}
	}
	return 0;
}
void updateErrorsInBoard(SudokuBoard* board){
	int n, m, N,i,j, error, curValue;
	i=0;
	j=0;
	error=0;
	curValue = 0;
	n = board->n;
	m = board->m;
	N = n*m;
	for (;i<N;i++){
		j=0;
		for (;j<N;j++){
			curValue = board->board[i][j].content;
			if (curValue!=0){
				error = !isLegalValue(board, i, j, curValue );
				board->board[i][j].isError = error;
			}
			else{
				board->board[i][j].isError = 0;
			}
		}
	}

}
int boardIsFull(SudokuBoard* board){
	int n,m,N,i,j;
	n = board->n;
	m = board->m;
	N = n*m;
	for (i = 0;i<N;i++){
		for (j = 0; j<N;j++){
			if (board->board[i][j].content==0){
				return 0;
			}
		}
	}
	return 1;
}
int boardIsEmpty(SudokuBoard* board){
	int n,m,N,i,j;
	n = board->n;
	m = board->m;
	N = n*m;
	for (i = 0;i<N;i++){
		for (j = 0; j<N;j++){
			if (board->board[i][j].content>0){
				return 0;
			}
		}
	}
	return 1;
}

int generateXY(SudokuGame* game,int x, int y){
	/*x,y<=N*N */
	int n,m,N,i,ind,try,allSuccess,indX,indY;
	int* xArray;
	int* nums;
	SudokuBoard* solvedBoard;

	SudokuBoard* board=game->curBoard->board;
	SudokuBoard* draftBoard=duplicateBoard(board);
	n = board->n;
	m = board->m;
	N = n*m;


	xArray=(int*)calloc((N*N)+1,sizeof(int));
	nums=(int*)calloc(N+1,sizeof(int));

	for (try = 0;try<1000;try++){
		allSuccess=1;
		xArray[0]=N*N;
		for (i = 1; i<=N*N;i++){
			xArray[i]=i;
		}
		nums[0]=N;
		for (i = 1; i<=N;i++){
			nums[i]=i;
		}
		for (i = 0; i<x;i++){
			ind=getRandIndex(xArray);
			indX=xArray[ind]/N;
			indY=xArray[ind]%N;

			if((manageArray(xArray,ind)==0)||(getPlausibleNums(draftBoard,indX,indY,nums)==0)){
				allSuccess=0;  /* failed to find a legal value for one of the board[indX][indY] */
			}
			else{
				ind=getRandIndex(nums); /* get index for a plausible number */
				draftBoard->board[indX][indY].content=nums[ind]; /* set random legal value in random x,y */
			}
		}
		solvedBoard=gurobi(draftBoard);
		if ((allSuccess=0)||(solvedBoard==NULL)){
			freeBoard(draftBoard);
			draftBoard=duplicateBoard(board);
		}
		else{
			clearYCells(solvedBoard,y,N);
			InsertBoardNextNode(game,solvedBoard);
			sudokuBoardPrinter(game); /*print */


			free(xArray);
			free(nums);
			freeBoard(draftBoard);
			return 1;
		}
	}
	printf("Error: puzzle generator failed\n");
	free(xArray);
	free(nums);
	freeBoard(draftBoard);
	return 0;
}

void dealWithFullBoard(SudokuGame* game){
	if (boardHasErrors(game->curBoard->board)==1){
		printf("Puzzle solution erroneous\n");
		game->onlyUndoAfterSolvedWithErrors=1;
	}
	else{
		printf("Puzzle solved successfully\n");
		freeGame(game);
		initGameInInitMode(game);
	}
}
int getPlausibleNums(SudokuBoard* board,int x, int y, int* pNums){
	int i=1;
	while(i<=pNums[0]){  /* pNums[0]=N init */
		if(isLegalValue(board,x,y,i)==0){
			manageArray(pNums,i);
		}
		else{
			i++;
		}
	}
	return pNums[0];
}
int num_solutions(SudokuBoard* board){
	SudokuBoard* workBoard;
	int i, j,n,m,N,result;
	i=0;
	j=0;
	if (boardHasErrors(board)==1){
		printf("Error: board contains erroneous values\n");
		return -1;
	}
	workBoard =  duplicateBoard(board);
	n=board->n;
	m=board->m;
	N = n*m;
	for (;i<N;i++){
		j=0;
		for(;j<N;j++){
			if (workBoard->board[i][j].content!=0){
				workBoard->board[i][j].isFixed=1;
			}
		}
	}
	result= numSolutionsFromPlace(workBoard, 0, 0);
	if (result == -1){
		return -1;
	}
	printf("Number of solutions: %d\n", result);
	if (result==1){
		printf("This is a good board!\n");
	}
	if (result > 1){
		printf("The puzzle has more than 1 solution, try to edit it further\n");
	}
	return result;

}
int numSolutionsFromPlace(SudokuBoard* board, int col, int row){


	return 1;

}



int manageArray(int* arr, int ind){
	/*given array and index, shifts left all values right to index.
	 *  returns new array size defacto. */
	int i=ind;
	if(arr[0]==0){
		return 0;
	}
	for (;i<=arr[0];i++){
		arr[i]=arr[i+1];
	}
	arr[0]--;
	return arr[0];
}
int getRandIndex(int* Arr){
	/*returns a random index from the list (based on optional indexes) for the random board generator */
	int size=Arr[0];
	int ran=0;

	if (size==0)
		return 0;
	ran=(rand()%size)+1;

	return ran;
}
void clearYCells(SudokuBoard* board, int y, int N){
	int i,ind;
	int* xArray=(int*)calloc((N*N)+1,sizeof(int));
	xArray[0]=N*N;
	for (i = 1; i<=N*N;i++){
		xArray[i]=i;
	}
	while(y>0){
		ind=getRandIndex(xArray);
		if(board->board[ind/N][ind%N].content>0){
			board->board[ind/N][ind%N].content=0;
			y--;
		}
	}
	free(xArray);
}
stackNode* getNewStackNode(int col, int row, int numToCheck){
	stackNode* result = (stackNode*)calloc(1, sizeof(stackNode));
	if (!result){
		printf("error allocating memory");
		return NULL;
	}
	result->col=col;
	result->row=row;
	result->numToCheck=numToCheck;
	return result;
}

stack* createNewEmptyStack(int max_num){
	stack* result = (stack*)calloc(1, sizeof(stack));
	assert (result);
	result->array = (stackNode*)calloc(max_num, sizeof(stackNode));
	result->numOfElements=0;
	result->max_num=max_num;
	return result;
}
void push(stack* stack, stackNode* nodeToPush){
	if(stack->numOfElements==stack->max_num){
		realloc(stack->array, 2*stack->max_num);
		stack->max_num*=2;
	}
	stack->array[stack->numOfElements]=*nodeToPush;
	stack->numOfElements++;

}
stackNode pop(stack* stack){
	assert (stack->numOfElements>0);
	stackNode result = stack->array[stack->numOfElements-1];
	stack->numOfElements--;
	return result;

}
stackNode peek(stack* stack){
	return stack->array[stack->numOfElements-1];
}
stackNode* copyNode(stackNode* nodeToCopy){
	stackNode* result = (stackNode*)calloc(1, sizeof(stackNode));
	assert (result);
	result->col = nodeToCopy->col;
	result->numToCheck=nodeToCopy->numToCheck;
	result->row=nodeToCopy->row;
	return result;
}
int isEmpty(stack* stack){
	return (stack->numOfElements==0);
}
void freeStack(stack* stack){
	free(stack->array);
	free(stack);
}


