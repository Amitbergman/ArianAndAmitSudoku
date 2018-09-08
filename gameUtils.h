/*
 * gameUtils.h
 *
 *  Created on: Aug 17, 2018
 *      Author: User
 */

#ifndef GAMEUTILS_H_
#define GAMEUTILS_H_
void freeStack(stack* stack);
void increaseHeadOfStackByOne(stack* stacker);
void loadBoardFromFile(SudokuGame* game, char* fileToOpen, int mode);
void setXYZ(SudokuGame* game, int* a);
void validate(SudokuBoard* board);
void hintXY(SudokuBoard* board, int x, int y);
void saveBoardToFile(SudokuGame* game, char* fileToOpen);
SudokuGame* initGameInInitMode();
void freeGame(SudokuGame* game);
void changeToEmptyGameInEditMode(SudokuGame*);
int isLegalValue(SudokuBoard * board, int col, int row, int valueToCheck);
int checkValidInBox(SudokuBoard* board, int col, int row, int n, int m, int valueToCheck);
void autofill(SudokuGame* game);
int boardHasErrors(SudokuBoard* board);
void dealWithFullBoard(SudokuGame* game);
int countNumberOfSolutions(SudokuBoard* board);
int num_solutions(SudokuBoard* board);

void hintXY(SudokuBoard* board, int x, int y);
void updateErrorsInBoard(SudokuBoard* board);

int boardIsFull(SudokuBoard* board);
int boardIsEmpty(SudokuBoard* board);
int generateXY(SudokuGame* game,int x, int y);
int getPlausibleNums(SudokuBoard* board,int x, int y, int* pNums);
int manageArray(int* arr, int ind);
int isStackLegal(stackNode* stack, int curStackCount);
int getRandIndex(int* Arr);
stackNode* getNewStackNode(int col, int row, int numToCheck);
void clearYCells(SudokuBoard* board, int y, int N);
stackNode pop(stack* stack);
void push(stack* stack, stackNode* nodeToPush);
stackNode peek(stack* stack);
stack* createNewEmptyStack();
int isEmpty(stack* stack);
void freeStack(stack* stack);
void increaseHeadOfStackByOne(stack* stacker);

#endif /* GAMEUTILS_H_ */
