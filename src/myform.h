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
    ~MyForm();                              // деструктор обязателен для удаления GUI-объекта

public slots:
    void radio_resolve_but_clicked();       // слот обработки перехода в режим "Resolve Mode"
    void radio_maze_but_clicked();          // слот обработки перехода в режим "Modify Mode"

    void maze_resized();                    // слот обработки изменения размера лабиринта
    void random_maze();                     // слот обработки нажатия кнопки генерации случайного лабиринта
    void ellers_maze();                     // слот обработки нажатия кнопки генерации лабиринта по алгоритму Эллера

    void allow_diagonal_step(int state);    // слот обработки установки/снятия флага разрешения шага по диагонали
    void show_solution_clicked();           // слот обработки нажатия на флаг показа поиска решений

private:
    Ui::MyUI *ui;   // GUI-объект из Qt Designer
};
#endif // MYFORM_H
