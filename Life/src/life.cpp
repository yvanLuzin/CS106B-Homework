// This is the CPP file you will edit and turn in.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;

/* Global vars */
bool isWrappingEnabled;

/* Function Prototypes */
void displayGrid(Grid<string> & grid);
Grid<string> createGridFromFile(string file);
bool promptForWrapping();
int increaseNeighborsCount(int row, int col, int cellRow, int cellCol, Grid<string> & grid);
int checkNumberOfNeighbors(int row, int col, Grid<string> & grid);
void simulate(Grid<string> & grid);

/* Main Program */
int main() {
    cout << "Welcome to the CS 106B Game Of Life,\n"
            "a simulation of the lifecycle of a bacteria colony.\n"
            "Cells (X) live and die by the following rules:\n"
            "- A cell with 1 or fewer neighbors dies.\n"
            "- Location with 2 neighbors remain stable.\n"
            "- Location with 3 neighbors create life.\n"
            "- A cell with 4 or more neighbors dies.\n"
         << endl;

    Grid<string> grid = createGridFromFile(promptUserForFile("Input filename: "));
    isWrappingEnabled = promptForWrapping();

    displayGrid(grid);

    string userInput;
    while (toLowerCase(userInput) != "q") {
        getLine("a)nimate, t)ick, q)uit? ", userInput);

        if (toLowerCase(userInput) == "a" ) {
            int numberOfFrames = getInteger("How many frames?", "Illegal integer format. Try again.");
            for (int i = 0; i < numberOfFrames; i++) {
                clearConsole();
                simulate(grid);
                pause(50);
            }
        } else if (toLowerCase(userInput) == "t" ) {
            simulate(grid);
        } else if (toLowerCase(userInput) == "q" ) {
            cout << "Have a nice Life!" << endl;
        }
    }

    return 0;
}

/* Function: createGridFromFile;
 * Usage: createGridFromFile(fileName);
 * -----------------------------------
 * Open filestream from filename and
 * creates grid from it's contents.
 */
Grid<string> createGridFromFile(string fileName) {
    ifstream file;
    file.open(fileName);

    string numberOfRows;
    string numberOfCols;
    getline(file, numberOfRows);
    getline(file, numberOfCols);

    Grid<string> grid(stringToInteger(numberOfRows), stringToInteger(numberOfCols));

    for (int r = 0; r < grid.numRows(); r++) {
        string line;
        getline(file, line);

        for (int ch = 0; ch < line.size(); ch++) {
            grid[r][ch] = line[ch];
        }
    }

    file.close();
    return grid;
}

/* Function: displayGrid;
 * Usage: displayGrid(grid);
 * ------------------------
 * Takes reference to grid and
 * prints it out in console.
 */
void displayGrid(Grid<string> & grid) {
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            cout << grid[r][c];
        }
        cout << endl;
    }
}

/* Function: promptForWrapping;
 * Usage: promptForWrapping;
 * ------------------------
 * Prompts user input and returns
 * true for 'y' and false for 'n'
 */
bool promptForWrapping() {
    while (true) {
        string input;
        getLine("Should the simulation wrap around the grid (y/n?) ", input);

        if (toLowerCase(input) == "y") {
            return true;
        } else if (toLowerCase(input) == "n") {
            return false;
        }
    }
}

/* Function: applyCellRules;
 * Usage: applyCellRules(cell, numOfNeighbors);
 * -------------------------------------------
 * Modify cell content according to game of life rules:
 *  - Cell dies if 1 or less neighbors
 *  - Cell stable is 2 neighbors
 *  - Cell creates life if 3 neighbors
 *  - Cell dies of 4 or more neighbors.
 */
string applyCellRules(string cell, int numOfNeighbors) {
    if (numOfNeighbors <=1 ) {
        return "-";
    } else if (numOfNeighbors == 2) {
        return cell;
    } else if (numOfNeighbors == 3) {
        return "X";
    } else {
        return "-";
    }
}

/*
 * Function: wrapCell;
 * Usage: wrapCell(cell, gridDimension);
 * ---------------------------------------------
 * Accepts cell position and grid's weight or height.
 * Returns wrapper position.
 */
int wrapCell(int cell, int gridDimension) {
    if (cell > 0) {
        return cell % gridDimension;
    } else {
        return (cell + gridDimension ) % gridDimension;
    }
}

/*
 * Function: increaseNeighborsCount;
 * Usage: increaseNeighborsCount(row, col, cellRow, cellCol, grid);
 * ---------------------------------------------
 * Takes reference to grid, row and col position,
 * and cell's row and col position; checks if
 * current position contains life and not cell itself,
 * and return 1, returns 0 otherwise.
 */
int increaseNeighborsCount(int row, int col, int cellRow, int cellCol, Grid<string> & grid) {
    if (!(row == cellRow && col == cellCol) && (grid[row][col] == "X")) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Function: checkNumberOfNeighbors;
 * Usage: checkNumberOfNeighbors(row, col, grid);
 * ---------------------------------------------
 * Takes coordinates of cell in grid and reference
 * to grid; checks 8 adjacent surrounding cells
 * and return number of cells containing life.
 */
int checkNumberOfNeighbors(int row, int col, Grid<string> & grid) {
    int numberOfNeighbors = 0;

    for (int r = row-1; r <= row+1; r++) {
        for (int c = col-1; c <= col+1; c++) {
            if (grid.inBounds(r,c)) {
                numberOfNeighbors += increaseNeighborsCount(r, c, row, col, grid);
            } else if (!(grid.inBounds(r,c)) && (isWrappingEnabled == true)) {
                int wr = wrapCell(r, grid.height());
                int wc = wrapCell(c, grid.width());
                numberOfNeighbors += increaseNeighborsCount(wr, wc, row, col, grid);
            }
        }
    }

    return numberOfNeighbors;
}

/*
 * Function: simulate;
 * Usage: simulate(grid);
 * ---------------------
 * Takes reference to grid, checks
 * every element and applying game rules
 * according to number of neighbors, after
 * finishing displays grid to the console.
 */
void simulate(Grid<string> & grid) {
    Grid<string> tempGrid = grid;

    for (int r = 0; r < tempGrid.numRows(); r++) {
        for (int c = 0; c < tempGrid.numCols(); c++) {
            int numOfNeighbors = checkNumberOfNeighbors(r, c, grid);
            tempGrid[r][c] = applyCellRules(tempGrid[r][c], numOfNeighbors);
        }
    }

    grid = tempGrid;
    displayGrid(grid);
}
