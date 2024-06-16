#include "myform.h"
#include "./ui_myui.h"

//===========================================================================
MyForm::MyForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyUI)
{
    ui->setupUi(this);

    connect(ui->radio_resolve,SIGNAL(clicked()),this,SLOT(radio_resolve_but_clicked()));
    connect(ui->radio_maze,SIGNAL(clicked()),this,SLOT(radio_maze_but_clicked()));

    connect(ui->width_spin,SIGNAL(valueChanged(int)),this,SLOT(maze_resized()));
    connect(ui->height_spin,SIGNAL(valueChanged(int)),this,SLOT(maze_resized()));

    connect(ui->random_but,SIGNAL(clicked()),this,SLOT(random_maze()));

    connect(ui->resolve_but,SIGNAL(clicked()),this,SLOT(resolve_but()));

    connect(ui->allow_diag_but,SIGNAL(stateChanged(int)),this,SLOT(allow_diagonal_step(int)));
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
    ui->maze_frame->modify_mode = false;
    ui->resolve_but->setEnabled(true);
    ui->allow_diag_but->setEnabled(true);
}
//===========================================================================
void MyForm::radio_maze_but_clicked()
{
    ui->size_box->setEnabled(true);
    ui->random_but->setEnabled(true);
    ui->maze_frame->modify_mode = true;
    ui->resolve_but->setEnabled(false);
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
void MyForm::resolve_but()
{
    int err = ui->maze_frame->maze.resolve_maze();
    switch (err)
    {
        case 0:
            // OK
            ui->maze_frame->repaint();
            break;
        case 1:
            // NO START POINT
            break;
        case 2:
            // NO STOP POINT
            break;
        case 3:
            // NO SOLUTIONS
            break;
        default:
            break;
    }
}
//===========================================================================
void MyForm::allow_diagonal_step(int state)
{
    ui->maze_frame->maze.set_diag_step(state);
}
//===========================================================================
