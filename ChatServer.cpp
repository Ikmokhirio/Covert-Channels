//
// Created by dryav on 14.03.2022.
//

#include "ChatServer.h"

#include <utility>

ChatServer::ChatServer(uint16_t port) {

    client = nullptr;

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

    client->Send(std::move(message));
}

void ChatServer::setOnMessage(std::function<void(std::string)> onMessageHandler) {
    onMessage = std::move(onMessageHandler);
}
