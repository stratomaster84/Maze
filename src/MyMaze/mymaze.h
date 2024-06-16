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

// GET METHODS
    const uint32_t* get_right_mask() const;
    const uint32_t* get_bottom_mask() const;
    maze_point      get_size() const;

    maze_point      get_start() const;
    maze_point      get_stop() const;
    const std::vector<maze_point>& get_solution() const;
    bool            get_diag_step() const;

    uint8_t get_free_neighbours_FN(int x, int y);   //найти свободных соседей в окрестности фон-Неймана
                                                    // -----------
                                                    // --|-|0|-|--
                                                    // --|6|X|2|--
                                                    // --|-|4|-|--
                                                    // -----------
    uint8_t get_free_neighbours_Moore(int x, int y);//найти свободных соседей в окрестности Мура
                                                    // -----------
                                                    // --|7|0|1|--
                                                    // --|6|X|2|--
                                                    // --|5|4|3|--
                                                    // -----------

// SET METHODS
    void invert_right(int x, int y);
    void invert_bottom(int x, int y);

    void resize_maze(int width, int height);
    void set_start(int x, int y);
    void set_stop(int x, int y);
    void set_diag_step(bool val);
    void clear_solution();

// RESOLVE METHOD
    int  resolve_maze();            // 0 - OK
                                    // 1 - NO START
                                    // 2 - NO STOP
                                    // 3 - NO SOLUTIONS

// MAZE CONSTRUCTORS
    void set_random_maze();         // простой случайный лабиринт
    void set_ellers_maze();         // лабиринт по алгоритму Эллера

private:
// MAZE VARIABLES
    uint32_t right_maze_mask [32];      // матрица ПРАВЫХ сторон лабиринта (каждое значение - СТРОКА, а не столбец)
    uint32_t bottom_maze_mask[32];      // матрица НИЖНИХ сторон лабиринта  (каждое значение - СТРОКА, а не столбец)
    maze_point maze_size;

// SOLUTION VARIABLES
    bool allow_diagonal;
    maze_point solution_start;
    maze_point solution_stop;
    std::vector<maze_point> solution;   // {-1, 0} - to left
                                        // { 1, 0} - to right
                                        // { 0,-1} - to down
                                        // { 0, 1} - to up
                                        // {

};

//===========================================================================
#endif
