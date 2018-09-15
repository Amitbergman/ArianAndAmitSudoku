#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "printer.h"
#include "gameUtils.h"
#include "structs.h"
#include "ActionsHistory.h"
 /* This module is responsible for parsing the commands of the user
 * and deciding on the right function to execute
 */


/*Inner method
 * used to check if the input for mark errors has only 1's and 0's
 * as asked in the project description
 * the implementation consists of going in a loop over the characters and check if each one of them is 0 or 1.
*/
int checkValidityAndPrintForMarkErrors(char* toCheck){
	int len,cur;
	if (toCheck==NULL){
		printf("ERROR: invalid command\n");
		return 0;
	}
	len=strlen(toCheck);
	for (cur=0;cur<len;cur++){
		if (toCheck[cur]!='0'&&toCheck[cur]!='1'){
			printf("Error: the value should be 0 or 1\n");
			return 0;
		}
	}
	return 1;
}

/*Inner method
 * used to check if a string has only numbers
 * it is used in order to avoid cases in which the user inserts for exaple 1.5
 * and atoi understands it as 1.
 * the implementation consists of going through the string and check if everychar is a number (0-9).

*/
int hasNotNums(char* toCheck){
	int len,cur;

	len=strlen(toCheck);
	for (cur=0;cur<len;cur++){
		if(toCheck[cur]<48||toCheck[cur]>57){
			return 1;
		}
	}
	return 0;

}

/*this functions receives the string the user entered
 * and decided the correct function to execute
 *
 * returns 1 for exit and 0 to continue the game
 * The implementation is with comparing the string to the words of the commands as described in the project description.
 * after we compared the string we check if it is in the correct format as describes in the project description
 * if the format is correct, we execute the right function from the gameUtils.
 *
 *
*/
int parseit(SudokuGame* game, char* str){

	int N, i,check,y;
	int* a;
	char *token;
	const char s[] = " \t\r\n";
	if (str==NULL || strlen(str )>256){
		printf("ERROR: invalid command\n");
		return 0;

	}
	token = strtok(str, s);
	if (token==NULL){
		printf("ERROR: invalid command\n");
		return 0;
	}

	if(strlen(token)>256){
		printf("ERROR: invalid command\n");
		return 0;
	}

	if(strcmp(token,"solve")==0){
		token = strtok(NULL, s);
		if (token==NULL){
			printf("ERROR: invalid command\n");
			return 0;
		}
		/* it is load with parameter 1 because 1 represents solve mode in our program */
		loadBoardFromFile(game, token, 1);
		return 0;
	}
	if(strcmp(token,"edit")==0){
		token = strtok(NULL, s);
		if (token==NULL){/* edit with no file path */

			changeToEmptyGameInEditMode(game);
			sudokuBoardPrinter(game);
			return 0;
		}
		/* it is load with parameter 2 because 2 represents edit mode in our program */

		loadBoardFromFile(game, token, 2);

		return 0;
	}
	if(strcmp(token,"mark_errors")==0){
		if(game->gameMode!=1){
			printf("ERROR: invalid command\n");
			return 0;
		}
		token = strtok(NULL, s);

		/*	check if the second token of mark errors is only 0's and 1's	 */

		check = checkValidityAndPrintForMarkErrors(token);
		if (check==0){
			return 0;
		}
		else{
			if (atoi(token)>1||atoi(token)<0){
				printf("Error: the value should be 0 or 1\n");
				return 0;
			}
			game->markErrors=atoi(token);
		}
		return 0;
	}
	if(strcmp(token,"print_board")==0){
		if(game->gameMode==0){
			/* do not print in init mode  */

			printf("ERROR: invalid command\n");
		}
		else{

			/*

				prints the board as requested by user
			 */

			sudokuBoardPrinter(game);
		}
		return 0;
	}

	if(strcmp(token,"set")==0){
		if(game->gameMode==0){/* no  set in init mode */
			printf("ERROR: invalid command\n");
			return 0;
		}
		N = game->curBoard->board->m*game->curBoard->board->n;
		a=(int*)calloc(3,sizeof(int));
		if (!a){
			printf("Error: problem while allocating memory");
			exit(1);
		}
		check=0;
		/* checks validity of the parameters */
		for(i=0;i<3;i++){
			token = strtok(NULL, s);
			if (token==NULL){
				printf("ERROR: invalid command\n");
				free(a);
				return 0;
			}
			if (check==0){
				/* if it has not numbers, it is not valid for set */
				check=hasNotNums(token);
			}
			if (check==0){
				/* if atoi is 0 and the char is not 0 so atoi had a problem
				 * that is what written in the documentation of atoi online */
				if (atoi(token)>N||atoi(token)<0||(atoi(token)==0&&(i<2||token[0]!='0'))){
					check=1;
				}
				else{
					a[i]=atoi(token);
				}
			}
		}
		if (check==1){
			/* got 3 parameters but 1 of them is wrong */
			printf("Error: value not in range 0-%d\n",N);
			free(a);
			return 0;
		}

		setXYZ(game,a);

		free(a);
		return 0;
	}
	if(strcmp(token,"validate")==0){
		if(game->gameMode==0){/* no validate in init mode */
			printf("ERROR: invalid command\n");
		}
		else{
			validate(game->curBoard->board);
		}
		return 0;
	}
	if(strcmp(token,"undo")==0){
		if(game->gameMode==0){/* no undo in init mode */
			printf("ERROR: invalid command\n");
		}
		else{
			undo(game);
		}
		return 0;
	}
	if(strcmp(token,"redo")==0){
		if(game->gameMode==0){/* no redo in init mode */
			printf("ERROR: invalid command\n");
		}
		else{
			redo(game);
		}
		return 0;
	}
	if(strcmp(token,"reset")==0){
		if(game->gameMode==0){/* no reset in init mode */
			printf("ERROR: invalid command\n");
		}

		else{
			resetGame(game);
		}

		return 0;
	}
	if(strcmp(token,"exit")==0){
		/* exit the game */
		return 1;
	}

	if(strcmp(token,"save")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
			return 0;
		}
		token = strtok(NULL, s);
		if (token == NULL){
			/* save without an argument */
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
		N = game->curBoard->board->m*game->curBoard->board->n;
		a=(int*)calloc(2,sizeof(int));
		if (!a){
			printf("Error: problem while allocating memory");
			exit(1);
		}

		check=0;
		/* checks validity of the parameters */
		for(i=0;i<2;i++){
			token = strtok(NULL, s);
			if (token==NULL){
				printf("ERROR: invalid command\n");
				free(a);
				return 0;
			}
			if (check==0){
				check=hasNotNums(token);
			}
			if (check==0){
				if (atoi(token)>N||atoi(token)<1){
					check=1;
				}
				else{
					a[i]=atoi(token);
				}
			}
		}
		if (check==1){
			/* we got 2 params but one of them is error */
			printf("Error: value not in range 1-%d\n",N);
			free(a);
			return 0;
		}


		hintXY(game->curBoard->board,a[0],a[1]);

		free(a);
		return 0;
	}

	if (strcmp(token,"num_solutions")==0){
		if(game->gameMode==0){/*no num_solutions in init mode */
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
		N = game->curBoard->board->m*game->curBoard->board->n;
		a=(int*)calloc(2,sizeof(int));
		if (!a){
			printf("Error: problem while allocating memory");
			exit( 1);
		}

		check=0;
		for(i=0;i<2;i++){
			token = strtok(NULL, s);
			if (token==NULL){
				/* missing parameter*/
				printf("ERROR: invalid command\n");
				free(a);
				return 0;
			}
			if (check==0){
				/* ensures the parameter has only numbers */
				check=hasNotNums(token);
			}
			if (check==0){
				/* gets the integer value of the parameter */
				if (atoi(token)>N*N||atoi(token)<0){
					check=1;
				}
				else{
					a[i]=atoi(token);
				}
			}
		}
		if (check==1){
			/* we got 3 parameters but there is a problem in one of them at least */
			printf("Error: value not in range 0-%d\n",N*N);
			free(a);
			return 0;
		}


		if(boardIsEmpty(game->curBoard->board)==0){ /* do not generate if not empty*/
			printf("Error: board is not empty\n");
			free(a);
			return 0;
		}


		N = game->curBoard->board->m*game->curBoard->board->n;
		y = N*N-a[1];
		/* wants to leave only a[1] cells alive so we send N^2-a[1] cells to be deleted */
		if (a[1]>0){
			generateXY(game,a[0],y);
		}
		else{
			/* no change in the board since the user wants 0 cells */
			sudokuBoardPrinter(game);
		}
		free(a);
		return 0;
	}


	printf("ERROR: invalid command\n");
	return 0;

}

