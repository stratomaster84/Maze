#include "mymaze.h"
#include <iostream>

//=CONSTRUCTOR===============================================================
MyMaze::MyMaze(int width, int height)
    : maze_size({width,height}),
      allow_diagonal(false),
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

//==GET METHODS===============================================================
const uint32_t* MyMaze::get_right_mask() const
{
    return right_maze_mask;
}
//===========================================================================
const uint32_t* MyMaze::get_bottom_mask() const
{
    return bottom_maze_mask;
}
//===========================================================================
maze_point MyMaze::get_size() const
{
    return maze_size;
}
//===========================================================================
maze_point MyMaze::get_start() const
{
    return solution_start;
}
//===========================================================================
maze_point MyMaze::get_stop() const
{
    return solution_stop;
}
//===========================================================================
const std::vector<maze_point>& MyMaze::get_solution() const
{
    return solution;
}
//===========================================================================
bool MyMaze::get_diag_step() const
{
    return allow_diagonal;
}
//===========================================================================
uint8_t MyMaze::get_free_neighbours_FN(int x, int y)// -----------
                                                    // --|-|0|-|--
                                                    // --|6|X|2|--
                                                    // --|-|4|-|--
                                                    // -----------
{
    if(x<0 || x>=maze_size.x || y<0 || y>=maze_size.y)
        return 0b00'00'00'00;
    uint8_t ret_val = 0b00'00'00'00;
    uint32_t x_mask = 0x00'00'00'01 << x;
    if(x<maze_size.x-1)
        if(!(right_maze_mask[y] & x_mask))  // right(x,y)
            ret_val |= 0b00'00'01'00;       // правый свободен
    if(y<maze_size.y-1)
        if(!(bottom_maze_mask[y] & x_mask)) // bottom(x,y)
            ret_val |= 0b00'01'00'00;       // нижний свободен
    if(y>0)
        if(!(bottom_maze_mask[y-1] & x_mask))// top(x,y) = bottom(x,y-1)
            ret_val |= 0b00'00'00'01;       // верхний свободен
    if(x>0){
        if(!(right_maze_mask[y] & (x_mask>>1)))// left(x,y) = right(x-1,y)
            ret_val |= 0b01'00'00'00;       // левый свободен
    }
    return ret_val;
}
//===========================================================================
uint8_t MyMaze::get_free_neighbours_Moore(int x, int y) // -----------
                                                        // --|7|0|1|--
                                                        // --|6|X|2|--
                                                        // --|5|4|3|--
                                                        // -----------
{
    if(x<0 || x>=maze_size.x || y<0 || y>=maze_size.y)
        return 0b00'00'00'00;
    uint8_t ret_val = get_free_neighbours_FN(x,y);
    uint32_t x_mask = 0x00'00'00'01 << x;
    if((ret_val & 0b00'00'00'01) && (ret_val & 0b00'00'01'00)){ // свободны верхний и правый
        if((!(right_maze_mask[y-1] & x_mask)) &&                // left(x+1,y-1) = right(x,y-1)
           (!(bottom_maze_mask[y-1] & (x_mask<<1))))            // bottom(x+1,y-1)
            ret_val |= 0b00'00'00'10;                           // свободен 1
    }
    if((ret_val & 0b00'01'00'00) && (ret_val & 0b00'00'01'00)){ // свободны нижний и правый
        if((!(right_maze_mask[y+1] & x_mask)) &&                // left(x+1,y+1) = right(x,y+1)
           (!(bottom_maze_mask[y] & (x_mask<<1))))              // top(x+1,y+1) = bottom(x+1,y)
            ret_val |= 0b00'00'10'00;                           // свободен 3
    }
    if((ret_val & 0b00'01'00'00) && (ret_val & 0b01'00'00'00)){ // свободны нижний и левый
        if((!(right_maze_mask[y+1] & (x_mask>>1))) &&           // right(y+1,x-1)
           (!(bottom_maze_mask[y] & (x_mask>>1))))              // top(y+1,x-1) = bottom(y,x-1)
            ret_val |= 0b00'10'00'00;                           // свободен 5
    }
    if((ret_val & 0b00'00'00'01) && (ret_val & 0b01'00'00'00)){ // свободны верхний и левый
        if((!(right_maze_mask[y-1] & (x_mask>>1))) &&           // right(x-1,y-1)
           (!(bottom_maze_mask[y-1] & (x_mask>>1))))            // bottom(x-1,y-1)
            ret_val |= 0b10'00'00'00;                           // свободен 7
    }
    return ret_val;
}

//===SET METHODS===============================================================
void MyMaze::invert_right(int x, int y)
{
    if(x<0 || x>=maze_size.x-1 || y<0 || y>=maze_size.y)
        return;
    solution.clear();
    uint32_t x_mask = 0x00'00'00'01 << x;
    right_maze_mask[y] ^= x_mask;

    if(get_free_neighbours_FN(solution_start.x,solution_start.y) == 0b00'00'00'00)
        solution_start = {-1,-1};
    if(get_free_neighbours_FN(solution_stop.x,solution_stop.y) == 0b00'00'00'00)
        solution_stop = {-1,-1};
}
//===========================================================================
void MyMaze::invert_bottom(int x, int y)
{
    if(x<0 || x>=maze_size.x || y<0 || y>=maze_size.y-1)
        return;
    solution.clear();
    uint32_t x_mask = 0x00'00'00'01 << x;
    bottom_maze_mask[y] ^= x_mask;

    if(get_free_neighbours_FN(solution_start.x,solution_start.y) == 0b00'00'00'00)
        solution_start = {-1,-1};
    if(get_free_neighbours_FN(solution_stop.x,solution_stop.y) == 0b00'00'00'00)
        solution_stop = {-1,-1};
}
//===========================================================================
void MyMaze::resize_maze(int width, int height)
{
    if(width<2 || width>32 || height<2 || height>32)
        return;
    solution.clear();

    maze_size = {width,height};

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
    if(x<0 || x>=maze_size.x || y<0 || y>=maze_size.y)
        return;
    if(get_free_neighbours_FN(x,y) == 0b00'00'00'00)
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
    if(x<0 || x>=maze_size.x || y<0 || y>=maze_size.y)
        return;
    if(get_free_neighbours_FN(x,y) == 0b00'00'00'00)
        return;
    solution.clear();
    solution_stop = {x,y};

    if(solution_start.x == solution_stop.x &&
       solution_start.y == solution_stop.y)
        solution_start = {-1,-1};
}
//===========================================================================
void MyMaze::set_diag_step(bool val)
{
    allow_diagonal = val;
}

//====RESOLVE METHOD============================================================
int MyMaze::resolve_maze()      // 0 - OK
                                // 1 - NO START POINT
                                // 2 - NO STOP POINT
                                // 3 - NO SOLUTIONS
{
    if(solution_start.x == -1 ||
       solution_start.y == -1)
        return 1;
    if(solution_stop.x  == -1 ||
       solution_stop.y  == -1)
        return 2;

    std::vector<std::vector<int>> steps_map;
    for(int x=0;x<maze_size.x;++x){
        steps_map.push_back(std::vector<int>());
        for(int y=0;y<maze_size.y;++y){
            steps_map[x].push_back(-1);
        }
    }

    int step = 0;
    std::vector<maze_point> cells_on_current_step;
    std::vector<maze_point> cells_on_next_step;
    cells_on_next_step.push_back(solution_start);
    steps_map[solution_start.x][solution_start.y] = step;

    bool no_solution = true;
    while(no_solution){
        if(cells_on_next_step.empty())
            break;
        cells_on_current_step = move(cells_on_next_step);

        ++step;

        for(auto [curr_x, curr_y] : cells_on_current_step){
           uint8_t curr_neighbours_mask = get_free_neighbours_Moore(curr_x, curr_y);
                                                                // -----------
                                                                // --|7|0|1|--
                                                                // --|6|X|2|--
                                                                // --|5|4|3|--
                                                                // -----------
            std::vector<maze_point> neighbours;
            if(curr_neighbours_mask & 0b00'00'00'01)            // есть клетка 0
                neighbours.push_back({curr_x,curr_y-1});
            if(curr_neighbours_mask & 0b00'00'01'00)            // есть клетка 2
                neighbours.push_back({curr_x+1,curr_y});
            if(curr_neighbours_mask & 0b00'01'00'00)            // есть клетка 4
                neighbours.push_back({curr_x,curr_y+1});
            if(curr_neighbours_mask & 0b01'00'00'00)            // есть клетка 6
                neighbours.push_back({curr_x-1,curr_y});
            if(allow_diagonal){
                if(curr_neighbours_mask & 0b00'00'00'10)        // есть клетка 1
                    neighbours.push_back({curr_x+1,curr_y-1});
                if(curr_neighbours_mask & 0b00'00'10'00)        // есть клетка 3
                    neighbours.push_back({curr_x+1,curr_y+1});
                if(curr_neighbours_mask & 0b00'10'00'00)        // есть клетка 5
                    neighbours.push_back({curr_x-1,curr_y+1});
                if(curr_neighbours_mask & 0b10'00'00'00)        // есть клетка 7
                    neighbours.push_back({curr_x-1,curr_y-1});
            }
            for(auto [next_x, next_y] : neighbours){
                if(solution_stop.x == next_x &&                 // найдена конечная точка
                   solution_stop.y == next_y){
                    steps_map[next_x][next_y] = step;
                    no_solution = false;
                    break;
                }
                if(steps_map[next_x][next_y] == -1){
                    steps_map[next_x][next_y] = step;
                    cells_on_next_step.push_back({next_x, next_y});
                }
            }
            if(!no_solution)
                break;
        }
    }
    if(no_solution)
        return 3;

// обратный расчёт решения
    solution.clear();
    int curr_x = solution_stop.x;
    int curr_y = solution_stop.y;

    no_solution = true;
    while(step){
        --step;
        uint8_t curr_neighbours_mask = get_free_neighbours_Moore(curr_x, curr_y);
        std::vector<maze_point> neighbours;
        if(curr_neighbours_mask & 0b00'00'00'01)            // есть клетка 0
            neighbours.push_back({curr_x,curr_y-1});
        if(curr_neighbours_mask & 0b00'00'01'00)            // есть клетка 2
            neighbours.push_back({curr_x+1,curr_y});
        if(curr_neighbours_mask & 0b00'01'00'00)            // есть клетка 4
            neighbours.push_back({curr_x,curr_y+1});
        if(curr_neighbours_mask & 0b01'00'00'00)            // есть клетка 6
            neighbours.push_back({curr_x-1,curr_y});
        if(allow_diagonal){
            if(curr_neighbours_mask & 0b00'00'00'10)        // есть клетка 1
                neighbours.push_back({curr_x+1,curr_y-1});
            if(curr_neighbours_mask & 0b00'00'10'00)        // есть клетка 3
                neighbours.push_back({curr_x+1,curr_y+1});
            if(curr_neighbours_mask & 0b00'10'00'00)        // есть клетка 5
                neighbours.push_back({curr_x-1,curr_y+1});
            if(curr_neighbours_mask & 0b10'00'00'00)        // есть клетка 7
                neighbours.push_back({curr_x-1,curr_y-1});
        }
        for(auto [next_x, next_y] : neighbours){
            if(steps_map[next_x][next_y] == step){
                solution.push_back({next_x-curr_x, next_y-curr_y});
                curr_x = next_x;
                curr_y = next_y;
                break;
            }
        }
    }
    return 0;
}

//====MAZE CONSTRUCTORS============================================================
void MyMaze::set_random_maze()
{
    solution.clear();
    for(int i=0;i<32;++i){
        right_maze_mask[i]  = (std::rand()<<24) | (std::rand()<<12) | std::rand();
        bottom_maze_mask[i] = (std::rand()<<24) | (std::rand()<<12) | std::rand();
    }
    if(get_free_neighbours_FN(solution_start.x,solution_start.y) == 0b00'00'00'00)
        solution_start = {-1,-1};
    if(get_free_neighbours_FN(solution_stop.x,solution_stop.y) == 0b00'00'00'00)
        solution_stop = {-1,-1};
}
//===========================================================================
void MyMaze::set_ellers_maze()
{
    // Здесь будет генератор лабиринта по алгоритму Эллера
}
//===========================================================================
