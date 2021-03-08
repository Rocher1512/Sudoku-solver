#include <iostream>
#include <vector>
#include <fstream>
#include "SudokuPuzzle.h"
using namespace std;

int main()
{
	char filename[] = "sudoku_puzzle.txt";
	SudokuPuzzle puzzle;
	puzzle.solve(filename);
}
