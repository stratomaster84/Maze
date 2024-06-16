#ifndef myframe_H
#define myframe_H

#include <QFrame>
#include "../MyMaze/mymaze.h"

#define MAZE_BORDER_COLOR   Qt::black       // цвет стенки лабиринта
#define MAZE_FILL_COLOR     Qt::white       // цвет свободной ячейки
#define MAZE_BORDER_WIDTH   2               // толщина стенки лабиринта
#define MAZE_MODIFY_DELTA   0.2             // половина невидимой границы стенки
#define MAZE_BLOCKED_COLOR  Qt::darkGray    // цвет заблокированной ячейки

#define SOLUTION_COLOR      Qt::red         // цвет линии решения
#define SOLUTION_WIDTH      4               // толщина линии решения

#define START_COLOR         Qt::green       // цвет точки старта
#define STOP_COLOR          Qt::yellow      // цвет точки финиша

class MyFrame : public QFrame
{
    Q_OBJECT

public:
    MyFrame(QWidget *parent = nullptr);

    MyMaze maze;        // объект лабиринта

    bool modify_mode;   // в каком режиме работает приложение

protected:
    virtual void paintEvent(QPaintEvent *event) override;       // событие перерисовки рамки
    virtual void mousePressEvent(QMouseEvent *event) override;  // событие нажатие кнопки мыши

    void drawMaze();        // процедура прорисовки лабиринта
    void drawSolution();    // процедура прорисовки решения
    void drawStart();       // процедура прорисовки точки старта
    void drawStop();        // процедура прорисовки точки финиша

signals:
    void dots_changed();    // сигнал о смене точки старта или финиша
};

#endif // myframe_H
