#include <QMessageBox>
#include "myform.h"
#include "./ui_myui.h"

//===========================================================================
MyForm::MyForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyUI)      // создаём GUI-объект
{
    ui->setupUi(this);                              // рисуем GUI-объект
    ui->width_spin->setMaximum(BIT_DEPTH);          // устанавливаем минимально и максимально допустимые значения
    ui->height_spin->setMaximum(BIT_DEPTH);         // ширины и высоты лабиринта
    ui->width_spin->setMinimum(MIN_MAZE_SIZE);
    ui->height_spin->setMinimum(MIN_MAZE_SIZE);

// СОЕДИНЕНИЯ
    connect(ui->radio_resolve,SIGNAL(clicked()),this,SLOT(radio_resolve_but_clicked()));    // переход в "Resolve Mode"
    connect(ui->radio_maze,SIGNAL(clicked()),this,SLOT(radio_maze_but_clicked()));          // переход в "Modify Mode"
    connect(ui->width_spin,SIGNAL(valueChanged(int)),this,SLOT(maze_resized()));            // изменена ширина лабиринта
    connect(ui->height_spin,SIGNAL(valueChanged(int)),this,SLOT(maze_resized()));           // изменена высота лабиринта
    connect(ui->random_but,SIGNAL(clicked()),this,SLOT(random_maze()));         // кнопка генерации случайного лабиринта
    connect(ui->ellers_but,SIGNAL(clicked()),this,SLOT(ellers_maze()));         // кнопка генерации лабиринта Эллера
    connect(ui->allow_diag_but,SIGNAL(stateChanged(int)),
                                    this,SLOT(allow_diagonal_step(int)));   // изменился флаг разрешения шага по диагонали
    connect(ui->show_solution_but,SIGNAL(clicked()),this,SLOT(show_solution_clicked()));    // нажатие на флаг показа/скрытия решения лабиринта
    connect(ui->maze_frame,SIGNAL(dots_changed()),this,SLOT(show_solution_clicked()));      // отклик на сигнал от рамки о том, что...
                                                                                            // ...изменена точка старта или финиша
}
//===========================================================================
MyForm::~MyForm()
{
    delete ui;  // удаляем GUI-объект
}
//===========================================================================
void MyForm::radio_resolve_but_clicked()    // слот обработки перехода в режим "Resolve Mode"
{
    ui->maze_frame->modify_mode = false;    // снять режим "modify_mode" для нажатия кнопок мыши по рамке
    ui->size_box->setEnabled(false);        // запретить изменения размеров лабиринта
    ui->random_but->setEnabled(false);      // запретить генерацию случай
    ui->ellers_but->setEnabled(false);      // запретить генерацию лабиринта Эллера
    ui->show_solution_but->setEnabled(true);    // разрешить изменение флага показа решения
    ui->allow_diag_but->setEnabled(true);       // разрешить изменение флага разрешения диагональных шагов
}
//===========================================================================
void MyForm::radio_maze_but_clicked()       // слот обработки перехода в режим "Modify Mode"
{
    ui->maze_frame->modify_mode = true;     // установить режим "modify_mode" для нажатия кнопок мыши по рамке
    ui->size_box->setEnabled(true);         // разрешить изменения размеров лабиринта
    ui->random_but->setEnabled(true);       // разрешить генерацию случай
    ui->ellers_but->setEnabled(true);       // разрешить генерацию лабиринта Эллера
    ui->show_solution_but->setEnabled(false);   // запретить изменение флага показа решения
        ui->show_solution_but->setChecked(false);   // снять флаг показа решения
        ui->maze_frame->maze.clear_solution();      // удалить решение
        ui->maze_frame->repaint();                  // перерисовать рамку
    ui->allow_diag_but->setEnabled(false);  // разрешить изменение флага разрешения диагональных шагов
}
//===========================================================================
void MyForm::maze_resized()     // слот обработки изменения размера лабиринта
{
    ui->maze_frame->maze.resize_maze(ui->width_spin->value(),ui->height_spin->value());
    ui->maze_frame->repaint();
}
//===========================================================================
void MyForm::random_maze()      // слот обработки нажатия кнопки генерации случайного лабиринта
{
    ui->maze_frame->maze.set_random_maze();
    ui->maze_frame->repaint();
}
//===========================================================================
void MyForm::ellers_maze()      // слот обработки нажатия кнопки генерации лабиринта по алгоритму Эллера
{
    ui->maze_frame->maze.set_ellers_maze();
    ui->maze_frame->repaint();
}
//===========================================================================
void MyForm::allow_diagonal_step(int state)     // слот обработки установки/снятия флага разрешения шага по диагонали
{
    ui->maze_frame->maze.set_diag_step(state);
    ui->maze_frame->repaint();
}
//===========================================================================
void MyForm::show_solution_clicked()    // слот обработки нажатия на флаг показа поиска решений
{
    if(!ui->show_solution_but->isChecked()){    // если флаг снят
        ui->maze_frame->maze.clear_solution();  // очищаем решение
        ui->maze_frame->repaint();              // перерисовываем рамку
        return;                                 // выход
    }

    QMessageBox msgBox;
    int err = ui->maze_frame->maze.resolve_maze();  // пробуем нацти решение
    switch (err)                                    // обработчик ошибок
    {
        case 1:                                             // нет точки старта
            ui->show_solution_but->setChecked(false);       // снимаем флаг показа решения
            msgBox.setText("Установите точку старта!");     // выводим сообщение
            msgBox.exec();
            break;
        case 2:                                             // нет точки финиша
            ui->show_solution_but->setChecked(false);       // снимаем флаг показа решения
            msgBox.setText("Установите точку финиша!");     // выводим сообщение
            msgBox.exec();
            break;
        case 3:                                             // нет возможных решений
            ui->show_solution_but->setChecked(false);       // снимаем флаг показа решения
            msgBox.setText("Нет решений!");                 // выводим сообщение
            msgBox.exec();
            break;
        default:                                            // нет ошибок
            break;
    }
    ui->maze_frame->repaint();                              // перерисовываем рамку
}
//===========================================================================
