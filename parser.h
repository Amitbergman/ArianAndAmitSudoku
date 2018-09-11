/*
 * parser.h
 *
 *  Created on: 17 ×‘×�×•×’ 2018
 *      Author: arian
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "main.h"
/*parses the string for every command line input
 * and chooses the correct function to execute
 * returns 1 for exit and 0 to continue the game
1= solve X
2= edit [X]
3= mark_errors X
4= printboard
*/

int parseit(SudokuGame* game, char* str);

#endif /* PARSER_H_ */
