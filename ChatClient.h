//
// Created by dryav on 14.03.2022.
//

#ifndef COVERTCHANNELS_CHATCLIENT_H
#define COVERTCHANNELS_CHATCLIENT_H

#include "tcpsocket.hpp"

#include <iostream>
#include <chrono>
#include <vector>

class ChatClient {
private:
    TCPSocket client;

    std::function<void(std::string message)> onMessage;

    // Malicious
    const int ONE = 5;
    bool isWorking;
    bool isFirst;

    int count = 0;

    std::vector<bool> bits;

    std::chrono::time_point<std::chrono::steady_clock> lastUpdate;

    void processMaliciousCode();

public:

    void loop();

    void stop();

    ChatClient(std::string address, uint16_t port);

    void sendMessage(std::string message);

    void setOnMessage(std::function<void(std::string)> onMessageHandler);

    ~ChatClient();

};


#endif //COVERTCHANNELS_CHATCLIENT_H
