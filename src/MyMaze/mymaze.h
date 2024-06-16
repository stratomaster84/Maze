
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
    uint32_t right_maze_mask[32];       // матрица ПРАВЫХ сторон лабиринта (каждое значение - СТРОКА, а не столбец)
    uint32_t bottom_maze_mask[32];      // матрица НИЖНИХ сторон лабиринта  (каждое значение - СТРОКА, а не столбец)
    maze_point maze_size;

// SOLUTION VARIABLES
    maze_point solution_start;
    maze_point solution_stop;
    std::vector<maze_point> solution;   // {-1, 0} - to left
                                        // { 1, 0} - to right
                                        // { 0,-1} - to down
                                        // { 0, 1} - to up

// FUNCTIONS
    void resize_maze(int width, int height);
    void set_random_maze();
    void resolve_maze();
};

//===========================================================================
#endif
