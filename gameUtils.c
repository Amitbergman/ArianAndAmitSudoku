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

/*recieves a file name and mode and loads the gameboard from the filename to the current game */
void loadBoardFromFile(SudokuGame* game, char* fileToOpen, int mode){

	FILE * fp;
	int n,m,N,i,j, check,curCellContent;
	char* curChar;
	SudokuBoard* resBoard;

	fp = fopen (fileToOpen, "r");
	if (!fp){
		if (mode ==1){
			printf("Error: file doesn't exist or cannot be opened\n");

		}
		if (mode ==2){
			printf("Error: file cannot be opened\n");

		}
		return;

	}

	n =0;
	m =0 ;
	check = fscanf(fp, "%d", &m);
	if (check!=1){
		printf("Problem with the format of the file being read");
		exit(1);
	}
	fscanf(fp, "%d", &n);
	if (check!=1){
		printf("Problem with the format of the file being read");
		exit(1);
	}
	resBoard = newEmptyBoard(n,m);
	N = n*m;
	(*resBoard).m=m;
	(*resBoard).n=n;

	curChar = (char *)calloc(1,sizeof(char));
	if (!curChar){
		printf("Problem in memory allocating");
		exit(1);

	}
	(*game).gameMode = mode;
	curCellContent=0;
	for (j=0;j<N;j++){
		for (i=0;i<N;i++){
			fscanf(fp, "%d", &curCellContent);
			if(curCellContent<0||curCellContent>n*m){
				printf("Error: board file contains ilegal value");
				exit(1);
			}
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
	check = fclose(fp);
	if (check!=0){
		printf("Problem while closing the file");
		exit(1);
	}

	cleanNextNodes(game->history->head); /*free history */
	game->curBoard=GetNewNode(resBoard);

	game->history->head = game->curBoard;
	updateErrorsInBoard(game->curBoard->board);
	/*sudokuBoardPrinter(game);   no need to print! */
	free(curChar);
}

/*sets the x y cell to be z
 * when a[0]=x
 * a[1]=y
 * a[2]=z
 *  */
void setXYZ(SudokuGame* game, int* a){

	SudokuBoard* newBoard;

	if ((game->gameMode==1)&&(game->curBoard->board->board[a[0]-1][a[1]-1].isFixed==1)){ /* value fixed in solve mode */
		printf("Error: cell is fixed\n");
		return;
	}

	if (game->curBoard->board->board[a[0]-1][a[1]-1].content==a[2]){
		return; /* if no change is made, do nothing  */
	}

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

	return;
}

/*validated the board
 * and prints the result og the validation */
void validate(SudokuBoard* board){
	SudokuBoard* result;
	if(boardHasErrors(board)){
		printf("Error: board contains erroneous values\n");
		return;
	}
	result = gurobi(board);
	if (result==NULL){
		printf("Validation failed: board is unsolvable\n");
	}
	else{
		freeBoard(result);
		printf("Validation passed: board is solvable\n");
	}
}

/*give hint to the user */
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

/*save the current board to the file specified in the variable fileToOpen */
void saveBoardToFile(SudokuGame* game, char* fileToOpen){
	int m,n,N,row,col,a,check;
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
	for (row=0;row<N;row++){
		for (col=0;col<N;col++){
			fprintf(fp, "%d", (game->curBoard->board->board)[col][row].content);
			if ((game->curBoard->board->board)[col][row].isFixed || (game->gameMode==2 && game->curBoard->board->board[col][row].content!=0)){
				fprintf(fp, ".");
			}
			if (col==N-1){
				fprintf(fp, "\n");
			}else{
				fprintf(fp, " ");
			}
		}
	}
	check = fclose(fp);
	if (check!=0){
		printf("Problem while closing the file");
		exit(1);
	}
	printf("Saved to: %s\n",fileToOpen);
	/*sudokuBoardPrinter(game); no need to print!  */
}

/*initializes a geme in init mode */
SudokuGame* initGameInInitMode(){
	SudokuGame* game = (SudokuGame*)calloc(1,sizeof(SudokuGame));
	if (!game){
		printf("Problem in memory allocating");
		exit(1);
	}
	game->gameMode=0; /* init */
	game->markErrors=1;
	game->history=(List*)calloc(1,sizeof(List));
	if (!(game->history)){
		printf("Problem in memory allocating");
		exit(1);
	}

	return game;
}

/*free all resources used by game */
void freeGame(SudokuGame* game){
	cleanNextNodes(game->history->head); /* clear and free history */
	free(game->history);

	free(game);
}

/*changes the status to empty game in edit mode
 * used when user writed "edit" */
void changeToEmptyGameInEditMode(SudokuGame* game){

	game->gameMode=2; /* 0-init 1-solve 2-edit */
	cleanNextNodes(game->history->head);
	game->history->head=GetNewNode(newEmptyBoard(3,3));
	game->curBoard=game->history->head;
	/*sudokuBoardPrinter(game);   no need to print!  */
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

/* return 0 if value is not legal and 1 if is legal in the current board */
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

/* return 0 if value is not legal inside box and 1 if is legal */
int checkValidInBox(SudokuBoard* board, int col, int row, int n, int m, int valueToCheck){
	int startCol,startRow, i,j;
	startCol= (col/n)*n;
	 startRow = (row/m)*m;
	 i = startCol;
	 j = startRow;
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

/* executes the autofill command on the game board */
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

/* returns 1 iff board has errors */
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

/* goes through all board and updates cells that are errors */
void updateErrorsInBoard(SudokuBoard* board){
	int n, m, N,col,row, error, curValue;
	col=0;
	row=0;
	error=0;
	curValue = 0;
	n = board->n;
	m = board->m;
	N = n*m;
	for (;col<N;col++){
		row=0;

		for (;row<N;row++){
			curValue = board->board[col][row].content;
			if (curValue!=0){

				error = !isLegalValue(board, col, row, curValue );
				board->board[col][row].isError = error;
			}
			else{
				board->board[col][row].isError = 0;
			}
		}
	}

}

/* returns 1 iff board is full with values */
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

/* returns 1 iff board is empty */
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

/* executes the generate command as written in the project description */
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
		for (i = 1; i<N*N;i++){
			xArray[i]=i-1;
		}  /* xArray = an array of [arraySize,0,...,N*N-1] */
		nums[0]=N;
		for (i = 1; i<=N;i++){
			nums[i]=i;
		}  /* nums = an array of [arraySize,0,...,N] */
		for (i = 0; i<x;i++){
			ind=getRandIndex(xArray); /* get rand num in range [0,N*N] */
			indX=xArray[ind]/N;
			indY=xArray[ind]%N;
			/* indX,indY in range [1,N] */

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

/* gets a full board, checks if it is successfully solved or not
 * and acts like we were told in the project description */
void dealWithFullBoard(SudokuGame* game){
	if (boardHasErrors(game->curBoard->board)==1){
		printf("Puzzle solution erroneous\n");
	}
	else{
		printf("Puzzle solved successfully\n");
		game->gameMode=0;
	}
}

/* returns the number of numbers that fit to the x,y cell */
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

/* executes the num_solutions command from the project menu
 * prints the number of solutions */
int num_solutions(SudokuBoard* board){
	SudokuBoard* workBoard;
	int i, j,n,m,N,result;
	i=0;
	j=0;
	if (boardHasErrors(board)==1){
		printf("Error: board contains erroneous values\n");
		return 0;
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
	result= countNumberOfSolutions(workBoard);
	printf("Number of solutions: %d\n", result);
	if (result==1){
		printf("This is a good board!\n");
	}
	if (result > 1){
		printf("The puzzle has more than 1 solution, try to edit it further\n");
	}
	freeBoard(workBoard);

	return result;

}

/* gets a board and counts the number of possible solutions */
int countNumberOfSolutions(SudokuBoard* board){

	int counter,n,m,N;
	stack* workStack;
	stackNode* currentNode;
	stackNode* nodeToPush;
	counter=0;
	m = board->m;
	n = board->n;
	N = m*n;
	currentNode = getNewStackNode(0,0,1);
	workStack = createNewEmptyStack(N*N+1);
	push(workStack, currentNode);
	free(currentNode);
	while(workStack->numOfElements > 0){
		currentNode = peek(workStack);
		if (currentNode->numToCheck>N){  /*crossed all the numbers */

			if (currentNode->col==0&&currentNode->row==0){
				break;
			}
			else{
				if (board->board[currentNode->col][currentNode->row].isFixed!=1){
					board->board[currentNode->col][currentNode->row].content=0;
				}
				pop(workStack);
				increaseHeadOfStackByOne(workStack);
			}
		}
		else{
			if (board->board[currentNode->col][currentNode->row].isFixed==1){
				/* fixed */
				if (board->board[currentNode->col][currentNode->row].content==currentNode->numToCheck){
					if (isLegalValue(board,currentNode->col,currentNode->row,currentNode->numToCheck)==1){
						if (currentNode->col==N-1&&currentNode->row==N-1){
							counter++;
							pop(workStack);
							increaseHeadOfStackByOne(workStack);
						}
						else{
							if (currentNode->col!=N-1){
								nodeToPush = getNewStackNode(currentNode->col+1, currentNode->row, 1);
								push(workStack, nodeToPush );
								free(nodeToPush);
							}
							else{
								nodeToPush = getNewStackNode(0, currentNode->row+1, 1);
								push(workStack,nodeToPush );
								free(nodeToPush);

							}
						}
					}
					else{
						increaseHeadOfStackByOne(workStack);
					}
				}
				else{
					increaseHeadOfStackByOne(workStack);
				}
			}
			else{	/* not fixed */
				if(isLegalValue(board, currentNode->col, currentNode->row, currentNode->numToCheck)==1){
					if (currentNode->col==N-1&&currentNode->row==N-1){
						counter++;
						board->board[currentNode->col][currentNode->row].content=0;
						/* maybe to change that pop will get reference */
						pop(workStack);
						increaseHeadOfStackByOne(workStack);
					}
					else{
						board->board[currentNode->col][currentNode->row].content=currentNode->numToCheck;
						if (currentNode->col!=N-1){
							nodeToPush = getNewStackNode(currentNode->col+1, currentNode->row, 1);
							push(workStack, nodeToPush);
							free(nodeToPush);
						}
						else{
							nodeToPush = getNewStackNode(0, currentNode->row+1, 1);
							push(workStack,nodeToPush);
							free(nodeToPush);

						}
					}

				}
				else{	/*not fixed not legal */
					increaseHeadOfStackByOne(workStack);

				}
			}
		}
	}

	freeStack(workStack);
	return counter;
}

/*given array and index, shifts left all values right to index.
	 *  returns new array size defacto. if returns 0 than array is empty*/
int manageArray(int* arr, int ind){
	/* ind is in range [1,arr[0]] */
	int i;
	if(arr[0]==0){
		return 0;
	}
	for (i=ind;i<arr[0];i++){
		arr[i]=arr[i+1];
	}
	arr[0]--;
	return arr[0];
}

/*returns a random index from the list
 * in range [1,size] ; size=Arr[0].
 * (based on optional indexes)
 * for the random board generator.
 *  */
int getRandIndex(int* Arr){

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
	if (!xArray){
		printf("Problem in memory allocating");
		exit(1);
	}
	xArray[0]=N*N;
	for (i = 1; i<=N*N;i++){
		xArray[i]=i-1;
	}
	for (i=0;i<y;i++){
		ind=getRandIndex(xArray);
		board->board[xArray[ind]/N][xArray[ind]%N].content=0;
		manageArray(xArray,ind);
	}
	/*
	while(y>0){
		ind=getRandIndex(xArray);
		if(board->board[ind/N][ind%N].content>0){
			board->board[ind/N][ind%N].content=0;
			y--;
		}
	}
	*/
	free(xArray);
}

/* returns a new stackNode */
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

/* returns a new empty stack */
stack* createNewEmptyStack(int max_num){
	stack* result = (stack*)calloc(1, sizeof(stack));
	if (!result){
		printf("Problem in memory allocating");
		exit(1);
	}
	result->array = (stackNode*)calloc(max_num, sizeof(stackNode));
	if (!(result->array)){
		printf("Problem in memory allocating");
		exit(1);
	}
	result->numOfElements=0;
	result->max_num=max_num;
	return result;
}

/* pushes the node to the stack */
void push(stack* stack, stackNode* nodeToPush){
	if(stack->numOfElements==stack->max_num){
		if ((realloc(stack->array, 2*stack->max_num)==NULL)){
			printf("problem allocating while pushing");
			exit(1);
		}
		stack->max_num*=2;
	}
	stack->array[stack->numOfElements]=*nodeToPush;
	stack->numOfElements++;


}

/* pops from the stack and returns the popped node */
stackNode pop(stack* stack){
	stackNode result;
	assert (stack->numOfElements>0);
	result = stack->array[stack->numOfElements-1];
	stack->numOfElements--;
	return result;

}

/* peek in the head of the stack*/
stackNode* peek(stack* stack){
	return &(stack->array[stack->numOfElements-1]);
}

/* frees the stack  and its content after we finish using it*/
void freeStack(stack* stack){

	free(stack->array);
	free(stack);
}

/* increases the number to check in the head of the stacy by 1*/
void increaseHeadOfStackByOne(stack* stacker){

	stacker->array[stacker->numOfElements-1].numToCheck+=1;

}

/* creates a new sudokecell*/
SudokuCell* createNewCell(int content, int isFixed, int isError){
	SudokuCell * cellRef = (SudokuCell*)calloc(1, sizeof(SudokuCell));
	if (!cellRef){
		printf("Problem in memory allocating");
		exit(1);
	}
		(*cellRef).content=content;
		(*cellRef).isError=isError;
		(*cellRef).isFixed=isFixed;
		return cellRef;
}

/* creates a new empty sudoku board*/
SudokuBoard* newEmptyBoard(int n, int m){
	int i,j,N;
	int** a;
	SudokuBoard* res;

	N=n*m;
	i=0;
	a = (int**)calloc(N, sizeof(int*));
	if (!a){
		printf("Problem in memory allocating");
		exit(1);
	}

	for (; i<N;i++){
		j=0;
		a[i] = (int *)calloc(N, sizeof(int));
		if (!(a[i])){
			printf("Problem in memory allocating");
			exit(1);
		}
		for (; j<N; j++){
			a[i][j]= 0;
		}
	}
	res= createSudokuBoardFromArray(a,n,m);
	i=0;
	for (;i<N;i++){
		free(a[i]);
	}
	free(a);
	return res;
}

/* creates a new sudokuboard from the array of ints specified*/
SudokuBoard* createSudokuBoardFromArray(int ** array, int n, int m){
	int i,j,N;
	SudokuCell* currentCell;

	SudokuBoard* result = (SudokuBoard*)calloc(1, sizeof(SudokuBoard));
	if(result==NULL){
		printf("Problem in memory allocating");
		exit(1);
	}
	(*result).n=n;
	(*result).m=m;
	N = n*m;
	(*result).board = (SudokuCell **) calloc (N, sizeof (SudokuCell *));
	if ((*result).board==NULL){
		printf("Problem in memory allocating");
		exit(1);
	}
	i = 0;
	for (;i<N;i++){
		j=0;
		((*result).board)[i] = (SudokuCell *) calloc (N, sizeof (SudokuCell));
		if(((*result).board)[i]==NULL){
			printf("Problem in memory allocating");
			exit(1);
		}
		for (;j<N;j++){
			currentCell = createNewCell(array[i][j],0,0);
			if (currentCell==NULL){
				printf("Problem in memory allocating");
				exit(1);
			}
			result->board[i][j]=*currentCell;
			free(currentCell);
		}
	}

	return result;

}

