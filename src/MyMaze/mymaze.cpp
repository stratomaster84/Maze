#include "mymaze.h"

MyMaze::MyMaze(int width, int height)
    : maze_size({width,height}),
      solution_start({-1,-1}),
      solution_stop({-1,-1})
{
    right_maze_mask[0] = 0x00'00'00'01;
    bottom_maze_mask[0] = 0x00'00'00'00;
    for(int i=1;i<32;++i){
        right_maze_mask[i] = 0x00'00'00'00;
        bottom_maze_mask[i] = 0x00'00'00'00;
    }
}

void MyMaze::resize_maze(int width, int height)
{
    maze_size.x = width;
    maze_size.y = height;

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

void MyMaze::set_random_maze()
{
    solution.clear();
    for(int i=0;i<32;++i){
        right_maze_mask[i] = 0xFF'FF'FF'FF;
        bottom_maze_mask[i] = 0xFF'FF'FF'FF;
    }
}
void MyMaze::resolve_maze()
{
    solution_start = {0,0};
    solution_stop  = {maze_size.x-1,maze_size.y-1};

    solution.clear();
    solution.push_back({0,1});
    solution.push_back({0,1});
}
