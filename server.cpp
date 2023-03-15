#include "server.h"

Server::Server(QWidget *parent) : QWidget(parent)
{
    tcpSocket = nullptr;
    tcpServer = nullptr;

    log.clear();

    /*qt4*/
//    connect(this,SIGNAL(send(const QString&)),this,SLOT(serverSend(const QString&)));
//    connect(this,SIGNAL(send(const std::string&)),this,SLOT(serverSend(const std::string&)));
//    connect(this,SIGNAL(close()),this,SLOT(serverClose()));

    /*qt5*/
    connect(this, static_cast<void(Server::*)(const QString&)>(&Server::send), this, static_cast<void(Server::*)(const QString&)>(&Server::serverSend));
    connect(this, static_cast<void(Server::*)(const std::string&)>(&Server::send), this, static_cast<void(Server::*)(const std::string&)>(&Server::serverSend));
    connect(this, &Server::close, this, &Server::serverClose);
}

Server::~Server()
{
    if(tcpSocket == nullptr)
    {
        return;
    }
    //主动和客户端断开连接
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    tcpSocket = nullptr;
}

void Server::start(int port)
{
    if (tcpServer == nullptr)
    {
        tcpServer = new QTcpServer(this);
    }
    else
    {
        tcpServer->close();
        delete tcpServer;
        tcpServer = new QTcpServer(this);
    }
    tcpServer->listen(QHostAddress::Any, port);
    qDebug()<<tcpServer->isListening();
    log.clear();
    log.append("tcpServer started");

    connect(tcpServer,&QTcpServer::newConnection,[=]()
    {
        //取出建立好连接的套接字
        tcpSocket = tcpServer->nextPendingConnection();
        //获取对方的IP和端口
        QString ip = tcpSocket->peerAddress().toString();
        qint16 port = tcpSocket->peerPort();
        log.clear();
        QString temp = QString("[%1：%2]:成功连接").arg(ip).arg(port);
        qDebug()<<temp;
        log.append(temp);
        emit this->newConnection();
        //接收
        connect(tcpSocket,&QTcpSocket::readyRead,[=]()
        {
            log.clear();
            //从通讯套接字中取出内容
            QByteArray array = tcpSocket->readAll();
            //使用settext可以将以前的进行覆盖，使用append会在后面添加
            qDebug()<<"size:"<<array.size();
            int i;
            for(i = 0 ; i<array.size(); i++)
            {
                if(array[i]!='\0')break;
            }
            array.remove(0, i); //最后一位
            log.append(array);
            emit this->newMessage();
        });
    });
}

bool Server::operator!()
{
    return (tcpServer!=nullptr);
}

void Server::serverSend(const QString &data)
{
    log.clear();
    if(data==""){
        qDebug()<<"cannot send empty text";
        log.append("cannot send empty text");
        return;
    }
    if(tcpSocket== nullptr)
    {
        qDebug()<<"cannot find client";
        log.append("cannot find client");
        return;
    }
    //给对方发送数据，使用套接字是tcpSocket
    tcpSocket->write( data.toUtf8().data());
}

void Server::serverSend(const std::string &data)
{
    this->serverSend(QString::fromStdString(data));
}

void Server::serverClose()
{
    log.clear();
    if (tcpServer != nullptr)
    {
        tcpServer->close();
        delete tcpServer;
        tcpServer=nullptr;
        log.append("tcpServer closed");
    }
}
