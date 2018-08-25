/*
 * gameUtils.h
 *
 *  Created on: Aug 17, 2018
 *      Author: User
 */

#ifndef GAMEUTILS_H_
#define GAMEUTILS_H_

void loadBoardFromFile(SudokuGame* game, char* fileToOpen, int mode);
void setBoard(SudokuGame* game, SudokuBoard* newBoard);
void setXYZ(SudokuGame* game, int* a);
void validate(SudokuBoard* board);
void hintXY(SudokuBoard* board, int x, int y);
void saveBoardToFile(SudokuGame* game, char* fileToOpen);
SudokuGame* initGameInInitMode();
void changeToEmptyGameInEditMode(SudokuGame*);
int isLegalValue(SudokuBoard * board, int col, int row, int valueToCheck);
int checkValidInBox(SudokuBoard* board, int col, int row, int n, int m, int valueToCheck);
void autofill(SudokuGame* game);
int boardHasErrors(SudokuBoard* board);

void hintXY(SudokuBoard* board, int x, int y);
void updateErrorsInBoard(SudokuBoard* board);

int boardIsFull(SudokuBoard* board);
int boardIsEmpty(SudokuBoard* board);

#endif /* GAMEUTILS_H_ */
