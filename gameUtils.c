/*
 * gameUtils.c
 *
 *  Created on: Aug 17, 2018
 *      Author: User
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "main.h"
#include "printer.h"

void loadBoardFromFile(SudokuGame* game, char* fileToOpen, int mode){
	SudokuBoard* resBoard = newEmptyBoard();
	(*game).gameMode = mode;
	(*game).markErrors = 0;
	FILE * fp;

	fp = fopen (fileToOpen, "r");
	assert (fp);


	//Now the Buffer Has all the data from the file
	int n =0;
	int m =0 ;
	fscanf(fp, "%d %d\n", &m,  &n);
	int N = n*m;
	(*resBoard).m=m;
	(*resBoard).n=n;

	char* curChar = (char *)calloc(1,sizeof(char));
	int curCellContent=0;
	for (int i=0;i<N;i++){
		for (int j=0;j<N;j++){
			fscanf(fp, "%d", &curCellContent);
			(*resBoard).board[i][j].content=curCellContent;
			(*resBoard).board[i][j].isError=0;
			fscanf(fp, "%c", curChar);
			if (strcmp(curChar, ".")==0){
				(*resBoard).board[i][j].isFixed=1;
				fscanf(fp, "%c", curChar);
				}
			else{
				(*resBoard).board[i][j].isFixed=0;
			}
		}
	}
	sudokuBoardPrinter(resBoard);


}
