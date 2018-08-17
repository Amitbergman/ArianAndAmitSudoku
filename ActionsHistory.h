/*
 * ActionsHistory.h
 *
 *  Created on: Aug 17, 2018
 *      Author: User
 */

#ifndef ACTIONSHISTORY_H_
#define ACTIONSHISTORY_H_


typedef struct Node  {
	SudokuBoard* board;
	struct Node* next;
	struct Node* prev;
}Node;
typedef struct List{
	Node* head;
} List;
SudokuBoard* supplyEmptyNonNBoard(int N, int sqrt);
void InsertAtHead(List* list,SudokuBoard* newContent);
void InsertAtTail(List *list,SudokuBoard* x);
#endif /* ACTIONSHISTORY_H_ */
