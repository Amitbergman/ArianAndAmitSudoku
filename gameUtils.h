#ifndef GAMEUTILS_H_
#define GAMEUTILS_H_
 /* This module is responsible for functions that are related to the flow of the game
 *
 */

/*recieves a file name and mode and loads the gameboard from the filename to the current game */
void loadBoardFromFile(SudokuGame* game, char* fileToOpen, int mode);

/*sets the x y cell to be z
 * when a[0]=x
 * a[1]=y
 * a[2]=z
 *  */
void setXYZ(SudokuGame* game, int* a);

/*validates the board
 * and prints the result of the validation */
void validate(SudokuBoard* board);

/*give hint to the user */
void hintXY(SudokuBoard* board, int x, int y);

/*save the current board to the file specified in the variable fileToOpen */
void saveBoardToFile(SudokuGame* game, char* fileToOpen);

/*initializes a game in init mode */
SudokuGame* initGameInInitMode();

/*free all resources used by game */
void freeGame(SudokuGame* game);

/*changes the status to empty game in edit mode
 * used when user writed "edit" */
void changeToEmptyGameInEditMode(SudokuGame*);

/* executes the autofill command on the game board */
void autofill(SudokuGame* game);

/* returns 1 iff board is empty */
int boardIsEmpty(SudokuBoard* board);

/* executes the generate command as written in the project description */
int generateXY(SudokuGame* game,int x, int y);

/* executes the num_solutions command from the project menu
 * prints the number of solutions */
int num_solutions(SudokuBoard* board);


/* creates and returns a new sudokecell*/
SudokuCell* createNewCell(int content, int isFixed, int isError);


/*recieves a board and frees all the resources it uses
 */
void freeBoard(SudokuBoard* board);
#endif /* GAMEUTILS_H_ */
