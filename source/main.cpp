//
// Created by zhb15 on 1/20/24.
//

#include <QApplication>


#include "TcpServer.h"
#include "TcpClient.h"

template <typename... T>
inline __attribute__((__always_inline__)) void ignore_unused(T&&...) {}

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    TcpClient tcpClient;
    tcpClient.show();
//    TcpServer tcpServer;
//    tcpServer.show();

    return QApplication::exec();
}
