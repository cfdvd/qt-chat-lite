//
// Created by zhb15 on 1/20/24.
//

#ifndef CPP_TCPCLIENT_H
#define CPP_TCPCLIENT_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTcpSocket>
#include <QTcpServer>

class TcpClient: public QDialog {
    Q_OBJECT
public:
    explicit TcpClient(QWidget *parent = nullptr);
private:
    QLabel *serverAddressLabel;
    QLineEdit *serverAddressLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QLabel *rcvLabel;
    QTextEdit *rcvTextEdit;
    QLabel *sndLabel;
    QTextEdit *sndTextEdit;
    QPushButton *sndButton;
    QGridLayout *mainLayout;
    QPushButton *connectButton;
    QPushButton *disconnectButton;

    QString rcvBuf;
    QString sndBuf;

    int port;
    QString serverAddress;
    QTcpSocket *clientSocket;
};

#endif //CPP_TCPCLIENT_H