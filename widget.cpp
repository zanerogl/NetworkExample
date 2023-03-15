#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_server = new Server(this);
    m_client = new Client(this);

    connect(ui->sendBtn, &QPushButton::clicked, this, &Widget::sendMessage);
    connect(ui->modeBox, static_cast<void(QComboBox::*)(const int)>(&QComboBox::currentIndexChanged), this, &Widget::modeSwitch);

    /*服务器*/
    connect(m_server, &Server::newConnection, this, [=]()
    {
        ui->receiveText->setText(m_server->log);
        ui->sendBtn->setEnabled(true);
    });
    connect(m_server, &Server::newMessage, this, [=](){ui->receiveText->append(m_server->log);});

    /*客户端*/
    connect(m_client, &Client::successConnect,this, [=]()
    {
        ui->receiveText->setText(m_server->log);
        ui->sendBtn->setEnabled(true);
        ui->sendBtn->setText("Send");
    });
    connect(m_client, &Client::newMessage,this, [=](){ui->receiveText->append(m_client->log);});
}

Widget::~Widget()
{
    delete ui;
}

void Widget::modeSwitch(int index)
{
    switch (index)
    {
    case 0:                                     //关闭模式
        emit(m_server->close());
        emit(m_client->close());
        ui->sendBtn->setEnabled(false);
        break;
    case 1:                                     //服务器模式
        emit(m_client->close());                //关闭客户端
        m_server->start(ui->portSpinBox->value());
        ui->sendBtn->setText("Send");
        ui->sendBtn->setEnabled(false);
        break;
    case 2:                                     //客户端模式
        emit(m_server->close());                //关闭服务器
        ui->sendBtn->setText("Connect");
        ui->sendBtn->setEnabled(true);
        break;
    }
}

void Widget::sendMessage()
{
    switch(ui->modeBox->currentIndex())
    {
    case 1:
        emit (m_server->send(ui->sendText->toPlainText()));
        break;
    case 2:
        if(ui->sendBtn->text() == "Connect")
        {
            m_client->start(ui->ipLineEdit->text(), ui->portSpinBox->value());
            qDebug() << __LINE__;
        }
        else
            emit (m_client->send(ui->sendText->toPlainText()));
        break;
    }
}

