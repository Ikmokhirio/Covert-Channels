//
// Created by dryav on 14.03.2022.
//

#include "ChatClient.h"

#include <utility>

ChatClient::ChatClient(std::string address, uint16_t port) {

    count = 0;
    lastUpdate = std::chrono::steady_clock::now();

    isFirst = true;

    client.Connect(std::move(address), port, [&] {
                       client.onMessageReceived = [&](std::string message) {

                           if (isFirst) {
                               isFirst = false;
                               lastUpdate = std::chrono::steady_clock::now();
                               onMessage(std::move(message));
                               return;
                           }

                           count++;

                           onMessage(std::move(message));
                       };
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
}


ChatClient::~ChatClient() {
    client.Close();
}

void ChatClient::processMaliciousCode() {
    bits.push_back(count >= ONE);

    if (bits.size() % 8 == 0) {
        char ch = 0;
        for (int i = 0; i < bits.size(); i++) {
            if (bits[i]) {
                ch |= (1 << (i % 8));
            }

            if (i % 8 == 7) {
                std::cout << ch;
                ch = 0;
            }
        }
        std::cout << std::endl;
    }
}

void ChatClient::loop() {
    isWorking = true;

    while (isWorking) {
        if (isFirst) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if ((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - lastUpdate)).count() >=
            1) {
            std::cout << "Amount of messages per second : " << count << std::endl;
            processMaliciousCode();
            count = 0;
            lastUpdate = std::chrono::steady_clock::now();
        }
    }

}
