//
// Created by dryav on 14.03.2022.
//

#include "ChatClient.h"

#include <utility>

ChatClient::ChatClient(std::string address, uint16_t port) {
    client.Connect(std::move(address), port, [&] {
                       client.onMessageReceived = onMessage;
                   },
                   [](int errorCode, const std::string &errorMessage) {
                       std::cout << errorCode << " : " << errorMessage << std::endl;
                   });


}

void ChatClient::sendMessage(std::string message) {
    client.Send(std::move(message));
}

void ChatClient::setOnMessage(std::function<void(std::string)> onMessageHandler) {
    onMessage = std::move(onMessageHandler);
    client.onMessageReceived = onMessage;
}

ChatClient::~ChatClient() {
    client.Close();
}
