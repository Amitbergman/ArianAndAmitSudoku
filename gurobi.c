/*
 * gurobi.c
 *
 *  Created on: 20 באוג 2018
 *      Author: arian
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "ActionsHistory.h"
#include "structs.h"

void freeAll(GRBenv *env, GRBmodel *model, double* sol, int* ind, double* val, double* lb, char* vtype, SudokuBoard* solBoard){
	GRBfreemodel(model);
	GRBfreeenv(env);

	free(sol);
	free(ind);
	free(val);
	free(lb);
	free(vtype);
	free(solBoard);
}
SudokuBoard* gurobi(SudokuGame* game, int m, int n)
{
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;
  int i,j,k,igrid, jgrid,count;
  int       error = 0;
  double*    sol;
  int*       ind;
  double*    val;
  double*   lb;
  char*      vtype;
  int       optimstatus;
  double    objval;

  int N=m*n;
  sol=(double*)calloc(N*N*N,sizeof(double));
  ind=(int*)calloc(N*N*N,sizeof(int));
  val=(double*)calloc(N*N*N,sizeof(double));
  lb=(double*)calloc(N*N*N,sizeof(double));
  vtype=(char*)calloc(N*N*N,sizeof(char));
  SudokuBoard* solBoard=duplicateBoard(game.curBoard.board);

  /* Create environment - log file is mip1.log */
  error = GRBloadenv(&env, "mip1.log");
  if (error) {
	  printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
	  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
	  return NULL;
  }

  /* Create an empty model named "mip1" */
  error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);

  if (error) {
	  printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
	  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
	  return NULL;
  }


  /* Add variables */

  /* coefficients - for x,y,z (cells 0,1,2 in "obj")
  obj[0] = 1; obj[1] = 1; obj[2] = 2;
  */

  for (i=0;i<N;i++){
	  for(j=0;j<N;j++){
		  for (k=0;k<N;k++){
			  if(game->curBoard->board->board[i][j]==k+1){
				  lb[i*N*N+j*N+k]=1;
			  }
			  else{
				  lb[i*N*N+j*N+k]=0;
			  }
			  vtype[i*N*N+j*N+k]=GRB_BINARY;
		  }
	  }
  }



  /* variable types - for x,y,z (cells 0,1,2 in "vtype")
  vtype[0] = GRB_BINARY; vtype[1] = GRB_BINARY; vtype[2] = GRB_BINARY;
  */
  /* add variables to model */
  error = GRBaddvars(model, N*N*N, 0, NULL, NULL, NULL, NULL, lb, NULL, vtype, NULL);
  if (error) {
	  printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
	  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
	  return NULL;
  }

  /* Change objective sense to maximization
   *

  error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
  if (error) {
	  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  /* update the model - to integrate new variables */
/*
  error = GRBupdatemodel(model);
  if (error) {
	  printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }
  */
  /* Each cell gets a value */

  for (i = 0; i < N; i++) {
	  for (j = 0; j < N; j++) {
		  for (k = 0; k < N; k++) {
			  ind[k] = i*N*N + j*N + k;
			  val[k] = 1.0;
		  }

		  error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
		  if (error) {
			  printf("ERROR %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
			  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
			  return NULL;
		  }
	  }
  }

  /* Each value must appear once in each row */

  for (k = 0; k < N; k++) {
	  for (j = 0; j < N; j++) {
		  for (i = 0; i < N; i++) {
			  ind[i] = i*N*N + j*N + k;
			  val[i] = 1.0;
		  }

		  error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
		  if (error) {
			  printf("ERROR %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
			  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
			  return NULL;
		  }
	  }
  }

  /* Each value must appear once in each column */

  for (k = 0; k < N; k++) {
	  for (i = 0; i < N; i++) {
		  for (j = 0; j < N; j++) {
			  ind[j] = i*N*N + j*N + k;
			  val[j] = 1.0;
		  }

		  error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
		  if (error) {
			  printf("ERROR %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
			  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
			  return NULL;
		  }
	  }
  }

  /* Each value must appear once in each subgrid */

  for (k = 0; k < N; k++) {
	  for (igrid = 0; igrid < m; igrid++) {
		  for (jgrid = 0; jgrid < n; jgrid++) {
			  count = 0;
			  for (i = igrid*m; i < (igrid+1)*m; i++) {
				  for (j = jgrid*n; j < (jgrid+1)*n; j++) {
					  ind[count] = i*N*N + j*N + k;
					  val[count] = 1.0;
					  count++;
				  }
			  }

			  error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			  if (error) {
				  printf("ERROR %d GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
				  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
				  return NULL;
			  }
		  }
	  }
  }


  /* Optimize model - need to call this before calculation */
  error = GRBoptimize(model);
  if (error) {
	  printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
	  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
	  return NULL;
  }

  /* Write model to 'mip1.lp' - this is not necessary but very helpful */
  error = GRBwrite(model, "mip1.lp");
  if (error) {
	  printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
	  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
	  return NULL;
  }

  /* Get solution information */

  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
  if (error) {
	  printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
	  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
	  return NULL;
  }

  /*
   get the objective -- the optimal result of the function
  error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
  if (error) {
	  printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }
  */

  /* get the solution - the assignment to each variable */
  /* 3-- number of variables, the size of "sol" should match */
  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N*N*N, sol);
  if (error) {
	  printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
	  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
	  return NULL;
  }

  /* print results */
  printf("\nOptimization complete\n");

  /* solution found */
  if (optimstatus == GRB_OPTIMAL) {
    printf("Optimal objective: %.4e\n", objval);
    //printf("  x=%.0f, y=%.0f, z=%.0f\n", sol[0], sol[1], sol[2]);
    for (i=0;i<N;i++){
    	for (j=0;j<N;j++){
    		for (k=0;k<N;k++){
    			if (sol[i*N*N+j*N+k]==1){
    				solBoard->board[i][j].content=k+1;
    			}
    		}
    	}
    }
  }
  /* no solution found */
  else if (optimstatus == GRB_INF_OR_UNBD) {
    printf("Model is infeasible or unbounded\n");
  }
  /* error or calculation stopped */
  else {
    printf("Optimization was stopped early\n");
  }

  freeAll(env, model, sol,ind, val, lb, vtype, solBoard);
  return solBoard;
}
