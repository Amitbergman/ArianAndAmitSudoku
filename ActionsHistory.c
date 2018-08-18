/*
 * ActionsHistory.c
 *
 *  Created on: Aug 17, 2018
 *      Author: User
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "printer.h"
#include "ActionsHistory.h"
#include "structs.h"

Node* GetNewNode(SudokuBoard* x) {
	Node* newNode = (Node*)calloc(1,sizeof(Node));
	newNode->board = x;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}
SudokuBoard* duplicateBoard(SudokuBoard* oldBoard) {
	SudokuBoard* newBoard = (SudokuBoard*)calloc(1,sizeof(SudokuBoard));
	assert(newBoard!=NULL);
	newBoard->m=oldBoard->m;
	newBoard->n=oldBoard->n;
	int N=(newBoard->m)*(newBoard->n);
	int i=0; int j=0;
	(*newBoard).board = (SudokuCell **) calloc (N, sizeof (SudokuCell *));
	assert ((*newBoard).board!=NULL);
	for (;i<N;i++){
		j=0;
		((*newBoard).board)[i] = (SudokuCell *) calloc (N, sizeof (SudokuCell));
		assert(((*newBoard).board)[i]!=NULL);
		for (;j<N;j++){
			SudokuCell* currentCell = createNewCell((*oldBoard).board[i][j].content,(*oldBoard).board[i][j].isFixed,(*oldBoard).board[i][j].isError);
			assert (currentCell!=NULL);
			((((*newBoard).board)[i])[j]) = *currentCell;
		}
	}
	return newBoard;
}

SudokuBoard* supplyEmptyNonNBoard(int N, int sqrt){

	int **arr = (int**)calloc(N,sizeof(int*));
	assert (arr!=NULL);
	for (int i=0;i<N;i++){
		arr[i] = (int*)calloc(N,sizeof(int));
		assert(arr[i]!=NULL);
		for(int j=0;j<N;j++)
		{
			arr[i][j]=0;
		}
	}
	return createSudokuBoardFromArray(arr, sqrt, sqrt);
}
//Inserts a Node at head of doubly linked list
void InsertAtHead(List* list,SudokuBoard* newContent) {
	struct Node* newNode = GetNewNode(newContent);
	if((*list).head == NULL) {
		(*list).head = newNode;
		return;
	}
	(*list).head->prev = newNode;
	(*newNode).next = (*list).head;
	(*list).head = newNode;
}

//Inserts a Node at tail of Doubly linked list
void InsertAtTail(List *list,SudokuBoard* x) {
	Node* temp = (*list).head;
	Node* newNode = GetNewNode(x);
	if((*list).head == NULL) {
		(*list).head = newNode;
		return;
	}
	while(temp->next != NULL) temp = temp->next; // Go To last Node
	temp->next = newNode;
	newNode->prev = temp;
}

void undo (SudokuGame* game){
	Node *nodeBoard=game->curBoard->prev;
	if (nodeBoard==NULL){
		printf("Error: no moves to undo\n");
	}
	else{
		game->curBoard=nodeBoard;

		sudokuBoardPrinter(game->curBoard->board);
	}
	return;
}
void redo (SudokuGame* game){
	Node *nodeBoard=game->curBoard->next;
	if (nodeBoard==NULL){
		printf("Error: no moves to redo\n");
	}
	else{
		game->curBoard=nodeBoard;
		sudokuBoardPrinter(game->curBoard->board);
	}
	return;
}

//void undo (SudokuGame* game){
//	if ((*(game.list)).prev!=NULL){
//		(game->list)=(*(game->list).prev);
//		printChanges(before,after);
//	}
//   else
//	printf("Error: no moves to undo\n")
//
//}

