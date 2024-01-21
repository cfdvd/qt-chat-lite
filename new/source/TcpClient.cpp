//
// Created by zhb15 on 1/20/24.
//

#include "TcpClient.h"

TcpClient::TcpClient(QWidget *parent): QDialog(parent) {
    setWindowTitle(QObject::tr("Tcp Client"));

    serverAddressLabel = new QLabel(QObject::tr("Server Ip Address: "));
    serverAddressLineEdit = new QLineEdit();
    portLabel = new QLabel(QObject::tr("Port: "));
    portLineEdit = new QLineEdit();
    rcvLabel = new QLabel(QObject::tr("Received Message"));
    rcvTextEdit = new QTextEdit();
    sndLabel = new QLabel(QObject::tr("Send Message"));
    sndTextEdit = new QTextEdit();
    sndButton = new QPushButton(QObject::tr("Send !"));
    auto *sndLayout = new QVBoxLayout();
    sndLayout->addWidget(sndLabel);
    sndLayout->addWidget(sndButton);

    connectButton = new QPushButton(QObject::tr("Connect !"));
    disconnectButton = new QPushButton(QObject::tr("Disconnect !"));
    auto *connectLayout = new QHBoxLayout();
    connectLayout->addWidget(connectButton);
    connectLayout->addWidget(disconnectButton);

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(serverAddressLabel, 0, 0);
    mainLayout->addWidget(serverAddressLineEdit, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(rcvLabel, 2, 0);
    mainLayout->addWidget(rcvTextEdit, 2, 1);
    mainLayout->addLayout(sndLayout, 3, 0);
    mainLayout->addWidget(sndTextEdit, 3, 1);
    mainLayout->addLayout(connectLayout, 4, 0, 1, 2);

    clientSocket = new QTcpSocket(this);
    QObject::connect(connectButton, &QPushButton::clicked, [this]() {
        if(QAbstractSocket::ConnectedState == this->clientSocket->state()) { return ; }
        this->serverAddress = serverAddressLineEdit->text();
        this->port = portLineEdit->text().toInt();
        this->clientSocket->connectToHost(this->serverAddress, this->port);
    });
    QObject::connect(disconnectButton, &QPushButton::clicked, [this]() {
        if(QAbstractSocket::UnconnectedState == this->clientSocket->state()) { return ; }
        this->clientSocket->disconnectFromHost();
    });
    QObject::connect(clientSocket, &QTcpSocket::readyRead, [this]() {
        if(0 >= this->clientSocket->bytesAvailable()) { return ; }
        this->rcvBuf += this->clientSocket->readAll() + "\n";
        this->rcvTextEdit->setText(this->rcvBuf);
    });
    QObject::connect(sndButton, &QPushButton::clicked, [this]() {
        if(!this->clientSocket->isValid()) { return; }
        std::string tmp = sndTextEdit->toPlainText().toStdString();
        this->sndBuf += QString(tmp.c_str()) + QString("\n");
        this->clientSocket->write(tmp.c_str(), tmp.size());
    });
}