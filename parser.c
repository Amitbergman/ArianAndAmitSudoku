#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "printer.h"
#include "gameUtils.h"
#include "structs.h"
#include "ActionsHistory.h"

/*Inner method
 * used to check if the input for mark errors has only 1's and 0's
 * as asked in the project description
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

/*parses the string for every command line input
 * and chooses the correct function to execute
 * returns 1 for exit and 0 to continue the game
*/
int parseit(SudokuGame* game, char* str){

	int N, i,check;
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
	if(token==NULL){
		return 0;
	}

	if(strcmp(token,"solve")==0){
		token = strtok(NULL, s);
		if (token==NULL){
			printf("ERROR: invalid command\n");
			return 0;
		}
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
		loadBoardFromFile(game, token, 2);

		return 0;
	}
	if(strcmp(token,"mark_errors")==0){
		if(game->gameMode!=1){
			printf("ERROR: invalid command\n");
			return 0;
		}
		token = strtok(NULL, s);

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
		N = game->curBoard->board->m*game->curBoard->board->n;
		a=(int*)calloc(3,sizeof(int));
		if (!a){
			printf("Error: problem while allocating memory");
			exit(1);
		}
		check=0;
		for(i=0;i<3;i++){
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
				if (atoi(token)>N||atoi(token)<0||(atoi(token)==0&&(i<2||token[0]!='0'))){
					check=1;
				}
				else{
					a[i]=atoi(token);
				}
			}
		}
		if (check==1){
			printf("Error: value not in range 0-%d\n",N);
			free(a);
			return 0;
		}

		setXYZ(game,a);

		free(a);
		return 0;
	}
	if(strcmp(token,"validate")==0){
		if(game->gameMode==0){
			printf("ERROR: invalid command\n");
		}
		else{
			validate(game->curBoard->board);
		}
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
		return 1;
	}

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
		N = game->curBoard->board->m*game->curBoard->board->n;
		a=(int*)calloc(2,sizeof(int));
		if (!a){
			printf("Error: problem while allocating memory");
			free(a);
			exit(1);
		}

		check=0;
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
			printf("Error: value not in range 1-%d\n",N);
			free(a);
			return 0;
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
				printf("ERROR: invalid command\n");
				free(a);
				return 0;
			}
			if (check==0){
				check=hasNotNums(token);
			}
			if (check==0){
				if (atoi(token)>N*N||atoi(token)<0){
					check=1;
				}
				else{
					a[i]=atoi(token);
				}
			}
		}
		if (check==1){
			printf("Error: value not in range 0-%d\n",N*N);
			free(a);
			return 0;
		}


		if(boardIsEmpty(game->curBoard->board)==0){ /* if board is not empty*/
			printf("Error: board is not empty\n");
			free(a);
			return 0;
		}

		generateXY(game,a[0],a[1]);

		free(a);
		return 0;
	}


	printf("ERROR: invalid command\n");
	return 0;

}

