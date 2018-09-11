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


/*

*starts the game
*and waits for commands from the user
*when this function ends, the game is ended

 */

int startGame(){
	int exitTheGame;
	char* input;
	SudokuGame* game;

	exitTheGame=0;
	input=(char*)calloc(1024,sizeof(char));
	if (!input){
		printf(" Problem in memory allocating");
		exit(1);
	}
	game = initGameInInitMode();
	while ((exitTheGame==0)&&(!feof(stdin))){
		printf("Enter your command:\n");
		fgets(input,1024,stdin);

		while(input[0]==' '){
			input++;
		}
		if (strcmp(input, "\n")!=0){
			exitTheGame=parseit(game,input);
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
	printf("Exiting...\n");
	return 0;
}

