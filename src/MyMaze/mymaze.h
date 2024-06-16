#ifndef mymaze_H
#define mymaze_H

#include <vector>
#include <cstdint>

#define MIN_MAZE_SIZE 2
#define BIT_DEPTH 64

#if BIT_DEPTH==8
typedef uint8_t MASK_TYPE;
#elif  BIT_DEPTH==16
typedef uint16_t MASK_TYPE;
#elif  BIT_DEPTH==32
typedef uint32_t MASK_TYPE;
#elif  BIT_DEPTH==64
typedef uint64_t MASK_TYPE;
#endif

//===========================================================================
struct maze_point
{
    int x;
    int y;
};
//===========================================================================
struct MyMaze
{
public:
    const static MASK_TYPE FIRST_BIT_MASK=0x01;
    MyMaze(int heigth, int width);

// GET METHODS
    const MASK_TYPE* get_right_mask() const;
    const MASK_TYPE* get_bottom_mask() const;
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
    MASK_TYPE right_maze_mask [BIT_DEPTH];      // матрица ПРАВЫХ сторон лабиринта (каждое значение - СТРОКА, а не столбец)
    MASK_TYPE bottom_maze_mask[BIT_DEPTH];      // матрица НИЖНИХ сторон лабиринта  (каждое значение - СТРОКА, а не столбец)
    maze_point maze_size;

// SOLUTION VARIABLES
    bool allow_diagonal;
    maze_point solution_start;
    maze_point solution_stop;
    std::vector<maze_point> solution;   // {-1, 0} - to left
                                        // { 1, 0} - to right
                                        // { 0, 1} - to up
                                        // { 0, 1} - to down
                                        // { 1, 1} - diagonal right-down, etc.

// SPECIAL METHODS
    MASK_TYPE random();
    bool randBool();
};

//===========================================================================
#endif
