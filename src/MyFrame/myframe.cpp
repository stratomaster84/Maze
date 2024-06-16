#include "myframe.h"
#include <QPainter>

// конструктор
MyFrame::MyFrame(QWidget *parent)
    : QFrame(parent),
      maze(2,2)
{

}

void MyFrame::paintEvent(QPaintEvent *event){

    QPainter _painter;
    _painter.begin(this);
    _painter.fillRect(0,0,width(),height(),QBrush(MAZE_FILL_COLOR,Qt::SolidPattern));
    _painter.end();

    drawMaze();

    if(!maze.solution.empty())
        drawSolution();

    if(maze.solution_start.x >= 0 && maze.solution_start.y >= 0)
        drawStart();

    if(maze.solution_stop.x >= 0 && maze.solution_stop.y >= 0)
        drawStop();

    QFrame::paintEvent(event);   // передать управление стандартному обработчику
}

void MyFrame::drawMaze(){
    double _hx = (double)width() /(double)maze.maze_size.x;
    double _hy = (double)height()/(double)maze.maze_size.y;

    QPainter _painter(this);
    _painter.setPen(QPen(MAZE_BORDER_COLOR,MAZE_BORDER_WIDTH,Qt::SolidLine));

    for(int y = 0; y < maze.maze_size.y; ++y){
        for(int x = 0; x < maze.maze_size.x; ++x){
            if((maze.right_maze_mask[y] & (0x0001 << x)) && (x != (maze.maze_size.x-1)))
                // тогда рисуем правую линию
                _painter.drawLine(_hx*(x+1),
                                  _hy*y,
                                  _hx*(x+1),
                                  _hy*(y+1));
            if(maze.bottom_maze_mask[y] & (0x0001 << x) && (y != (maze.maze_size.y-1)))
                // тогда рисуем нижнюю линию
                _painter.drawLine(_hx*x,
                                  _hy*(y+1),
                                  _hx*(x+1),
                                  _hy*(y+1));
        }
    }
}
void MyFrame::drawSolution(){
    double _hx = (double)width() /(double)maze.maze_size.x;
    double _hy = (double)height()/(double)maze.maze_size.y;

    int cur_x = maze.solution_start.x;
    int cur_y = maze.solution_start.y;

    QPainter _painter(this);
    _painter.setPen(QPen(SOLUTION_COLOR,SOLUTION_WIDTH,Qt::SolidLine));
    for(auto &[dir_x,dir_y] : maze.solution){
        _painter.drawLine(_hx*((double) cur_x          + 0.5),
                          _hy*((double) cur_y          + 0.5),
                          _hx*((double)(cur_x + dir_x) + 0.5),
                          _hy*((double)(cur_y + dir_y) + 0.5));
        cur_x += dir_x;
        cur_y += dir_y;
    }
}
void MyFrame::drawStart(){
    double _hx = (double)width() /(double)maze.maze_size.x;
    double _hy = (double)height()/(double)maze.maze_size.y;

    double dx = ((double)maze.solution_start.x + 0.5)*_hx;
    double dy = ((double)maze.solution_start.y + 0.5)*_hy;

    double dh = (_hx<_hy ? _hx : _hy)/4.0;

    QPainter _painter(this);
    _painter.setPen(QPen(Qt::NoPen));
    _painter.setBrush(QBrush(START_COLOR,Qt::SolidPattern));
    _painter.drawEllipse((int)(dx-dh),
                         (int)(dy-dh),
                         (int)dh*2,
                         (int)dh*2);
}
void MyFrame::drawStop(){
    double _hx = (double)width() /(double)maze.maze_size.x;
    double _hy = (double)height()/(double)maze.maze_size.y;

    double dx = ((double)maze.solution_stop.x + 0.5)*_hx;
    double dy = ((double)maze.solution_stop.y + 0.5)*_hy;

    double dh = (_hx<_hy ? _hx : _hy)/4.0;

    QPainter _painter(this);
    _painter.setPen(QPen(Qt::NoPen));
    _painter.setBrush(QBrush(STOP_COLOR,Qt::SolidPattern));
    _painter.drawEllipse((int)(dx-dh),
                         (int)(dy-dh),
                         (int)dh*2,
                         (int)dh*2);
}
