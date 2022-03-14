#include <iostream>
#include "ChatClient.h"
#include "ChatServer.h"

int main() {

    ChatServer server(25565);
    server.setOnMessage([](const std::string& message) {
        std::cout << message << std::endl;
    });

    ChatClient client("127.0.0.1", 25565);
    client.setOnMessage([](const std::string& message) {
        std::cout << message << std::endl;
    });


    return 0;
}