//
// Created by zhb15 on 1/20/24.
//

#ifndef CPP_TCPSERVER_H
#define CPP_TCPSERVER_H

#include <tuple>
#include <QDialog>
#include <QList>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTcpServer>

class TcpServer: public QDialog {
    Q_OBJECT;
public:
    using clientData_type = std::tuple<QTcpSocket *, QStringList>;
#define GET_SOCK(var) std::get<QTcpSocket *>(var)
#define GET_RCV(var) std::get<QStringList>(var)

    explicit TcpServer(QWidget *parent = nullptr);
private:
    QLabel *serverAddressLabel;
    QLineEdit *serverAddressLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QLabel *connectNumberLabel;
    QLineEdit *connectNumberLineEdit;
    QLabel *rcvLabel;
    QTextEdit *rcvTextEdit;
    QLabel *sndLabel;
    QTextEdit *sndTextEdit;
    QPushButton *sndButton;
    QPushButton *startServerButton;
    QPushButton *closeServerButton;
    QGridLayout *mainLayout;

    QString globalRcvBuf;
    QString globalSndBuf;

    QHostAddress serverAddress;
    int port;
    QTcpServer *tcpServer;
    QList<clientData_type> clientList;
};

#endif //CPP_TCPSERVER_H