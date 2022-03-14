#include <iostream>
#include <signal.h>
#include "ChatClient.h"
#include "ChatServer.h"

ChatServer *server;
ChatClient *client;

bool isWorking;

void intHandler(int code) {
    server->stop();
    client->stop();
    isWorking = false;
}

int main() {
    isWorking = true;

    signal(SIGINT, intHandler);
    signal(SIGTERM, intHandler);

    server = new ChatServer(25565);
    server->setOnMessage([](const std::string &message) {
        std::cout << message << std::endl;
    });

    client = new ChatClient("127.0.0.1", 25565);
    client->setOnMessage([](const std::string &message) {
        if (message != "EMPTY") {
            std::cout << message << std::endl;
        }
    });

    std::thread clientLoop([&]() {
        client->loop();
    });

    std::thread loop([&]() {
        server->loop();
    });

    std::thread sending([&]() {
        std::string message = "daskdlsakd;lsakd;lsakdlaskf;lakf;lakf;lsakfsalfkas;lfja;lfkjas;lkfsa;lkd;laskd;laskd;lsakd;";
        while (isWorking) {
            server->sendMessage(message);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    loop.join();
    sending.join();
    clientLoop.join();


    return 0;
}