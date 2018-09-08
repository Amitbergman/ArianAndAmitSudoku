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
#include <time.h>
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
		while(input[0]==' '){
			input++;
		}
		if (strcmp(input, "\n")!=0){
			exit=parseit(game,input);
		}
	}
	freeGame(game);
	free(input);

	return 0;
}
int main ()
{	SP_BUFF_SET();
	srand(time(NULL));
	printf("Sudoku\n------\n");
	startGame();
	return 0;
}

