#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "printer.h"
#include "ActionsHistory.h"
#include "structs.h"
#include "gameUtils.h"

/*
 * Returns a new Node of the undo redo list which contains the sudoku board recieved
 */
Node* GetNewNode(SudokuBoard* boardToAdd) {
	Node* newNode = (Node*)calloc(1,sizeof(Node));
	if (!newNode){
		printf("Problem in memory allocating");
		exit(1);
	}
	newNode->board = boardToAdd;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

/*
 * Duplicated the sudokuboard recieved and returns reference to new identical board
 * we use it for the undo redo list, hence we decided to put it in this module
 */
SudokuBoard* duplicateBoard(SudokuBoard* oldBoard) {
	int i,j,N;
	SudokuCell* currentCell;

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
			currentCell = createNewCell((*oldBoard).board[i][j].content,(*oldBoard).board[i][j].isFixed,(*oldBoard).board[i][j].isError);
			if (currentCell==NULL){
				printf("Problem in memory allocating");
				exit(1);
			}

			((((*newBoard).board)[i])[j]) = *currentCell;
			free(currentCell);

		}
	}
	return newBoard;
}

/*
 * resets the list of undo redo of the game
 */
void resetGame(SudokuGame* game){
	game->curBoard=game->history->head;
	cleanNextNodes(game->curBoard->next);
	game->curBoard->next=NULL;
	printf("Board reset\n");
}

/*
 * prints the differences between the boards after set or undo or redo
 */
void printDiffs(SudokuBoard* boardA, SudokuBoard* boardB, char* undoRedo) {
	int row,col,N,z1,z2;
	N=(boardA->m)*(boardA->n);

	for (row=0;row<N;row++){
		for (col=0;col<N;col++){
			z1=boardA->board[col][row].content;
			z2=boardB->board[col][row].content;
			if (z1!=z2){
				if (z1==0){
					printf("%s %d,%d: from %c to %d\n",undoRedo,col+1,row+1,'_',z2);
				}
				else if (z2==0){
					printf("%s %d,%d: from %d to %c\n",undoRedo,col+1,row+1,z1,'_');
				}
				else{
					printf("%s %d,%d: from %d to %d\n",undoRedo,col+1,row+1,z1,z2);
				}
			}
		}
	}
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

/* frees the nodes in the list that are after the node in the input
 *
 */
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

/*Executes the undo command as describes in the description of the project
 *
 */
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

/*Executes the redo command as describes in the description of the project
 *
 */
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


/*
 * prints the differences between the boards after autodfill
 */
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


