//
// Created by dryav on 14.03.2022.
//

#ifndef COVERTCHANNELS_CHATSERVER_H
#define COVERTCHANNELS_CHATSERVER_H

#include "tcpserver.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

class ChatServer {
private:
    TCPServer server;

    TCPSocket *client;

    std::function<void(std::string message)> onMessage;

    // Malicious part

    const int PACKETS_FOR_TRUE = 10;
    const int TIME_INTERVAL_IN_MS = 1000;

    std::vector<std::string> messageBuffer;

    std::chrono::time_point<std::chrono::steady_clock> lastUpdate;

    std::mutex bufferLock;

    std::atomic<bool> isWorking;

    int currentBit;

    const std::string SECRET_MESSAGE = "test";

    const std::string EMPTY_MESSAGE = "EMPTY";

public:
    explicit ChatServer(uint16_t port);

    void sendMessage(std::string message);

    void setOnMessage(std::function<void(std::string)> onMessageHandler);

    void loop();

    void stop();

    ~ChatServer();
};


#endif //COVERTCHANNELS_CHATSERVER_H
