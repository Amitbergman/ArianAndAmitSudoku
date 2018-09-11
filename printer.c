/*
 * printer.c
 *
 *  Created on: 12 ×‘×�×•×’ 2018
 *      Author: arian
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
			printCell (sudboard.board[column][rowToPrint], markErrors);
			column ++;
		}
	}

}

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

void sudokuBoardPrinter (SudokuGame* game)
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
