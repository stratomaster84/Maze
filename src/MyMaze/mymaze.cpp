#include "mymaze.h"

MyMaze::MyMaze(int width, int height)
    : maze_size({width,height}),
      have_solution(false),
      solution_start({-1,-1}),
      solution_stop({-1,-1})
{
    for(int i=0;i<32;++i){
        right_maze_mask[i] = 0xFFFFFFFF;
        bottom_maze_mask[i] = 0xFFFFFFFF;
    }
}

void MyMaze::resize_maze(int width, int height)
{
    maze_size.x = width;
    maze_size.y = height;

    have_solution = false;
    solution.clear();

    if(solution_start.x >= width ||
       solution_start.y >= height){
        solution_start.x = -1;
        solution_start.y = -1;
    }
    if(solution_stop.x >= width ||
       solution_stop.y >= height){
        solution_stop.x = -1;
        solution_stop.y = -1;
    }
}
