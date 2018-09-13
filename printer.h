#ifndef PRINTER_H_
#define PRINTER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "structs.h"


/*prints a seperator row between real rows */
void printSeperatorRow (int n, int N);

/*prints a cell of the sudoku */
void printCell (SudokuCell cellToPrint, int isInEditModeOrMarkSetToOne);

/*prints a real row of the sudoku */
void printRealRow (SudokuBoard * boardPointer, int rowToPrint, int markErrors);

/*prints the sudoku board of the game */
void sudokuBoardPrinter (SudokuGame* game);

#endif /* PRINTER_H_ */
