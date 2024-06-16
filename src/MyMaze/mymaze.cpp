#include "mymaze.h"
#include <iostream>

//=КОНСТРУКТОР===============================================================
MyMaze::MyMaze()
    : maze_size({MIN_MAZE_SIZE,MIN_MAZE_SIZE}), // размер установить минимальный
      allow_diagonal(false),    // запретить шаг по диагонали
      solution_start({-1,-1}),  // точка старта отсутствует
      solution_stop({-1,-1})    // точка финиша отсутствует
{
// запишем начальный лабиринт по алгоритму Эллера
    set_ellers_maze();
}

//==МЕТОДЫ ПОЛУЧЕНИЯ ПЕРЕМЕННЫХ ЛАБИРИНТА===================================================
const MASK_TYPE* MyMaze::get_right_mask() const     // получить указатель на маску стенок справа
{
    return right_maze_mask;
}
//===========================================================================
const MASK_TYPE* MyMaze::get_bottom_mask() const    // получить указатель на маску стенок снизу
{
    return bottom_maze_mask;
}
//===========================================================================
maze_point MyMaze::get_size() const     // получить размер лабиринта
{
    return maze_size;
}
//===========================================================================
maze_point MyMaze::get_start() const    // получить точку старта
{
    return solution_start;
}
//===========================================================================
maze_point MyMaze::get_stop() const     // получить точку финиша
{
    return solution_stop;
}
//===========================================================================
const std::vector<maze_point>& MyMaze::get_solution() const // получить указатель на решение
{
    return solution;
}
//===========================================================================
bool MyMaze::get_diag_step() const  // разрешён ли диагональный шаг
{
    return allow_diagonal;
}
//===========================================================================
uint8_t MyMaze::get_free_neighbours_FN(int x, int y)//найти свободных соседей в окрестности фон-Неймана
                                                    // -----------
                                                    // --|-|0|-|--
                                                    // --|6|X|2|--
                                                    // --|-|4|-|--
                                                    // -----------
{
    if(x<0 || x>=maze_size.x || y<0 || y>=maze_size.y)  // если точка за пределами лабиринта
                                                        // возвращаем 0
        return 0b00'00'00'00;

    uint8_t ret_val = 0b00'00'00'00;
    MASK_TYPE x_mask = FIRST_BIT_MASK << x; // маска для выборки по горизонтали
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
uint8_t MyMaze::get_free_neighbours_Moore(int x, int y) //найти свободных соседей в окрестности Мура
                                                        // -----------
                                                        // --|7|0|1|--
                                                        // --|6|X|2|--
                                                        // --|5|4|3|--
                                                        // -----------
{
    uint8_t ret_val = get_free_neighbours_FN(x,y);      // получим соседей в окрестности Мура
    if(ret_val == 0b00'00'00'00)                        // если соседи отсутствуют или точка за пределами лабиринта
                                                        // возвращаем 0
        return ret_val;

    MASK_TYPE x_mask = FIRST_BIT_MASK << x;                     // маска для выборки по горизонтали
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

//===МЕТОДЫ УСТАНОВКИ ПЕРЕМЕННЫХ ЛАБИРИНТА=========================================================
void MyMaze::invert_right(int x, int y)         // поменять наличие стенки справа
{
    if(x<0 || x>=maze_size.x-1 || y<0 || y>=maze_size.y)    // проверить можно ли инвертировать
        return; 
    solution.clear();                           // очистим решение
    MASK_TYPE x_mask = FIRST_BIT_MASK << x;     // маска для выборки по горизонтали
    right_maze_mask[y] ^= x_mask;               // инвертируем требуемый бит

    if(get_free_neighbours_FN(solution_start.x,solution_start.y) == 0b00'00'00'00)
                                                        // если точка старта стала изолированной - удаляем её
        solution_start = {-1,-1};

    if(get_free_neighbours_FN(solution_stop.x,solution_stop.y) == 0b00'00'00'00)
                                                        // если точка финиша стала изолированной - удаляем её
        solution_stop = {-1,-1};
}
//===========================================================================
void MyMaze::invert_bottom(int x, int y)        // поменять наличие стенки снизу
                                                // (всё по аналогии с invert_right(int x, int y))
{
    if(x<0 || x>=maze_size.x || y<0 || y>=maze_size.y-1)
        return;
    solution.clear();
    MASK_TYPE x_mask = FIRST_BIT_MASK << x;
    bottom_maze_mask[y] ^= x_mask;

    if(get_free_neighbours_FN(solution_start.x,solution_start.y) == 0b00'00'00'00)
        solution_start = {-1,-1};
    if(get_free_neighbours_FN(solution_stop.x,solution_stop.y) == 0b00'00'00'00)
        solution_stop = {-1,-1};
}
//===========================================================================
void MyMaze::resize_maze(int width, int height)     // поменять размер лабиринта
{
    if(width<MIN_MAZE_SIZE || width>BIT_DEPTH || height<MIN_MAZE_SIZE || height>BIT_DEPTH)
                                                    // если не входит в промежуток [MIN_MAZE_SIZE;BIT_DEPTH]
        return;                                     // выходим

    solution.clear();                   // очистим решение
    maze_size = {width,height};         // установим новый размер

// проверка на изолированность точки старта и точки финиша
    if(get_free_neighbours_FN(solution_start.x,solution_start.y) == 0b00'00'00'00)
        solution_start = {-1,-1};
    if(get_free_neighbours_FN(solution_stop.x,solution_stop.y) == 0b00'00'00'00)
        solution_stop = {-1,-1};

}
//===========================================================================
void MyMaze::set_start(int x, int y)        // установить точку старта
{
    if(x<0 || x>=maze_size.x || y<0 || y>=maze_size.y)  // выходим, если за рамками лабиринта
        return;
    if(get_free_neighbours_FN(x,y) == 0b00'00'00'00)    // выходим, если точка изолирована
        return;
    solution.clear();           // очищаем решение, потому что оно не действительно
    solution_start = {x,y};     // устанавливаем новую точку старта

    if(solution_stop.x == solution_start.x &&
       solution_stop.y == solution_start.y)
        solution_stop = {-1,-1};
}
//===========================================================================
void MyMaze::set_stop(int x, int y)         // установить точку финиша
                                            // (всё по аналогии с set_start(int x, int y))
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
void MyMaze::set_diag_step(bool val)    // разрешить/запретить шаг по диагонали в решении
{
    if(val != allow_diagonal){          // если меняется
        allow_diagonal = val;           // устанавливаем новое значение флага
        if(!solution.empty())           // если было решение
            resolve_maze();             // ищем по новой (с учётом нового значения флага)
    }
}
//===========================================================================
void MyMaze::clear_solution()           // очистить решение
{
    solution.clear();
}

//====МЕТОД ПОИСКА РЕШЕНИЯ===========================================================
int MyMaze::resolve_maze()      // найти кротчайшее решение лабиринта по алгоритму Ли (возвращает код ошибки)
                                // 0 - OK
                                // 1 - нет точки старта
                                // 2 - нет точки финиша
                                // 3 - нет возможных решений
{
    if(solution_start.x == -1 ||
       solution_start.y == -1)
        return 1;                   // нет точки старта - возвращаем 1
    if(solution_stop.x  == -1 ||
       solution_stop.y  == -1)
        return 2;                   // нет точки финиша - возвращаем 2

    std::vector<std::vector<int>> steps_map;    // карта удаления ячеек от точки старта
    for(int x=0;x<maze_size.x;++x){
        steps_map.push_back(std::vector<int>());
        for(int y=0;y<maze_size.y;++y){
            steps_map[x].push_back(-1);         // вначале все = -1
        }
    }

    int step = 0;                                       // текущий шаг =0
    std::vector<maze_point> cells_on_previous_step;     // список ячеек на предыдущем шаге
    std::vector<maze_point> cells_on_current_step;      // список ячеек на текущем шаге
    cells_on_current_step.push_back(solution_start);    // в текущем шаге только одна ячейка - точка старта
    steps_map[solution_start.x][solution_start.y] = step;   // помещаем в картуудалений точку старта

    bool no_solution = true;    // для входа в цикл
    while(no_solution){         // если до сих пор нет решения
//--подготовка для текущего шага
        if(cells_on_current_step.empty())   // если нет ни одной точки в предыдущем шаге,
                                            // выходим из цикла без найденного решения
            break;

        cells_on_previous_step = move(cells_on_current_step);
                            // переносим все точки в предыдущем шаге в нужную переменную
        ++step;             // увеличиваем шаг
// ----------------------------

        for(auto [curr_x, curr_y] : cells_on_previous_step){    // для каждой точки предыдущего шага
           uint8_t curr_neighbours_mask = get_free_neighbours_Moore(curr_x, curr_y);    // находим соседей
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
            if(allow_diagonal){                             // если разрешены диагональные шаги
                if(curr_neighbours_mask & 0b00'00'00'10)        // есть клетка 1
                    neighbours.push_back({curr_x+1,curr_y-1});
                if(curr_neighbours_mask & 0b00'00'10'00)        // есть клетка 3
                    neighbours.push_back({curr_x+1,curr_y+1});
                if(curr_neighbours_mask & 0b00'10'00'00)        // есть клетка 5
                    neighbours.push_back({curr_x-1,curr_y+1});
                if(curr_neighbours_mask & 0b10'00'00'00)        // есть клетка 7
                    neighbours.push_back({curr_x-1,curr_y-1});
            }
            for(auto [next_x, next_y] : neighbours){        // для всех найденных соседей
                if(solution_stop.x == next_x &&
                   solution_stop.y == next_y){              // если найдена точка финиша, то..
                    steps_map[next_x][next_y] = step;       // ...записываем ячейку в карту удаления от старта,
                    no_solution = false;                    // указываем, что решение найдено,
                    break;                                  // выходим из цикла прохода по соседям
                }
                if(steps_map[next_x][next_y] == -1){        // если волна ещё не доходила сюда, то..
                    steps_map[next_x][next_y] = step;       // ...записываем ячейку в карту удаления от старта,
                                                            // записываем ячейку в список ячеек на текущем шаге
                    cells_on_current_step.push_back({next_x, next_y});
                }
            }
            if(!no_solution)    // если найдено решение...
                break;          // ...выходим из цикла
        }
    }
    if(no_solution)     // если нет возможных решений, то...
        return 3;       // ...возвращаем 3

// ОБРАТНЫЙ РАСЧЁТ РЕШЕНИЯ
    solution.clear();               // удаляем существующее решение
    int curr_x = solution_stop.x;   // начинаем записывать решение с точки финиша
    int curr_y = solution_stop.y;

    while(step){
        --step; // текущий шаг - предыдущий
        uint8_t curr_neighbours_mask = get_free_neighbours_Moore(curr_x, curr_y);   // находим соседей
        std::vector<maze_point> neighbours;                 // записываем соседей по аналогии с предыдущим циклом
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
        for(auto [next_x, next_y] : neighbours){            // для всех найденных соседей
            if(steps_map[next_x][next_y] == step){                  // если сосед - ячейка с текущего шага
                solution.push_back({next_x-curr_x, next_y-curr_y}); // записываем в решение новое направление
                curr_x = next_x;        // продолжаем записывать решение с данного найденного соседа
                curr_y = next_y;
                break;                  // заканчиваем цикл прохода по соседям
            }
        }
    }
    return 0;   // нет ошибок - возвращаем 0
}

//====МЕТОДЫ ГЕНЕРАЦИИ ЛАБИРИНТОВ=========================================================
void MyMaze::set_random_maze()  // генерирует простой случайный лабиринт (может быть без решений)
{
    solution.clear();           // удаляем решение
    for(int i=0;i<BIT_DEPTH;++i){
                                // записываем случайные числа в МАТРИЦЫ правых и нижних сторон
        right_maze_mask[i]  = random();
        bottom_maze_mask[i] = random();
    }
    if(get_free_neighbours_FN(solution_start.x,solution_start.y) == 0b00'00'00'00)
                                        // если точка старта стала изолированной - удаляем её
        solution_start = {-1,-1};

    if(get_free_neighbours_FN(solution_stop.x,solution_stop.y) == 0b00'00'00'00)
                                        // если точка финиша стала изолированной - удаляем её
        solution_stop = {-1,-1};
}
//===========================================================================
void MyMaze::set_ellers_maze()      // генерирует лабиринт по алгоритму Эллера (всегда найдутся решения)
{
    solution.clear();               // удаляем решение
    for(int i=0;i<BIT_DEPTH;++i){   // очищаем матрицы лабиринта
        right_maze_mask[i]  = 0x00;
        bottom_maze_mask[i] = 0x00;
    }

    std::vector<int> CURR_ROW_SETS; // строка текущих множеств
    int SET_counter = 0;            // текущее множество

//Шаг 1: Подготовка СТРОКИ ТЕКУЩИХ МНОЖЕСТВ для создания первой строки лабиринта
    for(int x=0;x<maze_size.x;++x)
        CURR_ROW_SETS.push_back(SET_counter);   // записываем 0 (значит, что не принадлежит ни одному множеству)
//-------------------

// ГЛАВНЫЙ ЦИКЛ ДЛЯ СОЗДАНИЯ ОЧЕРЕДНОЙ СТРОКИ
    for(int y=0;y<maze_size.y;++y){
//Шаг 2: Присоединяем все ячейки не принадлежащие множествам к свои новым множествам
        for(int x=0;x<maze_size.x;++x){
            if(CURR_ROW_SETS[x]==0)
                CURR_ROW_SETS[x] = ++SET_counter;
        }
//-------------------
//Шаг 3: Создадим границы справа
        for(int x=0;x<maze_size.x-1;++x){                   // для всех ячеек кроме последней справа
            bool random_choise = randBool();                // случайно решаем ставить стенку или нет
            if((random_choise == true) ||                   // если решили ставить стенку справа...
               (CURR_ROW_SETS[x] == CURR_ROW_SETS[x+1]))    // ...или справа ячейка с тем же множеством, то...
                right_maze_mask[y] |= (FIRST_BIT_MASK<<x);  // ...ставим стенку справа

            else{           // если решили ячейку не ставить
                            // объединяем всё множество,
                            // которому принадлежит следующая ячейка
                            // с множеством, которому принадлежит текущая ячейка
                int SET_to_merge = CURR_ROW_SETS[x+1];
                for(int i=0;i<maze_size.x;++i){
                    if(CURR_ROW_SETS[i] == SET_to_merge)
                        CURR_ROW_SETS[i] = CURR_ROW_SETS[x];
                }
            }
        }
//Для последней строки пропускаем шаги 4 и 5
        if(y == maze_size.y-1)
            break;
//-------------------
//Шаг 4: Создание нижних границ
        for(int x=0;x<maze_size.x;++x){                     // для всех ячеек кроме последней справа
            bool random_choise = randBool();                // случайно решаем ставить стенку или нет
            if(random_choise == true)                       // если решили ставить стенку снизу...
                bottom_maze_mask[y] |= (FIRST_BIT_MASK<<x); // ...ставим стенку снизу
        }
// Убедимся, что каждое множество ячеек имеет хотя бы одну ячейку без нижней границы.
// Если это условие не будет выполнено, то мы создадим изолированные области.
        for(int x=0;x<maze_size.x;++x){     // для всех ячеекв строке
            int EXITS_FROM_SET = 0;         // количество выходов из множества, которому принадлежит текущая ячейка
            for(int i=0;i<maze_size.x;++i){
                if((CURR_ROW_SETS[i] == CURR_ROW_SETS[x]) &&                // если принадлежит текущему...
                   (bottom_maze_mask[y] & (FIRST_BIT_MASK<<i)) == false)    // ...и нет стенки снизу...
                    EXITS_FROM_SET++;                                       // увеличиваем счётчик выходов из множества
            }
            if(EXITS_FROM_SET == 0)                                         // если нет ни одного выхода...
                bottom_maze_mask[y] &= ~(FIRST_BIT_MASK<<x);                // убираем стенку снизу
        }
//-------------------
//Шаг 5: Подготавливаем СТРОКУ ТЕКУЩИХ МНОЖЕСТВ для создания следующей строки
        for(int x=0;x<maze_size.x;++x){
            if(bottom_maze_mask[y] & (FIRST_BIT_MASK<<x))       // если есть стенка снизу
                CURR_ROW_SETS[x] = 0;                           // удаляем ячейку из множества
        }
//-------------------
    }
//Шаг 6: Завершение ПОСЛЕДНЕЙ СТРОКИ "maze_size.y-1"
//Удаляем все границы ячеек принадлежащих разным множествам
    for(int x=0;x<maze_size.x-1;++x){                                   // для всех ячеек кроме последней справа
        if((right_maze_mask[maze_size.y-1] & (FIRST_BIT_MASK<<x)) &&    // если есть стенка справа...
           (CURR_ROW_SETS[x] != CURR_ROW_SETS[x+1])){                   // и следуящая ячейка не принадлежит текущему множеству
            right_maze_mask[maze_size.y-1] &= ~(FIRST_BIT_MASK<<x);     // удаляем стенку справа и...
                                                                        // объединяем всё множество,
                                                                        // которому принадлежит следующая ячейка
                                                                        // с множеством, которому принадлежит текущая ячейка
            int SET_to_merge = CURR_ROW_SETS[x+1];
            for(int i=0;i<maze_size.x;++i){
                if(CURR_ROW_SETS[i] == SET_to_merge)
                    CURR_ROW_SETS[i] = CURR_ROW_SETS[x];
            }
        }
    }
}

//===СПЕЦИАЛЬНЫЕ МЕТОДЫ=========================================================
MASK_TYPE MyMaze::random()      // случайное число требуемой разрядности
{
    MASK_TYPE ret_val;
#if BIT_DEPTH>=8
    ret_val = static_cast<MASK_TYPE>(std::rand())&0xFF;
#endif
#if  BIT_DEPTH>=16
    ret_val |= (static_cast<MASK_TYPE>(std::rand())<<8)&0xFF'00;
#endif
#if  BIT_DEPTH>=32
    ret_val |= ((static_cast<MASK_TYPE>(std::rand())<<24)&0xFF'00'00'00) |
               ((static_cast<MASK_TYPE>(std::rand())<<16)&0x00'FF'00'00) ;
#endif
#if  BIT_DEPTH==64
    ret_val |= ((static_cast<MASK_TYPE>(std::rand())<<56)&0xFF'00'00'00'00'00'00'00) |
               ((static_cast<MASK_TYPE>(std::rand())<<48)&0x00'FF'00'00'00'00'00'00) |
               ((static_cast<MASK_TYPE>(std::rand())<<40)&0x00'00'FF'00'00'00'00'00) |
               ((static_cast<MASK_TYPE>(std::rand())<<32)&0x00'00'00'FF'00'00'00'00) ;
#endif
    return ret_val;
}
//===========================================================================
bool MyMaze::randBool()         // случайное булево значение
{
    return std::rand()&0x00'80;
}
//===========================================================================
