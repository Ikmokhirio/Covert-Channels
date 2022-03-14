//
// Created by dryav on 14.03.2022.
//

#ifndef COVERTCHANNELS_CHATSERVER_H
#define COVERTCHANNELS_CHATSERVER_H

#include "tcpserver.hpp"

#include <string>
#include <iostream>

class ChatServer {
private:
    TCPServer server;

    TCPSocket *client;

    std::function<void(std::string message)> onMessage;
public:
    explicit ChatServer(uint16_t port);

    void sendMessage(std::string message);

    void setOnMessage(std::function<void(std::string)> onMessageHandler);

    ~ChatServer();
};


#endif //COVERTCHANNELS_CHATSERVER_H
