#ifndef ACTIONSHISTORY_H_
#define ACTIONSHISTORY_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "printer.h"
#include "parser.h"
#include "ActionsHistory.h"
#include "SPBufferset.h"
#include "structs.h"

/*
 * Returns a new Node of the undo redo list which contains the sudoku board recieved
 */
Node* GetNewNode(SudokuBoard* x);


/*
 * Duplicated the sudokuboard recieved and returns reference to new identical board
 * we use it for the undo redo list, hence we decided to put it in this module
 */
SudokuBoard* duplicateBoard(SudokuBoard* oldBoard);


/* Inserts a new Node after curNode and cleans next nodes
 *
 */
void InsertBoardNextNode(SudokuGame* game,SudokuBoard* newBoard);


/* frees the nodes in the list that are after the node in the input
 *
 */
void cleanNextNodes (Node* node);

/*Inserts a Node at head of doubly linked list
 *
 */
void InsertAtHead(List* list,SudokuBoard* newContent);

/*Executes the undo command as describes in the description of the project
 *
 */
void undo (SudokuGame* game);

/*Executes the redo command as describes in the description of the project
 *
 */
void redo (SudokuGame* game);

/*
 * resets the list of undo redo of the game
 */
void resetGame(SudokuGame* game);

/*
 * prints the differences between the boards after autodfill
 */
void printDiffsAutoFill(SudokuBoard* boardA, SudokuBoard* boardB);

#endif /* ACTIONSHISTORY_H_ */

