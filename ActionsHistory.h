/*
 * ActionsHistory.h
 *
 *  Created on: Aug 17, 2018
 *      Author: User
 */

#ifndef ACTIONSHISTORY_H_
#define ACTIONSHISTORY_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "main.h"
#include "printer.h"
#include "parser.h"
#include "ActionsHistory.h"
#include "SPBufferset.h"
#include "structs.h"

Node* GetNewNode(SudokuBoard* x);
SudokuBoard* supplyEmptyNonNBoard(int N, int sqrt);
void InsertAtHead(List* list,SudokuBoard* newContent);
void InsertAtTail(List *list,SudokuBoard* x);
SudokuBoard* duplicateBoard(SudokuBoard* oldBoard);
#endif /* ACTIONSHISTORY_H_ */
