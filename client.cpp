#include "client.h"

Client::Client(QWidget *parent) : QWidget{parent}
{
    tcpSocket = nullptr;
    log.clear();

//    connect(this,SIGNAL(send(const QString&)),this,SLOT(ClientSend(const QString&)));
//    connect(this,SIGNAL(send(const std::string&)),this,SLOT(ClientSend(const std::string&)));
//    connect(this,SIGNAL(close()),this,SLOT(ClientClose()));

    connect(this, static_cast<void(Client::*)(const QString&)>(&Client::send), this, static_cast<void(Client::*)(const QString&)>(&Client::ClientSend));
    connect(this, static_cast<void(Client::*)(const std::string&)>(&Client::send), this, static_cast<void(Client::*)(const std::string&)>(&Client::ClientSend));
    connect(this, &Client::close, this, &Client::ClientClose);
}

Client::~Client()
{
    if(tcpSocket == nullptr)
    {
        return;
    }
    //主动和客户端断开连接
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    delete tcpSocket;
    tcpSocket=nullptr;
}

void Client::start(QString ip, int port)
{
    if(tcpSocket!=nullptr)
    {
        emit this->close();
    }
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress(ip),port);
    connect(tcpSocket, &QTcpSocket::connected,[=]()
    {
        log.clear();
        qDebug()<<"成功和服务器建立连接";
        log.append("成功和服务器建立连接");
        emit (this->successConnect());
    });
    //接收数据
    connect(tcpSocket,&QTcpSocket::readyRead,[=]()
    {
        //获取对方发送的内容
        QByteArray arry = tcpSocket->readAll();
        log.clear();
        //显示在console
        qDebug()<<arry;
        log.append(arry);
        emit (this->newMessage());
    });
}

void Client::ClientSend(const QString &data)
{
    if(tcpSocket==nullptr||data=="") return;
    log.clear();
    QByteArray arry;
    arry.resize(data.size());

    memcpy(arry.data(),data.toUtf8().data(),data.size());

    tcpSocket->write(arry);
}

void Client::ClientSend(const std::string &data)
{
    this->ClientSend(QString::fromStdString(data));
}

void Client::ClientClose()
{
    log.clear();
    if(tcpSocket != nullptr)
    {
        tcpSocket->close();
        delete tcpSocket;
        tcpSocket=nullptr;
        log.append("client closed");
    }
}
