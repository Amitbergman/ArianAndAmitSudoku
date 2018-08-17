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


void printSeperatorRow (int n, int N)
{
	/*Prints the delimeter between rows */
	int i = 0;
	for (; i < 4 * N + n + 1; i++)
	{
		printf ("-");
	}
	printf ("\n");
}

void printCell (SudokuCell cellToPrint, int isInEditModeOrMarkSetToOne)
{
	printf (" ");
	if (cellToPrint.content > 0)
	{
		printf ("%d", cellToPrint.content);
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
		if (cellToPrint.isError == 1 && isInEditModeOrMarkSetToOne == 1)
		{
			printf ("*");
		}
		else
		{
			printf (" ");
		}
	}
}

void printRealRow (SudokuBoard * boardPointer, int rowToPrint)
{

	SudokuBoard sudboard = *boardPointer;
	int i = 0;
	int n = sudboard.n;
	int m = sudboard.m;
	int N = n * m;

	int column = 0;
	for (; i < N + m + 1; i++){
		if (i % (n + 1) == 0){
			printf ("|");
		}
		else {
			printCell (sudboard.board[rowToPrint][column], 0);
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

void sudokuBoardPrinter (SudokuBoard * boardPointer)
{

	SudokuBoard board = *boardPointer;
	int n = board.n;
	int m = board.m;
	int N = n * m;

	int row = 0, i = 0;
	for (; i < N + n + 1; i++)
	{

		if (i % (m + 1) == 0)
			printSeperatorRow (n, N);

		else
		{
			printRealRow (&board, row);
			row++;
		}
		printf ("\n");
	}
}

SudokuBoard* createSudokuBoardFromArray(int ** array, int n, int m){
	int N = n*m;
	int i = 0;
	int j = 0;

	SudokuBoard* result = (SudokuBoard*)calloc(1, sizeof(SudokuBoard));
	assert(result!=NULL);
	(*result).n=n;
	(*result).m=m;
	(*result).board = (SudokuCell **) calloc (N, sizeof (SudokuCell *));
	assert ((*result).board!=NULL);
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
	char *row = (char*)calloc(1024, sizeof(char));

	int n = (*sudoku).n;
	int m = (*sudoku).m;
	int N = n*m;
	char buffer[1024];
	char* empty = "";
	for (int i = 0;i<N;i++){
		strcpy(buffer,empty );
		int a = ((*sudoku).board[rowToPrint][i]).content;
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
char* sudokuBoardToString(SudokuBoard* sudoku){
	int n = (*sudoku).n;
	int m = (*sudoku).m;
	int N = n*m;
	char *res = (char*)calloc(1024, sizeof(char));
	char buffer[1024];
	sprintf(buffer, "%d %d\n", m,n);
	strcat(res, buffer);
	for (int i=0;i<N;i++){
		strcat (res, rowToStr(sudoku, i));
		strcat(res, "\n");
	}
	return res;
}
SudokuBoard* newEmptyBoard(){

	int N=9;
	int i=0;
	int j=0;
	int** a = (int**)calloc(N, sizeof(int*));

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
	char* cur = str;

	while ((*cur) != '\0'){

		cur=cur+1;
		char x = *cur;
		printf("%c\n", x);
	}
	return NULL;
}
