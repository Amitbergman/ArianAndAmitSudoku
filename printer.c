/*
 * printer.c
 *
 *  Created on: 12 ×‘×�×•×’ 2018
 *      Author: arian
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "printer.h"
#include "structs.h"


void printSeperatorRow (int m, int N)
{
	/*Prints the delimeter between rows */
	int i = 0;
	for (; i < 4 * N + m + 1; i++)
	{
		printf ("-");
	}

}

void printCell (SudokuCell cellToPrint, int isInEditModeOrMarkErrorsSetToOne)
{
	printf (" ");
	if (cellToPrint.content > 0)
	{
		printf ("%2d", cellToPrint.content);
	}
	else
	{
		printf ("  ");
	}
	if (cellToPrint.isFixed == 1)
	{
		printf (".");
	}
	else
	{
		if (cellToPrint.isError == 1 && isInEditModeOrMarkErrorsSetToOne == 1)
		{
			printf ("*");
		}
		else
		{
			printf (" ");
		}
	}
}

void printRealRow (SudokuBoard * boardPointer, int rowToPrint, int markErrors)
{

	int i,n,m,N, column;
	SudokuBoard sudboard = *boardPointer;
	i = 0;
	n = sudboard.n;
	m = sudboard.m;
	N = n * m;

	column = 0;
	for (; i < N + m + 1; i++){
		if (i % (n + 1) == 0){
			printf ("|");
		}
		else {
			printCell (sudboard.board[rowToPrint][column], markErrors);
			column ++;
		}
	}

}

SudokuCell* createNewCell(int content, int isFixed, int isError){
	SudokuCell * cellRef = (SudokuCell*)calloc(1, sizeof(SudokuCell));
	if (cellRef){
		(*cellRef).content=content;
		(*cellRef).isError=isError;
		(*cellRef).isFixed=isFixed;
		return cellRef;
	}
	else{
		return NULL;
	}
}

void sudokuBoardPrinter (SudokuGame * game)
{

	int i,n,m,N, row, markErrors;
	SudokuBoard* board;
	board = game->curBoard->board;

	n = board->n;
	m = board->m;
	N = n * m;
	row = 0;
	i = 0;
	markErrors = 0;
	if (game->markErrors==1 || game->gameMode==2){
		markErrors=1;
	}

	for (; i < N + n + 1; i++)
	{

		if (i % (m + 1) == 0)
			printSeperatorRow (m, N);

		else
		{
			printRealRow (board, row, markErrors);
			row++;
		}
		printf ("\n");
	}
}

SudokuBoard* createSudokuBoardFromArray(int ** array, int n, int m){
	int i,j,N;

	SudokuBoard* result = (SudokuBoard*)calloc(1, sizeof(SudokuBoard));
	assert(result!=NULL);
	(*result).n=n;
	(*result).m=m;
	N = n*m;
	(*result).board = (SudokuCell **) calloc (N, sizeof (SudokuCell *));
	assert ((*result).board!=NULL);
	i = 0;
	for (;i<N;i++){
		j=0;
		((*result).board)[i] = (SudokuCell *) calloc (N, sizeof (SudokuCell));
		assert(((*result).board)[i]!=NULL);
		for (;j<N;j++){
			SudokuCell* currentCell = createNewCell(array[i][j],0,0);
			assert (currentCell!=NULL);
			((((*result).board)[i])[j]) = *currentCell;
		}
	}
	return result;

}

char* rowToStr(SudokuBoard* sudoku, int rowToPrint ){
	int i,n,m,N,a;
	char buffer[1024];
	char* empty = "";
	char *row = (char*)calloc(1024, sizeof(char));
	n = (*sudoku).n;
	m = (*sudoku).m;
	N = n*m;
	i=0;
	for (;i<N;i++){
		strcpy(buffer,empty );
		a = ((*sudoku).board[rowToPrint][i]).content;
		if (((*sudoku).board[rowToPrint][i]).isFixed ==1){
			sprintf(buffer, "%d.", a );
		}
		else{
			sprintf(buffer, "%d", a );
		}
		if (i!=N-1){
			strcat(buffer, " ");
		}
		strcat(row,buffer );
	}
	return row;

}
SudokuBoard* newEmptyBoard(){
	int i,j,N;
	int** a;

	N=9;
	i=0;
	a = (int**)calloc(N, sizeof(int*));

	for (; i<N;i++){
		j=0;
		a[i] = (int *)calloc(N, sizeof(int));
		for (; j<N; j++){
			a[i][j]= 0;
		}
	}
	return createSudokuBoardFromArray(a,3,3);
}
SudokuBoard* stringToSudokuBoard(char * str){
	char x;
	char* cur = str;

	while ((*cur) != '\0'){

		cur=cur+1;
		x = *cur;
		printf("%c\n", x);
	}
	return NULL;
}
