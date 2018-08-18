/*
 * gameUtils.c
 *
 *  Created on: Aug 17, 2018
 *      Author: User
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "main.h"
#include "printer.h"
#include "ActionsHistory.h"


void loadBoardFromFile(SudokuGame* game, char* fileToOpen, int mode){
//TODO - function to free all memory of previous game
	SudokuBoard* resBoard = newEmptyBoard();
	(*game).gameMode = mode;
	(*game).markErrors = 0;
	FILE * fp;

	fp = fopen (fileToOpen, "r");
	if (!fp){
		printf("Error: file doesn't exist or cannot be opened\n");
		return;
	}

	//Now the Buffer Has all the data from the file
	int n =0;
	int m =0 ;
	fscanf(fp, "%d %d\n", &m,  &n);
	int N = n*m;
	(*resBoard).m=m;
	(*resBoard).n=n;

	char* curChar = (char *)calloc(1,sizeof(char));
	int curCellContent=0;
	for (int i=0;i<N;i++){
		for (int j=0;j<N;j++){
			fscanf(fp, "%d", &curCellContent);
			(*resBoard).board[i][j].content=curCellContent;
			(*resBoard).board[i][j].isError=0;
			fscanf(fp, "%c", curChar);
			if (strcmp(curChar, ".")==0){
				(*resBoard).board[i][j].isFixed=1;
				fscanf(fp, "%c", curChar);
				}
			else{
				(*resBoard).board[i][j].isFixed=0;
			}
		}
	}
	fclose(fp);

	(*((*((*game).curBoard)).board))= *resBoard;
	game->curBoard->next=NULL;
	game->curBoard->prev = NULL;
	game->history->head = game->curBoard;
	sudokuBoardPrinter(game->curBoard->board);

}

void setXYZ(SudokuGame* game, int* a){
	if (game->gameMode==1){ //solve mode
		if (game->curBoard->board->board[a[0]-1][a[1]-1].isFixed==1){
			printf("Error: cell is fixed\n");
			return;
		}

		//TODO validate(game);
		SudokuBoard* newBoard=duplicateBoard(game->curBoard->board);


		newBoard->board[a[0]-1][a[1]-1].content=a[2]; //set board[x][y]=z
		Node* node=GetNewNode(newBoard);
		node->prev=game->curBoard;
		//TODO - change to free all the nodes after the current,
		//because we are starting a new path of action and all the undo that brought us here
		//are not relevant.
		free(game->curBoard->next);//free previousely done action
		game->curBoard->next=node;
		game->curBoard=node;
		sudokuBoardPrinter(game->curBoard->board); //print
		return;

		//if (validate(game)==0){}

		//TODO checkIfSolved(game);
	}
	else{ //edit mode
//		game->curBoard->board->board[a[0]][a[1]].content=a[2];
//		sudokuBoardPrinter(game->curBoard->board);

	}
	return;
}

int doesBoardHaveErrors(SudokuBoard* board){
	int n = board->n;
	int m = board->m;
	int N = n*m;
	for (int i = 0; i<N;i++){
		for (int j=0;j<N;j++){
			if (board->board[i][j].isError==1)
				return 1;}
	}
	return 0;

}

void saveBoardToFile(SudokuGame* game, char* fileToOpen){

	int a = doesBoardHaveErrors(game->curBoard->board);
	if (a==1 && game->gameMode==2){
		printf("Error: board contains erroneous values\n");
	}

	if (game->gameMode==2){
		//validateTheBoard and exit if not valid after printing not valid
		//Error: board validation failed\n
	}

	FILE * fp;
	fp = fopen(fileToOpen, "w");
	if (!fp){
		printf("Error: File cannot be created or modified\n");
		return;
	}
	char* str = (char*)calloc(1024, sizeof(char));
	str = sudokuBoardToString(game->curBoard->board);
	fwrite(str, sizeof(char), 1024, fp);
	fclose(fp);
	sudokuBoardPrinter(game->curBoard->board);


}

SudokuGame* initGameInInitMode(){
	SudokuGame* game = (SudokuGame*)calloc(1,sizeof(SudokuGame));
	game->gameMode=0; //init
	game->markErrors=0;
	game->history=(List*)calloc(1,sizeof(List));
	game->curBoard=GetNewNode(newEmptyBoard());

	return game;
}
void changeToEmptyGameInEditMode(SudokuGame* game){
	game->gameMode=2; //0-init 1-solve 2-edit
	InsertAtHead(game->history,newEmptyBoard());
	game->curBoard=game->history->head;
	sudokuBoardPrinter(game->curBoard->board);
}
