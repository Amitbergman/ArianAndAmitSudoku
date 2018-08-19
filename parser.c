/*
 * parser.c
 *
 *  Created on: 16 ×‘×�×•×’ 2018
 *      Author: arian
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "main.h"
#include "printer.h"
#include "gameUtils.h"
#include "structs.h"
#include "ActionsHistory.h"


int parseit(SudokuGame* game, char* str){
	/*parses the string for every command line input
	 * returns 1 for exit and 0 to continue the game
	1= solve X
	2= edit [X]
	3= mark_errors X
	4= printboard
	 */

	int N;
	int i;
	int* a;
	char *token;
	const char s[] = " \t\r\n";
	token = strtok(str, s);
	N=(game->curBoard->board->m)*(game->curBoard->board->n);

	if(strcmp(token,"solve")==0){
		token = strtok(NULL, s);
		if (token==NULL){
			printf("Error: file doesn't exist or cannot be opened\n");
			return 0;
		}
		/* TODO - free memory of previous game
		 *
		 */
		loadBoardFromFile(game, token, 1);
		return 0;
	}
	if(strcmp(token,"edit")==0){
		token = strtok(NULL, s);
		if (token==NULL){/* edit with no file path */
			/* TODO - free memory of previous game
			 *
			 */
			changeToEmptyGameInEditMode(game);

			return 0;
		}
		/* TODO - free memory of previous game !!!if the loading succeeded!!!
		 *
		 */
		loadBoardFromFile(game, token, 2);

		return 0;
	}
	if(strcmp(token,"mark_errors")==0){
		if(game->gameMode!=1){
			printf("ERROR: invalid command\n");
			return 0;
		}
		token = strtok(NULL, s);

		if ((token==NULL)||((atoi(token)==0)&&(strcmp(token,"0")!=0))||(atoi(token)<0)||(atoi(token)>1)){
			printf("Error: the value should be 0 or 1\n");
		}
		else{
			game->markErrors=atoi(token);
		}
		return 0;
	}
	if(strcmp(token,"print_board")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
		}
		else{
			sudokuBoardPrinter(game->curBoard->board);
		}
		return 0;
	}

	if(strcmp(token,"set")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
			return 0;
		}
		a=(int*)calloc(3,sizeof(int));
		if (!a){
			printf("Error: problem while allocating memory");
			return 1;
		}
		i=0;
		for(;i<3;i++){
			token = strtok(NULL, s);
			if ((token==NULL)||((atoi(token)==0)&&(strcmp(token,"0")!=0))||(atoi(token)<0)||(atoi(token)>N)||((i<2)&&(atoi(token)==0))){
				printf("Error: value not in range 0-%d\n",N);
				free(a);
				return 0;
			}
			a[i]=atoi(token);
		}
		setXYZ(game,a);

		free(a);
		return 0;
	}
	if(strcmp(token,"undo")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
		}
		else{
			undo(game);
		}
		return 0;
	}
	if(strcmp(token,"redo")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
		}
		else{
			redo(game);
		}
		return 0;
	}
	if(strcmp(token,"reset")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
		}
		else{
			resetGame(game);
		}
		return 0;
	}
	if(strcmp(token,"exit")==0){

		printf("Exiting...\n");
		return 1;
	}
	if(strcmp(token,"save")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
		}
		else{
			token = strtok(NULL, s);
			saveBoardToFile(game, token);
		}
		return 0;
	}

	if (strcmp(token,"num_solutions")==0){
		printf("num_solutions");
		return 0;
	}

	if (strcmp(token,"autofill")==0){
		printf("autofill");
		return 0;
	}

	if (strcmp(token,"hint")==0){
		printf("hint");
		return 0;
	}
	if(strcmp(token,"validate")==0){
		printf("validate");
		return 0;
	}
	if(strcmp(token,"generate")==0){
		printf("generate");
		return 0;
	}
	printf("ERROR: invalid command\n");
	return 0;

}



