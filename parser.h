#ifndef PARSER_H_
#define PARSER_H_
 /* This module is responsible for parsing the commands of the user
 *
 */


/*parses the string for every command line input
 * and chooses the correct function to execute
 *
*/
int parseit(SudokuGame* game, char* str);

#endif /* PARSER_H_ */
