#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_server = new Server(this);
    const QString str = "sss";
    connect(ui->sendBtn, &QPushButton::clicked, this, [=]()
    {
        emit (m_server->send(ui->sendText->toPlainText()));
    });

    connect(ui->modeBox, static_cast<void(QComboBox::*)(const int)>(&QComboBox::currentIndexChanged), this, &Widget::modeSwitch);
    connect(m_server, &Server::newConnection, this, [=](){ui->sendBtn->setEnabled(true);});
    connect(m_server, &Server::newMessage, this, [=](){ui->receiveText->append(m_server->log);});

}

Widget::~Widget()
{
    delete ui;
}

void Widget::modeSwitch(int index)
{
    switch (index)
    {
    case 0:
        emit(m_server->close());
        ui->sendBtn->setEnabled(false);
        break;
    case 1:
        m_server->start(ui->portSpinBox->value());
        qDebug()<<ui->portSpinBox->value();
        break;
    case 2:
        qDebug() << __LINE__;
        ui->sendBtn->setEnabled(true);
        break;
    }
}

