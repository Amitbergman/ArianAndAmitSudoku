/*

 * main.c
 *
 *  Created on: May 5, 2018
 *      Author: User
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "main.h"
#include "printer.h"
#include "parser.h"
#include "ActionsHistory.h"


//Creates a new Node and returns pointer to it.

int startGame(){
	int exit=0;
	char* str=(char*)calloc(1024,sizeof(char));
	SudokuGame* board = NULL;
	SudokuGame* game = (SudokuGame*)calloc(1,sizeof(SudokuGame));
	game->gameMode=0; //init
	game->markErrors=0;
	while (exit==0){
		printf("Enter your command:\n");
		fgets(str,1025,stdin);
		exit=parseit(game,str);
	}
	return 0;
}
int main ()
{
	int **a = NULL;
	// char* str = NULL;
	a = (int**)calloc(10, sizeof(int*));

	for (int i = 0; i<10;i++){
		a[i] = (int *)calloc(10, sizeof(int));
		for (int j = 0; j<10; j++){
			a[i][j]= i;
		}
	}
	//  SudokuBoard* board1pointer = createSudokuBoardFromArray(a,2,5);
	//  (*board1pointer).board[4][5].isFixed=1;
	//  str = sudokuBoardToString(board1pointer);
	//  printf("%s",str);

	startGame();
	return 0;
}







