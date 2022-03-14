//
// Created by dryav on 14.03.2022.
//

#include "ChatServer.h"

#include <utility>

ChatServer::ChatServer(uint16_t port) {

    client = nullptr;
    bufferLock.unlock();
    currentBit = 0;

    server.Bind(port, [](int errorCode, const std::string &errorMessage) {
        std::cout << errorCode << " : " << errorMessage << std::endl;
    });

    server.Listen([](int errorCode, const std::string &errorMessage) {
        std::cout << errorCode << " : " << errorMessage << std::endl;
    });

    server.onNewConnection = [&](TCPSocket *newClient) {

        client = newClient;

        client->onMessageReceived = onMessage;

    };
}

ChatServer::~ChatServer() {
    server.Close();
}

void ChatServer::sendMessage(std::string message) {
    if (client == nullptr) {
        return;
    }

    bufferLock.lock();
    messageBuffer.push_back(message);
    bufferLock.unlock();
}

void ChatServer::setOnMessage(std::function<void(std::string)> onMessageHandler) {
    onMessage = std::move(onMessageHandler);
}

void ChatServer::loop() {
    isWorking = true;
    while (isWorking) {

        // Maybe 900 ms
        if ((SECRET_MESSAGE[currentBit / 8] >> (currentBit % 8)) & 1) {
            for (int i = 0; i < PACKETS_FOR_TRUE; i++) {

                if (messageBuffer.empty()) {
                    client->Send(EMPTY_MESSAGE);
                } else {
                    bufferLock.lock();
                    client->Send(messageBuffer[0]);
                    messageBuffer.erase(messageBuffer.begin());
                    bufferLock.unlock();
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(TIME_INTERVAL_IN_MS/PACKETS_FOR_TRUE));
            }
        } else {
            if (messageBuffer.empty()) {
                client->Send(EMPTY_MESSAGE);
            } else {
                bufferLock.lock();
                client->Send(messageBuffer[0]);
                bufferLock.unlock();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(TIME_INTERVAL_IN_MS));
        }

        if ((std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - lastUpdate)).count() >= 1) {

            currentBit++;
            if (currentBit == SECRET_MESSAGE.size() * 8) {
                currentBit = 0;
            }

            lastUpdate = std::chrono::steady_clock::now();
        }
    }
}

void ChatServer::stop() {
    isWorking = false;
}
