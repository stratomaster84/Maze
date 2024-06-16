#include <QPainter>
#include <QMouseEvent>
#include <cmath>
#include "myframe.h"

//===========================================================================
MyFrame::MyFrame(QWidget *parent)
    : QFrame(parent),
      maze(MIN_MAZE_SIZE,MIN_MAZE_SIZE),
      modify_mode(false)
{

}
//===========================================================================
void MyFrame::paintEvent(QPaintEvent *event){
    QPainter _painter;
    _painter.begin(this);
    _painter.fillRect(0,0,width(),height(),QBrush(MAZE_FILL_COLOR,Qt::SolidPattern));
    _painter.end();

    drawMaze();

    if(!maze.get_solution().empty())
        drawSolution();

    if(maze.get_start().x >= 0 && maze.get_start().y >= 0)
        drawStart();

    if(maze.get_stop().x >= 0 && maze.get_stop().y >= 0)
        drawStop();

    QFrame::paintEvent(event);   // передать управление стандартному обработчику
}
//===========================================================================
void MyFrame::drawMaze(){
    double _hx = (double)width() /(double)maze.get_size().x;
    double _hy = (double)height()/(double)maze.get_size().y;

    QPainter _painter(this);

//------сначала рисуем заблокированные ячейки
//------нельзя в основной блок for, потому что будет затирать нарисованный лабиринт
    for(int y = 0; y < maze.get_size().y; ++y){
        for(int x = 0; x < maze.get_size().x; ++x){
            if(maze.get_free_neighbours_FN(x,y) == 0b00'00'00'00){
                    _painter.fillRect(_hx*x,_hy*y,_hx,_hy,QBrush(MAZE_BLOCKED_COLOR,Qt::SolidPattern));
            }
        }
    }
//------
    _painter.setPen(QPen(MAZE_BORDER_COLOR,MAZE_BORDER_WIDTH,Qt::SolidLine));
    for(int x = 0; x < maze.get_size().x; ++x){
        MASK_TYPE x_mask = MyMaze::FIRST_BIT_MASK << x;   // маска для поиска по x
        for(int y = 0; y < maze.get_size().y; ++y){
            if((x != (maze.get_size().x-1)) && (maze.get_right_mask()[y] & x_mask)){
                // тогда рисуем правую линию
                _painter.drawLine(_hx*(x+1),
                                  _hy*y,
                                  _hx*(x+1),
                                  _hy*(y+1));
            }
            if((y != (maze.get_size().y-1)) && (maze.get_bottom_mask()[y] & x_mask)){
                // тогда рисуем нижнюю линию
                _painter.drawLine(_hx*x,
                                  _hy*(y+1),
                                  _hx*(x+1),
                                  _hy*(y+1));
            }
        }
    }
}
//===========================================================================
void MyFrame::drawSolution(){
    double _hx = (double)width() /(double)maze.get_size().x;
    double _hy = (double)height()/(double)maze.get_size().y;

    int cur_x = maze.get_stop().x;  // начинаем с ПОСЛЕДНЕЙ точки
    int cur_y = maze.get_stop().y;

    QPainter _painter(this);
    _painter.setPen(QPen(SOLUTION_COLOR,SOLUTION_WIDTH,Qt::SolidLine));
    for(auto [dir_x, dir_y] : maze.get_solution()){
        _painter.drawLine(_hx*((double) cur_x          + 0.5),
                          _hy*((double) cur_y          + 0.5),
                          _hx*((double)(cur_x + dir_x) + 0.5),
                          _hy*((double)(cur_y + dir_y) + 0.5));
        cur_x += dir_x;
        cur_y += dir_y;
    }
}
//===========================================================================
void MyFrame::drawStart(){
    double _hx = (double)width() /(double)maze.get_size().x;
    double _hy = (double)height()/(double)maze.get_size().y;

    double dx = ((double)maze.get_start().x + 0.5)*_hx;
    double dy = ((double)maze.get_start().y + 0.5)*_hy;

    double dh = (_hx<_hy ? _hx : _hy)/4.0;

    QPainter _painter(this);
    _painter.setPen(QPen(Qt::NoPen));
    _painter.setBrush(QBrush(START_COLOR,Qt::SolidPattern));
    _painter.drawEllipse((int)(dx-dh),
                         (int)(dy-dh),
                         (int)dh*2,
                         (int)dh*2);
}
//===========================================================================
void MyFrame::drawStop(){
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
void MyFrame::mousePressEvent(QMouseEvent *event)
{
    double _hx = (double)width() /(double)maze.get_size().x;
    double _hy = (double)height()/(double)maze.get_size().y;

    int x = (int)((double)event->x()/_hx);
    int y = (int)((double)event->y()/_hy);

    if(modify_mode){
        if(event->buttons() & Qt::LeftButton){
            double dx = (double)event->x()/_hx;
            double dy = (double)event->y()/_hy;
            double round_x = std::round(dx);
            double round_y = std::round(dy);
            double delta_x = abs(round_x - dx);
            double delta_y = abs(round_y - dy);
            if     (delta_x <= MAZE_MODIFY_DELTA && round_x < maze.get_size().x){
                maze.invert_right(round_x-1,y);
                repaint();
            }
            else if(delta_y <= MAZE_MODIFY_DELTA && round_y < maze.get_size().y){
                maze.invert_bottom(x,round_y-1);
                repaint();
            }
        }
    }
    else{
        if(event->buttons() & Qt::LeftButton){
            if(maze.get_start().x != x || maze.get_start().y != y){
                maze.set_start(x,y);
                emit dots_changed(); //must have repaint
            }
        }
        else if(event->buttons() & Qt::RightButton){
            if(maze.get_stop().x != x || maze.get_stop().y != y){
                maze.set_stop(x,y);
                emit dots_changed(); //must have repaint
            }
        }
    }

    QFrame::mousePressEvent(event);     // передать управление стандартному обработчику
}
//===========================================================================
