#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "server.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void modeSwitch(int index);

    void sendMessage();


private:
    Ui::Widget *ui;
    Server *m_server = nullptr;
    Client *m_client = nullptr;
};
#endif // WIDGET_H
