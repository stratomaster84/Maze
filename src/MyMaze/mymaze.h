
#ifndef mymaze_H
#define mymaze_H

#include <vector>
#include <cstdint>

//===========================================================================
struct maze_point
{
    int x;
    int y;
};

//===========================================================================
struct MyMaze
{
    MyMaze(int heigth, int width);

// MAZE VARIABLES
    uint32_t right_maze_mask[32];    // матрица ПРАВЫХ сторон лабиринта (каждое значение - СТРОКА, а не столбец)
    uint32_t bottom_maze_mask[32];    // матрица НИЖНИХ сторон лабиринта  (каждое значение - СТРОКА, а не столбец)
    maze_point maze_size;

// SOLUTION VARIABLES
    bool have_solution;
    maze_point solution_start;
    maze_point solution_stop;
    std::vector<bool> solution;

// FUNCTIONS
    void resize_maze(int width, int height);
};

//===========================================================================
#endif
