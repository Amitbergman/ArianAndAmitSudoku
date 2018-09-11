#ifndef GAMEUTILS_H_
#define GAMEUTILS_H_

/*recieves a file name and mode and loads the gameboard from the filename to the current game */
void loadBoardFromFile(SudokuGame* game, char* fileToOpen, int mode);

/*sets the x y cell to be z
 * when a[0]=x
 * a[1]=y
 * a[2]=z
 *  */
void setXYZ(SudokuGame* game, int* a);

/*validated the board
 * and prints the result og the validation */
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

/* return 0 if value is not legal and 1 if is legal in the current board */
int isLegalValue(SudokuBoard * board, int col, int row, int valueToCheck);

/* return 0 if value is not legal inside box and 1 if is legal */
int checkValidInBox(SudokuBoard* board, int col, int row, int n, int m, int valueToCheck);

/* executes the autofill command on the game board */
void autofill(SudokuGame* game);

/* returns 1 iff board has errors */
int boardHasErrors(SudokuBoard* board);

/* goes through all board and updates cells that are errors */
void updateErrorsInBoard(SudokuBoard* board);

/* returns 1 iff board is full with values */
int boardIsFull(SudokuBoard* board);

/* returns 1 iff board is empty */
int boardIsEmpty(SudokuBoard* board);

/* executes the generate command as written in the project description */
int generateXY(SudokuGame* game,int x, int y);

/* gets a full board, checks if it is successfully solved or not
 * and acts like we were told in the project description */
void dealWithFullBoard(SudokuGame* game);

/* returns the number of numbers that fit to the x,y cell */
int getPlausibleNums(SudokuBoard* board,int x, int y, int* pNums);

/* executes the num_solutions command from the project menu
 * prints the number of solutions */
int num_solutions(SudokuBoard* board);

/* gets a board and counts the number of possible solutions */
int countNumberOfSolutions(SudokuBoard* board);

/*given array and index, shifts left all values right to index.
	 *  returns new array size defacto. if returns 0 than array is empty*/
int manageArray(int* arr, int ind);

/*returns a random index from the list
 * in range [1,size] ; size=Arr[0].
 * (based on optional indexes)
 * for the random board generator.
 *  */
int getRandIndex(int* Arr);

void clearYCells(SudokuBoard* board, int y, int N);

/* returns a new stackNode */
stackNode* getNewStackNode(int col, int row, int numToCheck);

/* returns a new empty stack */
stack* createNewEmptyStack();

/* pushes the node to the stack */
void push(stack* stack, stackNode* nodeToPush);

/* pops from the stack and returns the popped node */
stackNode pop(stack* stack);


/* peek in the head of the stack*/
stackNode* peek(stack* stack);

/* frees the stack  and its content after we finish using it*/
void freeStack(stack* stack);

/* increases the number to check in the head of the stacy by 1*/
void increaseHeadOfStackByOne(stack* stacker);

/* creates a new sudokecell*/
SudokuCell* createNewCell(int content, int isFixed, int isError);

/* creates and returns a new empty sudoku board with n and m as parameters*/
SudokuBoard* newEmptyBoard(int n, int m);

/* creates a new sudokuboard from the array of ints specified*/
SudokuBoard* createSudokuBoardFromArray(int ** array, int n, int m);

#endif /* GAMEUTILS_H_ */
