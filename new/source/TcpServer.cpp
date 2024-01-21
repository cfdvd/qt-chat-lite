//
// Created by zhb15 on 1/20/24.
//

#include "TcpServer.h"

TcpServer::TcpServer(QWidget *parent): QDialog(parent) {
    setWindowTitle(QObject::tr("Tcp Server"));

    serverAddressLabel = new QLabel(QObject::tr("Server Address"));
    serverAddressLineEdit = new QLineEdit();
    auto *serverAddressLayout = new QHBoxLayout();
    serverAddressLayout->addWidget(serverAddressLabel);
    serverAddressLayout->addWidget(serverAddressLineEdit);
    portLabel = new QLabel(QObject::tr("Port"));
    portLineEdit = new QLineEdit();
    auto *portLayout = new QHBoxLayout();
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portLineEdit);
    connectNumberLabel = new QLabel(QObject::tr("Client Connect Number: "));
    connectNumberLineEdit = new QLineEdit();
    auto *connectNumberLayout = new QHBoxLayout();
    connectNumberLayout->addWidget(connectNumberLabel);
    connectNumberLayout->addWidget(connectNumberLineEdit);

    rcvLabel = new QLabel(QObject::tr("Received Message"));
    rcvTextEdit = new QTextEdit();
    sndLabel = new QLabel(QObject::tr("Send Message"));
    sndTextEdit = new QTextEdit();
    sndButton = new QPushButton(QObject::tr("Send !"));
    auto *sndLayout = new QVBoxLayout();
    sndLayout->addWidget(sndLabel);
    sndLayout->addWidget(sndButton);
    startServerButton = new QPushButton(QObject::tr("Open Server !"));
    closeServerButton = new QPushButton(QObject::tr("Close Server !"));
    auto *startCloseButtonLayout = new QHBoxLayout();
    startCloseButtonLayout->addWidget(startServerButton);
    startCloseButtonLayout->addWidget(closeServerButton);

    mainLayout = new QGridLayout(this);
    mainLayout->addLayout(serverAddressLayout, 0, 0, 1, 2);
    mainLayout->addLayout(portLayout, 1, 0);
    mainLayout->addLayout(connectNumberLayout, 1, 1);
    mainLayout->addWidget(rcvLabel, 2, 0);
    mainLayout->addWidget(rcvTextEdit, 2, 1);
    mainLayout->addLayout(sndLayout, 3, 0);
    mainLayout->addWidget(sndTextEdit, 3, 1);
    mainLayout->addLayout(startCloseButtonLayout, 4, 0, 1, 2);

    tcpServer = new QTcpServer(this);
    QObject::connect(tcpServer, &QTcpServer::newConnection, [this]() {
        while(this->tcpServer->hasPendingConnections()) {
            auto *clientSocket = tcpServer->nextPendingConnection();
            clientList.push_back(clientData_type{clientSocket, QString{}});

            QObject::connect(clientSocket, &QTcpSocket::readyRead, [this, clientSocket]() {
                if(0 >= clientSocket->bytesAvailable()) { return; }
                auto iter = std::find_if(clientList.begin(), clientList.end(), [&clientSocket](const auto &elem) {
                    return GET_SOCK(elem)->socketDescriptor() == clientSocket->socketDescriptor();
                });
                auto &readBuf = GET_RCV(*iter);
                readBuf.push_back(QString{clientSocket->readAll()});
                QString rcvMsg = QString("[%1:%2]: \n%3\n")
                                            .arg(clientSocket->peerAddress().toString())
                                            .arg(clientSocket->peerPort())
                                            .arg(readBuf.back());
                globalRcvBuf.append(rcvMsg);

                for(auto &elem: clientList) {
                    GET_SOCK(elem)->write(rcvMsg.toStdString().c_str(), rcvMsg.size());
                }

                rcvTextEdit->setText(globalRcvBuf);
            });

            QObject::connect(clientSocket, &QTcpSocket::disconnected,[this, clientSocket]() {
                clientSocket->deleteLater();
                auto iter = std::find_if(clientList.begin(), clientList.end(), [&clientSocket](const auto &elem) {
                    return std::get<0>(elem)->socketDescriptor() == clientSocket->socketDescriptor();
                });
                clientList.removeOne(*iter);

                serverAddressLineEdit->setText(tcpServer->serverAddress().toString());
                portLineEdit->setText(QString::number(tcpServer->serverPort()));
                connectNumberLineEdit->setText(QString::number(clientList.count()));
            });

            serverAddressLineEdit->setText(tcpServer->serverAddress().toString());
            portLineEdit->setText(QString::number(tcpServer->serverPort()));
            connectNumberLineEdit->setText(QString::number(clientList.count()));
        }
    });

    QObject::connect(startServerButton, &QPushButton::clicked, [this]() {
        if(this->tcpServer->isListening()) { return ; }
        QString tmpAddress = this->serverAddressLineEdit->text();
        this->serverAddress = tmpAddress.isEmpty() ? QHostAddress::AnyIPv4 : QHostAddress{tmpAddress};
        QString tmpPort = this->portLineEdit->text();
        this->port = tmpPort.isEmpty() ? 8080 : tmpAddress.toInt();
        this->tcpServer->listen(serverAddress, port);

        serverAddressLineEdit->setText(tcpServer->serverAddress().toString());
        portLineEdit->setText(QString::number(tcpServer->serverPort()));
        connectNumberLineEdit->setText(QString::number(clientList.count()));
    });

    QObject::connect(closeServerButton, &QPushButton::clicked, [this]() {
        tcpServer->close();
        for(auto &iter: clientList) {
            std::get<0>(iter)->disconnectFromHost();
            if(QAbstractSocket::UnconnectedState != std::get<0>(iter)->state()) {
                std::get<0>(iter)->abort();
            }
        }

        serverAddressLineEdit->setText(tcpServer->serverAddress().toString());
        portLineEdit->setText(QString::number(tcpServer->serverPort()));
        connectNumberLineEdit->setText(QString::number(clientList.count()));
    });

    QObject::connect(sndButton, &QPushButton::clicked, [this]() {
        foreach(auto &iter, clientList) {
            auto &client = GET_SOCK(iter);
            client->write(QByteArray{sndTextEdit->toPlainText().toUtf8()});
        }
    });
}