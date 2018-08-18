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
	Node* newNode
		= (Node*)malloc(sizeof(Node));
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
void resetGame(SudokuGame* game){
	game->curBoard=game->history->head;
	cleanNextNodes(game->curBoard->next);
	game->curBoard->next=NULL;
	printf("Board reset\n");
}
void printDiffs(SudokuBoard* boardA, SudokuBoard* boardB, char* undoRedo) {
	int N=(boardA->m)*(boardA->n);
	int i=0; int j=0;
	int z1; int z2;

	for (;i<N;i++){
		j=0;

		for (;j<N;j++){
			z1=boardA->board[i][j].content;
			z2=boardB->board[i][j].content;

			if (z1!=z2){
				printf(undoRedo);
				printf(" %d,%d: from %d to %d\n",i+1,j+1,z1,z2);
			}
		}
	}
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
void cleanNextNodes (Node* node){
	if (node==NULL){
		return;
	}
	Node* next=node->next;
	freeBoard(node->board);
	cleanNextNodes(next);
	node->next=NULL;
	return;
}
void freeBoard(SudokuBoard* board){
	int i=0;
	int N=(board->m)*(board->n);
	for (;i<N;i++){
		free(board->board[i]);
	}
	free(board);
	return;
}
void undo (SudokuGame* game){
	Node *nodeBoard=game->curBoard->prev;
	if (nodeBoard==NULL){
		printf("Error: no moves to undo\n");
	}
	else{
		game->curBoard=nodeBoard;
		printDiffs(game->curBoard->next->board, game->curBoard->board, "Undo");
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
		printDiffs(game->curBoard->prev->board, game->curBoard->board, "Redo");
		sudokuBoardPrinter(game->curBoard->board);
	}
	return;
}


