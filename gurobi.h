/*
 * gurobi.h
 *
 *  Created on: 22 באוג 2018
 *      Author: arian
 */

#ifndef GUROBI_H_
#define GUROBI_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gameUtils.h"
#include "ActionsHistory.h"
#include "structs.h"


SudokuBoard* gurobi(SudokuBoard* board);


#endif /* GUROBI_H_ */
