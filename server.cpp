//
// Created by dryav on 15.03.2022.
//
#include <iostream>
#include <signal.h>
#include "ChatServer.h"

ChatServer *server;

bool isWorking;

void intHandler(int code) {
    server->stop();
    isWorking = false;
}

int main(int argc, char **argv) {

    isWorking = true;

    signal(SIGINT, intHandler);
    signal(SIGTERM, intHandler);

    if (argc < 2) {
        throw std::runtime_error("Pass secret message as first argument");
    }

    std::string secret = argv[1];
    std::cout << "Your secret message : " << secret << std::endl;

    server = new ChatServer(25565);
    server->setSecretMessage(secret);
    server->setOnMessage([](const std::string &message) {
        std::cout << message << std::endl;
    });


    std::thread loop([&]() {
        server->loop();
    });

    std::thread sending([&]() {
        std::string message = "Message from user";
        while (isWorking) {
            server->sendMessage(message);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    loop.join();
    sending.join();

    return 0;
}

