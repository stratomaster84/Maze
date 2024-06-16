#include <QMessageBox>
#include "myform.h"
#include "./ui_myui.h"

//===========================================================================
MyForm::MyForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyUI)
{
    ui->setupUi(this);
    ui->width_spin->setMaximum(BIT_DEPTH);
    ui->height_spin->setMaximum(BIT_DEPTH);
    ui->width_spin->setMinimum(MIN_MAZE_SIZE);
    ui->height_spin->setMinimum(MIN_MAZE_SIZE);

    connect(ui->radio_resolve,SIGNAL(clicked()),this,SLOT(radio_resolve_but_clicked()));
    connect(ui->radio_maze,SIGNAL(clicked()),this,SLOT(radio_maze_but_clicked()));

    connect(ui->width_spin,SIGNAL(valueChanged(int)),this,SLOT(maze_resized()));
    connect(ui->height_spin,SIGNAL(valueChanged(int)),this,SLOT(maze_resized()));

    connect(ui->random_but,SIGNAL(clicked()),this,SLOT(random_maze()));
    connect(ui->ellers_but,SIGNAL(clicked()),this,SLOT(ellers_maze()));

    connect(ui->allow_diag_but,SIGNAL(stateChanged(int)),this,SLOT(allow_diagonal_step(int)));
    connect(ui->show_solution_but,SIGNAL(clicked()),this,SLOT(show_solution_clicked()));
    connect(ui->maze_frame,SIGNAL(dots_changed()),this,SLOT(show_solution_clicked()));
}
//===========================================================================
MyForm::~MyForm()
{
    delete ui;
}
//===========================================================================
void MyForm::radio_resolve_but_clicked()
{
    ui->size_box->setEnabled(false);
    ui->random_but->setEnabled(false);
    ui->ellers_but->setEnabled(false);
        ui->maze_frame->modify_mode = false;
    ui->show_solution_but->setEnabled(true);
    ui->allow_diag_but->setEnabled(true);
}
//===========================================================================
void MyForm::radio_maze_but_clicked()
{
    ui->size_box->setEnabled(true);
    ui->random_but->setEnabled(true);
    ui->ellers_but->setEnabled(true);
        ui->maze_frame->modify_mode = true;
    ui->show_solution_but->setEnabled(false);
        ui->show_solution_but->setChecked(false);
        ui->maze_frame->maze.clear_solution();
        ui->maze_frame->repaint();
    ui->show_solution_but->setEnabled(false);
    ui->allow_diag_but->setEnabled(false);
}
//===========================================================================
void MyForm::maze_resized()
{
    ui->maze_frame->maze.resize_maze(ui->width_spin->value(),ui->height_spin->value());
    ui->maze_frame->repaint();
}
//===========================================================================
void MyForm::random_maze()
{
    ui->maze_frame->maze.set_random_maze();
    ui->maze_frame->repaint();
}
//===========================================================================
void MyForm::ellers_maze()
{
    ui->maze_frame->maze.set_ellers_maze();
    ui->maze_frame->repaint();
}
//===========================================================================
void MyForm::allow_diagonal_step(int state)
{
    ui->maze_frame->maze.set_diag_step(state);
    ui->maze_frame->repaint();
}
//===========================================================================
void MyForm::show_solution_clicked()
{
    if(!ui->show_solution_but->isChecked()){
        ui->maze_frame->maze.clear_solution();
        ui->maze_frame->repaint();
        return;
    }

    QMessageBox msgBox;
    int err = ui->maze_frame->maze.resolve_maze();
    switch (err)
    {
        case 0:     // OK
            break;
        case 1:     // NO START POINT
            ui->show_solution_but->setChecked(false);
            msgBox.setText("Установите точку старта!");
            msgBox.exec();
            break;
        case 2:     // NO STOP POINT
            ui->show_solution_but->setChecked(false);
            msgBox.setText("Установите точку финиша!");
            msgBox.exec();
            break;
        case 3:     // NO SOLUTIONS
            ui->show_solution_but->setChecked(false);
            msgBox.setText("Нет решений!");
            msgBox.exec();
            break;
        default:
            break;
    }
    ui->maze_frame->repaint();
}
//===========================================================================
