//
// Created by dryav on 15.03.2022.
//
#include <iostream>
#include <signal.h>
#include "ChatClient.h"

ChatClient *client;

void intHandler(int code) {
    client->stop();
}

int main() {
    signal(SIGINT, intHandler);
    signal(SIGTERM, intHandler);

    client = new ChatClient("127.0.0.1", 25565);
    client->setOnMessage([](const std::string &message) {
        if (message != "EMPTY") {
            std::cout << message << std::endl;
        }
    });

    std::thread clientLoop([&]() {
        client->loop();
    });

    clientLoop.join();
}
