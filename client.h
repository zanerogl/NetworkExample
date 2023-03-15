#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>//通信套接字
#include <QHostAddress>

class Client : public QWidget
{
    Q_OBJECT
public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
    void start(QString,int);
    QString log;

signals:
    void send(const QString&);
    void send(const std::string&);
    void close();
    void newMessage();
    void successConnect();

private:
    void ClientSend(const QString&);
    void ClientSend(const std::string&);
    void ClientClose();

private:
    QTcpSocket *tcpSocket;
};

#endif // CLIENT_H
