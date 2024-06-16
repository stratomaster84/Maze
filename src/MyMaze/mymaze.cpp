#include "mymaze.h"

//===========================================================================
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
//===========================================================================
void MyMaze::resize_maze(int width, int height)
{
    solution.clear();

    maze_size.x = width;
    maze_size.y = height;

    if(solution_start.x >= width ||
       solution_start.y >= height){
        solution_start = {-1,-1};
    }
    if(solution_stop.x >= width ||
       solution_stop.y >= height){
        solution_stop = {-1,-1};
    }
}
//===========================================================================
void MyMaze::set_start(int x, int y)
{
    if(checkBlocked(x,y))
        return;
    solution.clear();
    solution_start = {x,y};
    if(solution_stop.x == solution_start.x &&
       solution_stop.y == solution_start.y)
        solution_stop = {-1,-1};
}
//===========================================================================
void MyMaze::set_stop(int x, int y)
{
    if(checkBlocked(x,y))
        return;
    solution.clear();
    solution_stop = {x,y};
    if(solution_start.x == solution_stop.x &&
       solution_start.y == solution_stop.y)
        solution_start = {-1,-1};
}
//===========================================================================
void MyMaze::invert_right(int x, int y)
{
    solution.clear();
    uint32_t tmp = 0x00'00'00'01 << x;
    if(right_maze_mask[y] & tmp)
        right_maze_mask[y] &= ~tmp;
    else
        right_maze_mask[y] |= tmp;
    if(checkBlocked(solution_start.x,solution_start.y))
        solution_start = {-1,-1};
    if(checkBlocked(solution_stop.x,solution_stop.y))
        solution_stop = {-1,-1};
}
//===========================================================================
void MyMaze::invert_bottom(int x, int y)
{
    solution.clear();
    uint32_t tmp = 0x00'00'00'01 << x;
    if(bottom_maze_mask[y] & tmp)
        bottom_maze_mask[y] &= ~tmp;
    else
        bottom_maze_mask[y] |= tmp;
    if(checkBlocked(solution_start.x,solution_start.y))
        solution_start = {-1,-1};
    if(checkBlocked(solution_stop.x,solution_stop.y))
        solution_stop = {-1,-1};
}
//===========================================================================
bool MyMaze::checkBlocked(int x, int y){
    if(x<0 || x>=maze_size.x || y<0 || y>=maze_size.y)
        return true;
    bool ret_val = true;
    uint32_t tmp = 0x00'00'00'01 << x;
    if(x<maze_size.x-1)
        ret_val &= (bool)(right_maze_mask[y] & tmp);
    if(y<maze_size.y-1)
        ret_val &= (bool)(bottom_maze_mask[y] & tmp);
    if(y>0)
        ret_val &= (bool)(bottom_maze_mask[y-1] & tmp);
    if(x>0){
        tmp = 0x00'00'00'01 << (x-1);
        ret_val &= (bool)(right_maze_mask[y] & tmp);
    }
    return ret_val;
}
//===========================================================================
bool MyMaze::getRight(int x, int y)
{
    if(x<0 || x>=maze_size.x-1 || y<0 || y>=maze_size.y)
        return true;
    uint32_t tmp = 0x00'00'00'01 << x;
    return right_maze_mask[y] & tmp;
}
//===========================================================================
bool MyMaze::getLeft(int x, int y)
{
    if(x<=0 || x>=maze_size.x || y<0 || y>=maze_size.y)
        return true;
    uint32_t tmp = 0x00'00'00'01 << (x-1);
    return right_maze_mask[y] & tmp;
}
//===========================================================================
bool MyMaze::getBottom(int x, int y)
{
    if(x<0 || x>=maze_size.x || y<0 || y>=maze_size.y-1)
        return true;
    uint32_t tmp = 0x00'00'00'01 << x;
    return bottom_maze_mask[y] & tmp;
}
//===========================================================================
bool MyMaze::getTop(int x, int y)
{
    if(x<0 || x>=maze_size.x || y<=0 || y>=maze_size.y)
        return true;
    uint32_t tmp = 0x00'00'00'01 << x;
    return bottom_maze_mask[y-1] & tmp;
}
//===========================================================================
void MyMaze::set_random_maze()
{
    solution.clear();
    for(int i=0;i<32;++i){
        right_maze_mask[i] = 0xFF'FF'FF'FF;
        bottom_maze_mask[i] = 0xFF'FF'FF'FF;
    }
}
//===========================================================================
void MyMaze::resolve_maze()
{
    solution.clear();

    solution_start = {0,0};
    solution_stop  = {maze_size.x-1,maze_size.y-1};

    solution = {{1,0},{0,1},{1,0},{0,1},{-1,0},{0,-1}};
}
//===========================================================================
