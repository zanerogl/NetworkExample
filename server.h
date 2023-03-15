#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>  //监听套接字
#include <QTcpSocket> //通信套接字

class Server : public QWidget
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();
    void start(int);
    bool operator!();
    QString log;

signals:
    void send(const QString&);
    void send(const std::string&);
    void close();
    void newConnection();
    void newMessage();

private slots:
    void serverSend(const QString&);
    void serverSend(const std::string&);
    void serverClose();

private:
    QTcpServer *tcpServer = nullptr; //定义监听套接字tcpServer
    QTcpSocket *tcpSocket = nullptr; //定义通信套接字tcpSocket
};

#endif // SERVER_H
