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

Node* GetNewNode(SudokuBoard* x) {
	Node* newNode
		= (Node*)malloc(sizeof(Node));
	newNode->board = x;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
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

//Prints all elements in linked list in reverse traversal order.
//void undo (SudokuGame* game){
//	if ((*(game.list)).prev!=NULL){
//		(game->list)=(*(game->list).prev);
//		printChanges(before,after);
//	}
//   else
//	printf("Error: no moves to undo\n")
//
//}

