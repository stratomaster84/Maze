#ifndef myframe_H
#define myframe_H

#include <QFrame>
#include "../MyMaze/mymaze.h"

#define MAZE_BORDER_COLOR   Qt::black
#define MAZE_FILL_COLOR     Qt::white
#define MAZE_BORDER_WIDTH   2
#define MAZE_MODIFY_DELTA   0.2
#define MAZE_BLOCKED_COLOR  Qt::darkGray

#define SOLUTION_COLOR      Qt::red
#define SOLUTION_WIDTH      4

#define START_COLOR         Qt::green
#define STOP_COLOR          Qt::yellow

class MyFrame : public QFrame
{
    Q_OBJECT

public:
    MyFrame(QWidget *parent = nullptr);

    MyMaze maze;

    bool modify_mode;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

    void drawMaze();
    void drawSolution();
    void drawStart();
    void drawStop();

signals:
    void dots_changed();
};

#endif // myframe_H
