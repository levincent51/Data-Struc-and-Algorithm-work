// BFS maze solver

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "helpers.h"
#include "Maze.h"
#include "Queue.h"

bool isValid(int row, int col, int height, int width);

bool solve(Maze m) {
    // TODO: Complete this function
    //       Feel free to add helper functions
    // createCell Matrix finds path

    Cell Start = MazeGetStart(m);
    int height = MazeHeight(m);
    int width = MazeWidth(m);
    
    bool **Visited = createBoolMatrix(height, width);
    Cell **Pred = createCellMatrix(height, width);

    Queue q = QueueNew();

    Visited[Start.row][Start.col] = true;
    Pred[Start.row][Start.col] = Start;
    MazeVisit(m, Start);
    
    QueueEnqueue(q, Start);

    Cell exit = {-1,-1};
    while (!QueueIsEmpty(q)) {
        Cell curr = QueueDequeue(q);

        if (MazeVisit(m, curr) == true) {
            MazeMarkPath(m, curr);
            exit = curr;
            break;
        }
        for(int i = 0; i < 4; i++) {
            Cell cellVisit = curr;
            // visit all 4 directions left, right, down, up
            switch (i) {
            case 0:
                cellVisit.col--;
                break;
            case 1:
                cellVisit.col++;
                break;
            case 2:
                cellVisit.row++;
                break;
            case 3:
                cellVisit.row--;
                break;
            }

            if (isValid(cellVisit.row, cellVisit.col, height, width) && 
                !MazeIsWall(m, cellVisit) && !Visited[cellVisit.row][cellVisit.col]) {
                Visited[cellVisit.row][cellVisit.col] = true;
                Pred[cellVisit.row][cellVisit.col] = curr;
                QueueEnqueue(q, cellVisit);
            }

        }
    }

    // use maze visit on our queue, if maze is wall change cell to -1 -1
    // if cell is exit mazevisit returns true, if true then find path in array
    
    // if exit is found, (i,j) index marks exit, then find content of Cell Pred and keep unwinding till Cell STart.
    // explore down then left right than up, if alr visited ignore,

    if (exit.row != -1 && exit.col != -1) {
        // unwind BF
        Cell curr = Pred[exit.row][exit.col];
        MazeMarkPath(m, curr);
        while ((curr.row != Start.row) || (curr.col != Start.col)) {
            curr = Pred[curr.row][curr.col];
            MazeMarkPath(m, curr);
        }
        MazeMarkPath(m,Start);
        return true;
    }
    return false;
}

bool isValid(int row, int col, int height, int width) {
    // return true if row number and column number
    // is in range
    return (row >= 0) && (row < height) && (col >= 0) && (col < width);
}
