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
//#include "game.h"


int parseit(SudokuGame* game, char* str){
	/*parses the string for every command line input
	 * returns 1 for exit and 0 else
	1= solve X
	2= edit [X]
	3= mark_errors X
	4= printboard


	 */
	const char s[] = " \t\r\n";
	char *token;
	token = strtok(str, s);
	//int N=(game->board->m)*(game->board->n);
	int N=(game->curBoard->board->m)*(game->curBoard->board->n);

	if(strcmp(token,"solve")==0){
		token = strtok(NULL, s);
		if (token==NULL){
			printf("Error: file doesn't exist or cannot be opened\n");
			return 0;
		}
		loadBoardFromFile(game, token, 1);
		return 0;
	}
	else if(strcmp(token,"edit")==0){
		token = strtok(NULL, s);
		if (token==NULL){
			game->gameMode=2; //0-init 1-solve 2-edit
//			game->board=newEmptyBoard();
//			game->curBoard=GetNewNode(newEmptyBoard());

			InsertAtHead(game->history,newEmptyBoard());
			game->curBoard=game->history->head;
			sudokuBoardPrinter(game->curBoard->board);

			return 0;
		}
		loadBoardFromFile(game, token, 2);
		sudokuBoardPrinter(game->curBoard->board);
		return 0;
	}
	else if(strcmp(token,"mark_errors")==0){
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
	else if(strcmp(token,"print_board")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
		}
		else{
			sudokuBoardPrinter(game->curBoard->board);
		}
		return 0;
	}

	else if(strcmp(token,"set")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
			return 0;
		}
		int* a=(int*)calloc(3,sizeof(int));
		int i=0;
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
	else if(strcmp(token,"undo")==0){
		undo(game);
		return 0;
	}
	else if(strcmp(token,"redo")==0){
		redo(game);
		return 0;
	}
	else if(strcmp(token,"reset")==0){
			resetGame(game);
			return 0;
		}
	else if(strcmp(token,"exit")==0){
		printf("Exiting...\n");
		return 1;
	}
	else {
		if(strcmp(token,"save")==0){
			token = strtok(NULL, s);
			saveBoardToFile(game, token );

		}
		else{
		printf("ERROR: invalid command\n");
		}
		return 0;
	}
}

