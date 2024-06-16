#include "myform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyForm w;
    w.show();
    return a.exec();
}
