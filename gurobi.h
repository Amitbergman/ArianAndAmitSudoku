#ifndef GUROBI_H_
#define GUROBI_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gameUtils.h"
#include "ActionsHistory.h"
#include "structs.h"

/* Module Description
*this module is respobsible for solving sudoku boards with ILP as requested in project description
*we use the gurobi tool to create a model and make constraints of ILP
*we try to solve the board received, and return the solved board on success and NULL on failure
*
*/



/* this function gets a board and solves it with ILP
 * as described in the project description
 * if the board is not solvable it returns NULL */
SudokuBoard* solveSudoku(SudokuBoard* board);


#endif /* GUROBI_H_ */
