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
#include "structs.h"
#include "printer.h"
#include "parser.h"
#include "ActionsHistory.h"
#include "SPBufferset.h"
#include "gameUtils.h"


/* Creates a new Node and returns pointer to it. */

int startGame(){
	int exit;
	char* input;
	SudokuGame* game;

	exit=0;
	input=(char*)calloc(1024,sizeof(char));
	game = initGameInInitMode();

	while (exit==0){
		printf("Enter your command:\n");
		fgets(input,1024,stdin);
		exit=parseit(game,input);
	}
	/* TODO - free all the list of boards
	 * TODO - free the game
	 */
	return 0;
}
int main ()
{	SP_BUFF_SET();
	printf("Sudoku\n------\n");
	startGame();
	return 0;
}

