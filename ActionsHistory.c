/*
 * ActionsHistory.c
 *
 *  Created on: Aug 17, 2018
 *      Author: User
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "printer.h"
#include "ActionsHistory.h"
#include "structs.h"

Node* GetNewNode(SudokuBoard* x) {
	Node* newNode = (Node*)calloc(1,sizeof(Node));
	if (!newNode){
		printf("Problem in memory allocating");
		exit(1);
	}
	newNode->board = x;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}
SudokuBoard* duplicateBoard(SudokuBoard* oldBoard) {
	int i,j,N;

	SudokuBoard* newBoard = (SudokuBoard*)calloc(1,sizeof(SudokuBoard));
	if(newBoard ==NULL){
		printf("Problem in memory allocating");
		exit(1);
	}
	newBoard->m=oldBoard->m;
	newBoard->n=oldBoard->n;
	N=(newBoard->m)*(newBoard->n);
	i=0;
	(*newBoard).board = (SudokuCell **) calloc (N, sizeof (SudokuCell *));
	if ((*newBoard).board==NULL){
		printf("Problem in memory allocating");
		exit(1);
	}
	for (;i<N;i++){
		j=0;
		((*newBoard).board)[i] = (SudokuCell *) calloc (N, sizeof (SudokuCell));
		if(((*newBoard).board)[i]==NULL){
			printf("Problem in memory allocating");
			exit(1);
		}
		for (;j<N;j++){
			SudokuCell* currentCell = createNewCell((*oldBoard).board[i][j].content,(*oldBoard).board[i][j].isFixed,(*oldBoard).board[i][j].isError);
			if (currentCell==NULL){
				printf("Problem in memory allocating");
				exit(1);
			}
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
	int row,col,N,z1,z2;
	N=(boardA->m)*(boardA->n);

	for (row=0;row<N;row++){
		for (col=0;col<N;col++){
			z1=boardA->board[col][row].content;
			z2=boardB->board[col][row].content;
			if (z1!=z2){
				if (z1==0){
					printf("%s %d,%d: from %c to %d\n",undoRedo,row+1,col+1,'_',z2);
				}
				else if (z2==0){
					printf("%s %d,%d: from %d to %c\n",undoRedo,row+1,col+1,z1,'_');
				}
				else{
					printf("%s %d,%d: from %d to %d\n",undoRedo,row+1,col+1,z1,z2);
				}
			}
		}
	}
}
void printDiffsAutoFill(SudokuBoard* boardA, SudokuBoard* boardB) {
	int row, col,N,z1,z2;
	row=0;
	col=0;
	N=(boardA->m)*(boardA->n);

	for (;row<N;row++){
		col=0;
		for (;col<N;col++){
			z1=boardA->board[col][row].content;
			z2=boardB->board[col][row].content;
			if (z1!=z2){
				printf("Cell <%d,%d> set to %d\n",col+1,row+1,z2);
			}
		}
	}
}


SudokuBoard* supplyEmptyNonNBoard(int N, int sqrt){
	int i,j;
	int **arr = (int**)calloc(N,sizeof(int*));
	if (!arr){
		printf("Problem in memory allocating");
		exit(1);
	}
	i=0;
	for (;i<N;i++){
		j=0;
		arr[i] = (int*)calloc(N,sizeof(int));
		if(!(arr[i])){
			printf("Problem in memory allocating");
			exit(1);
		}
		for(;j<N;j++)
		{
			arr[i][j]=0;
		}
	}
	return createSudokuBoardFromArray(arr, sqrt, sqrt);
}
/*Inserts a Node at head of doubly linked list
 *
 */
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

/* Inserts a Node at tail of Doubly linked list
 *
 */
void InsertAtTail(List *list,SudokuBoard* x) {
	Node* temp;
	Node* newNode;
	temp = (*list).head;
	newNode = GetNewNode(x);
	if((*list).head == NULL) {
		(*list).head = newNode;
		return;
	}
	while(temp->next != NULL) temp = temp->next; /* Go To last Node */
	temp->next = newNode;
	newNode->prev = temp;
}
/* Inserts a new Node after curNode and cleans next nodes
 *
 */
void InsertBoardNextNode(SudokuGame* game,SudokuBoard* newBoard) {
	Node* node;
	cleanNextNodes(game->curBoard->next); /* free proceeding nodes in history list */
	node=GetNewNode(newBoard); /* create new node for new board */
	node->prev=game->curBoard; /* update prev and next */
	game->curBoard->next=node;
	game->curBoard=node;
}
void cleanNextNodes (Node* node){
	Node* next;
	if (node==NULL){
		return;
	}
	next=node->next;
	freeBoard(node->board);
	cleanNextNodes(next);
	node->next=NULL;
	free(node);
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
		sudokuBoardPrinter(game);
		printDiffs(game->curBoard->next->board, game->curBoard->board, "Undo");
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
		sudokuBoardPrinter(game);
		printDiffs(game->curBoard->prev->board, game->curBoard->board, "Redo");
	}
	return;
}


