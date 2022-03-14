//
// Created by dryav on 14.03.2022.
//

#ifndef COVERTCHANNELS_CHATCLIENT_H
#define COVERTCHANNELS_CHATCLIENT_H

#include "tcpsocket.hpp"
#include <iostream>

class ChatClient {
private:
    TCPSocket client;

    std::function<void(std::string message)> onMessage;
public:

    ChatClient(std::string address, uint16_t port);

    void sendMessage(std::string message);

    void setOnMessage(std::function<void(std::string)> onMessageHandler);

    ~ChatClient();

};


#endif //COVERTCHANNELS_CHATCLIENT_H
