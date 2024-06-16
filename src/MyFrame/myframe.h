#ifndef myframe_H
#define myframe_H

#include <QFrame>
#include "../MyMaze/mymaze.h"

#define MAZE_BORDER_COLOR   Qt::black
#define MAZE_FILL_COLOR     Qt::white
#define BLOCKED_CELL_COLOR  Qt::black
#define MAZE_BORDER_WIDTH   2

#define SOLUTION_COLOR      Qt::red
#define SOLUTION_WIDTH      4

#define START_COLOR         Qt::green
#define STOP_COLOR          Qt::red

class MyFrame : public QFrame
{
    Q_OBJECT

public:
    MyFrame(QWidget *parent = nullptr);

    MyMaze maze;

protected:
    virtual void paintEvent(QPaintEvent *) override;
    void drawMaze();
    void drawSolution();
    void drawStart();
    void drawStop();
};

#endif // myframe_H
