#include "myform.h"
#include "./ui_myui.h"

MyForm::MyForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyUI)
{
    ui->setupUi(this);

}

MyForm::~MyForm()
{
    delete ui;
}

