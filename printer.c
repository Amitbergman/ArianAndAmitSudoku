#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "printer.h"
#include "structs.h"
#include "gameUtils.h"

/*prints a seperator row between real rows */
void printSeperatorRow (int m, int N)
{
	/*Prints the delimeter between rows */
	int i = 0;
	for (; i < 4 * N + m + 1; i++)
	{
		printf ("-");
	}

}

/*prints a cell of the sudoku */
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

/*prints a real row of the sudoku */
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

/*prints the sudoku board of the game */
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

