/*
 * printer.h
 *
 *  Created on: 12 ×‘×�×•×’ 2018
 *      Author: arian
 */

#ifndef PRINTER_H_
#define PRINTER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "main.h"

void printSeperatorRow (int n, int N);
void printCell (SudokuCell cellToPrint, int isInEditModeOrMarkSetToOne);
void printRealRow (SudokuBoard * boardPointer, int rowToPrint);
SudokuCell* createNewCell(int content, int isFixed, int isError);
void sudokuBoardPrinter (SudokuBoard * boardPointer);
SudokuBoard* createSudokuBoardFromArray(int ** array, int n, int m);
char* rowToStr(SudokuBoard* sudoku, int rowToPrint );
char* sudokuBoardToString(SudokuBoard* sudoku);
#endif /* PRINTER_H_ */