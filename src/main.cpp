#include "myform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyForm w;
    w.show();                   // показать основную форму
    return a.exec();
}
