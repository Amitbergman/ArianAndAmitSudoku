/*
 * parser.c
 *
 *  Created on: 16 ×‘×�×•×’ 2018
 *      Author: arian
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	/*SudokuBoard* test;*/
	const char s[] = " \t\r\n";
	token = strtok(str, s);
	N=(game->curBoard->board->m)*(game->curBoard->board->n);


	/*SudokuBoard* test;
	 * Amit: I am not sure about it, isnt it say  that the first
	 *  word of the input is in size 256?
	 *  I think they told us that if all the line is longer than 256
	 *   then we can ignore the input
	 */

	if(strlen(token)>256){
		printf("ERROR: invalid command\n");
		return 0;
	}
	if(token==NULL){
		return 0;
	}

	if(strcmp(token,"solve")==0){
		token = strtok(NULL, s);
		if (token==NULL){
			printf("Error: file doesn't exist or cannot be opened\n");
			return 0;
		}
		loadBoardFromFile(game, token, 1);
		return 0;
	}
	if(strcmp(token,"edit")==0){
		token = strtok(NULL, s);
		if (token==NULL){/* edit with no file path */

			changeToEmptyGameInEditMode(game);
			return 0;
		}
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
			sudokuBoardPrinter(game);
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
	if(strcmp(token,"validate")==0){
		if(game->gameMode==0){
					printf("ERROR: invalid command\n");
				}
		validate(game->curBoard->board);
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
	/*
	if(strcmp(token,"testSolve")==0){

		test= gurobi(game->curBoard->board);
		if (test!=NULL){
			printf("test!=null");
			setBoard(game,test);
		}

		*/
		/*game->curBoard->board=test;*/
		/*
		return 0;
	}
	*/

	if(strcmp(token,"save")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
			return 0;
		}
		token = strtok(NULL, s);
		if (token == NULL){
			printf("ERROR: invalid command\n");
			return 0;
		}
		saveBoardToFile(game, token);

		return 0;
	}
	if(strcmp(token,"hint")==0){
		if(game->gameMode!=1){ /* only in solve mode */
			printf("ERROR: invalid command\n");
			return 0;
		}
		a=(int*)calloc(2,sizeof(int));
		if (!a){
			printf("Error: problem while allocating memory");
			return 1;
		}

		for(i=0;i<2;i++){
			token = strtok(NULL, s);
			if ((token==NULL)||(atoi(token)<1)||(atoi(token)>N)){
				printf("Error: value not in range 0-%d\n",N);
				free(a);
				return 0;
			}
			a[i]=atoi(token);
		}
		hintXY(game->curBoard->board,a[0],a[1]);

		free(a);
		return 0;

	}

	if (strcmp(token,"num_solutions")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
			return 0;
		}
		num_solutions(game->curBoard->board);
		return 0;
	}

	if (strcmp(token,"autofill")==0){
		if (game->gameMode!=1)	/*game mode is not solve */
		{
			printf("ERROR: invalid command\n");
			return 0;
		}
		autofill(game);
		return 0;
	}

	if(strcmp(token,"generate")==0){
		if(game->gameMode!=2){ /* only in edit mode */
			printf("ERROR: invalid command\n");
			return 0;
		}
		if(boardIsEmpty(game->curBoard->board)==0){ /* if board is not empty*/
			printf("ERROR: board is not empty\n");
			return 0;
		}
		a=(int*)calloc(2,sizeof(int));
		if (!a){
			printf("Error: problem while allocating memory");
			return 1;
		}

		for(i=0;i<2;i++){
			token = strtok(NULL, s);
			if ((token==NULL)||(atoi(token)<1)||(atoi(token)>N*N)){
				printf("Error: value not in range 0-%d\n",N*N);
				free(a);
				return 0;
			}
			a[i]=atoi(token);
		}
		generateXY(game,a[0],a[1]);

		free(a);
		return 0;
	}
	printf("ERROR: invalid command\n");
	return 0;

}



