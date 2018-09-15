#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "ActionsHistory.h"
#include "structs.h"
#include "gameUtils.h"



/* Module Description
*this module is respobsible for solving sudoku boards with ILP as requested in project description
*we use the gurobi tool to create a model and make constraints of ILP
*we try to solve the board received, and return the solved board on success and NULL on failure
*
*/





/*Inner method
 * used to free all the resources used by the gurobisolver
 * if the tofreeBoard parameter is 1, it means there was no solutions so we should also free the board
 * otherwise, we free all the resources but the board
*/

void freeAll(GRBenv *env, GRBmodel *model, double* sol, int* indX, double* valX, double* lowerBound, char* vtype, SudokuBoard* board, int toFreeBoard){
	GRBfreemodel(model);
	GRBfreeenv(env);

	free(sol);
	free(indX);
	free(valX);
	free(lowerBound);
	free(vtype);
	if (toFreeBoard){
		freeBoard(board);
	}
}

/*this function gets a board and solves it with ILP with gurobi.
 * It creates the model and the constraints of the sudoku as talked about in the class and project description.
 * if a solution was found, the functions returns it, otherwise, NULL.
*/


void onceInAabox(int n, int m, int N,double* valX,int* indX, GRBmodel *model){
	int iInBox, jInBox, count, k, i, j,error;
	for(iInBox=0;iInBox<N;iInBox+=n){
		for(jInBox=0;jInBox<N;jInBox+=m){
			for(k=0;k<N;k++){
				count=0;
				for(i=iInBox;i<iInBox+n;i++){
					for(j=jInBox;j<jInBox+m;j++){
						indX[count] = i*N*N + j*N + k;
						valX[count] = 1.0;
						count++;
					}
				}
				error = GRBaddconstr(model, N, indX, valX, GRB_EQUAL, 1.0, NULL);
				if (error) {
					printf("Failure during gurobi execution");
					exit(1);
				}
			}
		}
	}
}

void onceInAColumn(int N,int* indX,double* valX,GRBmodel *model ){
	int k,i,j,error;
	for (k = 0; k < N; k++) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				indX[j] = i*N*N + j*N + k;
				valX[j] = 1.0;
			}

			error = GRBaddconstr(model, N, indX, valX, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("Failure during gurobi execution");
				exit(1);
			}
		}
	}
}

void onceInARow (int N,int* indX,double* valX,GRBmodel *model){
	int k,j,i,error;
	for (k = 0; k < N; k++) {
		for (j = 0; j < N; j++) {
			for (i = 0; i < N; i++) {
				indX[i] = i*N*N + j*N + k;
				valX[i] = 1.0;
			}

			error = GRBaddconstr(model, N, indX, valX, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("Failure during gurobi execution");
				exit(1);
			}
		}
	}
}
/*
 * adds constraints such that every cell has only value between 1 to N
 */
void everyCell1toN(int N,int* indX,double* valX,GRBmodel *model){
	int i,j,k,error;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				indX[k] = i*N*N + j*N + k;
				valX[k] = 1.0;
			}

			error = GRBaddconstr(model, N, indX, valX, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("Failure during gurobi execution");
				exit(1);
			}
		}
	}
}

SudokuBoard* solveSudoku(SudokuBoard* board)
{	int errorInSolutions=0;
	GRBenv   *env   = NULL;
	GRBmodel *model = NULL;
	int i,j,k,m,n,N;
	int       error = 0;
	double*    sol;
	int*       indX;
	double*    valX;
	double*   lowerBound;
	char*      vtype;
	int       optimstatus;


	SudokuBoard* solBoard;
	m=board->m;
	n=board->n;
	N=m*n;
	sol=(double*)calloc(N*N*N,sizeof(double));
	indX=(int*)calloc(N*N*N,sizeof(int));
	valX=(double*)calloc(N*N*N,sizeof(double));
	lowerBound=(double*)calloc(N*N*N,sizeof(double));
	vtype=(char*)calloc(N*N*N,sizeof(char));
	solBoard=duplicateBoard(board);


	/* Create environment - log file is SudokuLog.log */
	error = GRBloadenv(&env, "SudokuLog.log");
	if (error) {

		printf("Failure during gurobi execution");
		exit(1);
	}

	/* Cancel printing */
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {

		printf("Failure during gurobi execution");
		exit(1);
	}

	/* Create an empty model named "SudokuProject" */
	error = GRBnewmodel(env, &model, "SudokuProject", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) {
		printf("Failure during gurobi execution");
		exit(1);
	}


	/* Add variables to out newly created model */
	for (i=0;i<N;i++){
		for(j=0;j<N;j++){
			for (k=0;k<N;k++){
				if(board->board[i][j].content==k+1){
					lowerBound[i*N*N+j*N+k]=1;
				}
				else{
					lowerBound[i*N*N+j*N+k]=0;
				}
				vtype[i*N*N+j*N+k]=GRB_BINARY;
			}
		}
	}

	/* check for errors in creating the variablles
	 * if an error occurred we still free the resources though we did not asked to
	 * we decided to do it for better habit
	  */
	error = GRBaddvars(model, N*N*N, 0, NULL, NULL, NULL, NULL, lowerBound, NULL, vtype, NULL);
	if (error) {
		printf("Failure during gurobi execution");
		exit(1);
	}

	/* Change objective sense to maximization
	 * It does not mind because we only want a solution, if its feasible its good */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {

		printf("Failure during gurobi execution");
		exit(1);
	}

	/* update the model - to integrate new variables */
	error = GRBupdatemodel(model);
	if (error) {
		printf("Failure during gurobi execution");
		exit(1);
	}

	/* Add constraints: each cell must have a value in range [1,N]*/
	everyCell1toN(N, indX, valX, model);

	/* Add constraints: each value must appear exactly once in each row */
	onceInARow(N, indX, valX,  model);

	/* Add constraints: each value must appear exactly once in each column */
	onceInAColumn(N, indX, valX, model);

	/* Add constraints: each value must appear exactly once in each box*/

	onceInAabox( n, m, N, valX, indX, model);

	/* Optimize model
	 * here we are taking all the constaraints and try to get a solved board */
	error = GRBoptimize(model);
	if (error) {
		printf("Failure during gurobi execution");
		exit(1);
	}


	/* Get solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		errorInSolutions=1;
	}


	/* get value 0 or 1 to each variable Xijk, Xijk==1 iff board[i][j]==k+1 */
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N*N*N, sol);
	if (error){
		errorInSolutions =1;
	}

	/* solution found */
	if (errorInSolutions==0 && optimstatus == GRB_OPTIMAL) {
		for (i=0;i<N;i++){
			for (j=0;j<N;j++){
				for (k=0;k<N;k++){
					if (sol[i*N*N+j*N+k]==((double)1)){
						solBoard->board[i][j].content=k+1;
					}
				}
			}
		}
	}

	/* no solution found
	 * problem is infeaasible or unbounded */
	else if (optimstatus ==GRB_INFEASIBLE|| optimstatus == GRB_INF_OR_UNBD || optimstatus == GRB_UNBOUNDED ) {
		freeAll(env, model, sol,indX, valX, lowerBound, vtype, solBoard, 1);
		return NULL;

	}
	/* error or calculation stopped */
	else {
		printf("Failure during gurobi execution");
		exit(1);
	}

	freeAll(env, model, sol,indX, valX, lowerBound, vtype, NULL, 0);
	return solBoard;
}
