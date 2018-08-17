/*
 * gameUtils.h
 *
 *  Created on: Aug 17, 2018
 *      Author: User
 */

#ifndef GAMEUTILS_H_
#define GAMEUTILS_H_

void loadBoardFromFile(SudokuGame* game, char* fileToOpen, int mode);
void saveBoardToFile(SudokuGame* game, char* fileToOpen);

#endif /* GAMEUTILS_H_ */
