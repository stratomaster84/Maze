#ifndef MYFORM_H
#define MYFORM_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MyUI; }
QT_END_NAMESPACE

class MyForm : public QWidget
{
    Q_OBJECT

public:
    MyForm(QWidget *parent = nullptr);
    ~MyForm();

public slots:
    void radio_resolve_but_clicked();
    void radio_maze_but_clicked();

    void maze_resized();
    void random_maze();
    void ellers_maze();

    void allow_diagonal_step(int state);
    void show_solution_clicked();

private:
    Ui::MyUI *ui;
};
#endif // MYFORM_H
