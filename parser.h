#ifndef PARSER_H_
#define PARSER_H_

/*parses the string for every command line input
 * and chooses the correct function to execute
 * returns 1 for exit and 0 to continue the game
*/
int parseit(SudokuGame* game, char* str);

#endif /* PARSER_H_ */
