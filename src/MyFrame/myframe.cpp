#include <QPainter>
#include <QMouseEvent>
#include <cmath>
#include "myframe.h"

//===========================================================================
MyFrame::MyFrame(QWidget *parent)
    : QFrame(parent),
      maze(),
      modify_mode(false)    // устанавливаем режим работы приложения - "Resolve Mode"
{

}
//===========================================================================
void MyFrame::paintEvent(QPaintEvent *event){       // событие перерисовки рамки
    QPainter _painter;
    _painter.begin(this);
                                            // рисуем пустую рамку лабиринта
    _painter.fillRect(0,0,width(),height(),QBrush(MAZE_FILL_COLOR,Qt::SolidPattern));
    _painter.end();

    drawMaze();         // рисуем стенки лабиринта и заблокированные ячейки, если есть

    if(!maze.get_solution().empty())    // если есть решение - рисуем его
        drawSolution();

    if(maze.get_start().x >= 0 && maze.get_start().y >= 0)  // есть точка старта - рисуем её
        drawStart();

    if(maze.get_stop().x >= 0 && maze.get_stop().y >= 0)    // есть точка финиша - рисуем её
        drawStop();

    QFrame::paintEvent(event);  // передать управление стандартному обработчику
}
//===========================================================================
void MyFrame::drawMaze(){       // процедура прорисовки лабиринта
    double _hx = (double)width() /(double)maze.get_size().x;    // ширина ячейки в пикселях
    double _hy = (double)height()/(double)maze.get_size().y;    // высота ячейки в пикселях

    QPainter _painter(this);

//------сначала рисуем заблокированные ячейки
//------нельзя в основной блок for, потому что будет затирать нарисованный лабиринт
    for(int y = 0; y < maze.get_size().y; ++y){
        for(int x = 0; x < maze.get_size().x; ++x){
            if(maze.get_free_neighbours_FN(x,y) == 0b00'00'00'00){  // если у ячейки нет соседей - она заблокирована
                                                                    // рисуем рамку цвета MAZE_BLOCKED_COLOR
                    _painter.fillRect(_hx*x,_hy*y,_hx,_hy,QBrush(MAZE_BLOCKED_COLOR,Qt::SolidPattern));
            }
        }
    }
//------теперь рисуем сами стенки
    _painter.setPen(QPen(MAZE_BORDER_COLOR,MAZE_BORDER_WIDTH,Qt::SolidLine));   // устанавливаем перо нужного цвета и толщины
    for(int x = 0; x < maze.get_size().x; ++x){
        MASK_TYPE x_mask = MyMaze::FIRST_BIT_MASK << x;   // маска для поиска по x
        for(int y = 0; y < maze.get_size().y; ++y){
            if((x != (maze.get_size().x-1)) && (maze.get_right_mask()[y] & x_mask)){    // если не последняя ячейка
                                                                                        // и есть стенка справа
                _painter.drawLine(_hx*(x+1),                                            // тогда рисуем правую линию
                                  _hy*y,
                                  _hx*(x+1),
                                  _hy*(y+1));
            }
            if((y != (maze.get_size().y-1)) && (maze.get_bottom_mask()[y] & x_mask)){   // если не последняя ячейка
                                                                                        // и есть стенка справа
                _painter.drawLine(_hx*x,                                                // тогда рисуем нижнюю линию
                                  _hy*(y+1),
                                  _hx*(x+1),
                                  _hy*(y+1));
            }
        }
    }
}
//===========================================================================
void MyFrame::drawSolution(){       // процедура прорисовки решения
    double _hx = (double)width() /(double)maze.get_size().x;    // ширина ячейки в пикселях
    double _hy = (double)height()/(double)maze.get_size().y;    // высота ячейки в пикселях

    int cur_x = maze.get_stop().x;  // начинаем с точкифиниша
    int cur_y = maze.get_stop().y;

    QPainter _painter(this);
    _painter.setPen(QPen(SOLUTION_COLOR,SOLUTION_WIDTH,Qt::SolidLine)); // устанавливаем перо нужного цвета и толщины
    for(auto [dir_x, dir_y] : maze.get_solution()){             // проходим по всем направлениям решения
        _painter.drawLine(_hx*((double) cur_x          + 0.5),  // "+0.5" чтобы начинать с центра ячейки
                          _hy*((double) cur_y          + 0.5),
                          _hx*((double)(cur_x + dir_x) + 0.5),
                          _hy*((double)(cur_y + dir_y) + 0.5));
        cur_x += dir_x;         // устанавливаем новую текущую точку
        cur_y += dir_y;
    }
}
//===========================================================================
void MyFrame::drawStart(){  // процедура прорисовки точки старта
    double _hx = (double)width() /(double)maze.get_size().x;    // ширина ячейки в пикселях
    double _hy = (double)height()/(double)maze.get_size().y;    // высота ячейки в пикселях

    double dx = ((double)maze.get_start().x + 0.5)*_hx;     // X-координата центра ячейки в пикселях
    double dy = ((double)maze.get_start().y + 0.5)*_hy;     // Y-координата центра ячейки в пикселях

    double dh = (_hx<_hy ? _hx : _hy)/4.0;                  // радиус круга для рисования точки =
                                                            // (наименьший размер ячейки по обеим координатам)/2
    QPainter _painter(this);
    _painter.setPen(QPen(Qt::NoPen));                           // устанавливаем отсутствие пера
    _painter.setBrush(QBrush(START_COLOR,Qt::SolidPattern));    // устанавливаем цвет точки
    _painter.drawEllipse((int)(dx-dh),
                         (int)(dy-dh),
                         (int)dh*2,
                         (int)dh*2);
}
//===========================================================================
void MyFrame::drawStop(){       // процедура прорисовки точки финиша
                                // всё по аналогии с "drawStart()"
    double _hx = (double)width() /(double)maze.get_size().x;
    double _hy = (double)height()/(double)maze.get_size().y;

    double dx = ((double)maze.get_stop().x + 0.5)*_hx;
    double dy = ((double)maze.get_stop().y + 0.5)*_hy;

    double dh = (_hx<_hy ? _hx : _hy)/4.0;

    QPainter _painter(this);
    _painter.setPen(QPen(Qt::NoPen));
    _painter.setBrush(QBrush(STOP_COLOR,Qt::SolidPattern));
    _painter.drawEllipse((int)(dx-dh),
                         (int)(dy-dh),
                         (int)dh*2,
                         (int)dh*2);
}
//===========================================================================
void MyFrame::mousePressEvent(QMouseEvent *event)       // событие нажатие кнопки мыши
{
    double _hx = (double)width() /(double)maze.get_size().x;    // ширина ячейки в пикселях
    double _hy = (double)height()/(double)maze.get_size().y;    // высота ячейки в пикселях

    int x = (int)((double)event->x()/_hx);      // ЦЕЛЫЕ координаты ячейки, на которой нажата кнопка мыши
    int y = (int)((double)event->y()/_hy);

    if(modify_mode){                                // выбран режим "modify mode"
        if(event->buttons() & Qt::LeftButton){      // если нажата левая кнопка мыши
            double dx = (double)event->x()/_hx;     // координаты ячейки, по которой нажата кнопка мыши...
            double dy = (double)event->y()/_hy;     // ЧИСЛА С ПЛАВАЮЩЕЙ ТОЧКОЙ
            double round_x = std::round(dx);        // координаты ближайшей стенки
            double round_y = std::round(dy);
            double delta_x = abs(round_x - dx);     // удаления от ближайшей стенки
            double delta_y = abs(round_y - dy);
            if     (delta_x <= MAZE_MODIFY_DELTA && round_x < maze.get_size().x){   // если указатель мыши находится
                                                                                    // достаточно близко к правой стенке
                                                                                    // и это не крайняя правая ячейка
                maze.invert_right(round_x-1,y);                                     // инвертируем наличие стенки
                repaint();                                                          // ... и перерисовываем всю рамку
            }
            else if(delta_y <= MAZE_MODIFY_DELTA && round_y < maze.get_size().y){   // по аналогии, только по Y-координате
                maze.invert_bottom(x,round_y-1);
                repaint();
            }
        }
    }
    else{           // выбран режим "resolve mode"
        if(event->buttons() & Qt::LeftButton){                          // нажата левая кнопка мыши
            if(maze.get_start().x != x || maze.get_start().y != y){     // проверяем изменились ли координаты точки старта
                maze.set_start(x,y);                                    // устанавливаем новую точку старта
                emit dots_changed();                                    // уведомляем об этом главную форму
                                                                        // (обработчик сигнала должен перерисовать рамку)
            }
        }
        else if(event->buttons() & Qt::RightButton){                    // нажата правая кнопка мыши
            if(maze.get_stop().x != x || maze.get_stop().y != y){       // то же для точки финиша
                maze.set_stop(x,y);
                emit dots_changed();
            }
        }
    }

    QFrame::mousePressEvent(event);     // передать управление стандартному обработчику
}
//===========================================================================
