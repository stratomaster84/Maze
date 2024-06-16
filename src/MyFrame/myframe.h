#ifndef myframe_H
#define myframe_H

#include <QFrame>
//#include <QKeyEvent>

class MyFrame : public QFrame
{
    Q_OBJECT

public:
    MyFrame(QWidget *parent = nullptr);

protected:
    //virtual void keyPressEvent(QKeyEvent *event);   // обработчик события нажатия кнопки Shift+Enter на клавиатуре

Q_SIGNALS:
    //void signalToQuerySlot();    // сигнал для связывания нажатия кнопки на клавиатуре и обработчика запроса slotQuery
};

#endif // myframe_H
