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
#include "gurobi_c.h"
#include "gameUtils.h"
#include "ActionsHistory.h"
#include "structs.h"


void freeAll(GRBenv *env, GRBmodel *model, double* sol, int* ind, double* val, double* lb, char* vtype);
SudokuBoard* gurobi(SudokuBoard* board);


#endif /* GUROBI_H_ */
