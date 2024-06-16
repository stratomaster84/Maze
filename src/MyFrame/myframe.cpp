#include "myframe.h"

// конструктор
MyFrame::MyFrame(QWidget *parent)
    : QFrame(parent){

}

// обработчик события нажатия кнопки Shift+Enter на клавиатуре
/*void MyFrame::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Return && (event->modifiers() & Qt::ShiftModifier)){
        emit signalToQuerySlot();    // если нажата Shift+Enter - послать сигнал
    }
    else{
        QPlainTextEdit::keyPressEvent(event);   // иначе передать управление стандартному обработчику
    }
}*/
