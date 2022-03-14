#include <iostream>
#include "ChatClient.h"
#include "ChatServer.h"

int main() {

    ChatServer server(25565);
    server.setOnMessage([](const std::string &message) {
        std::cout << message << std::endl;
    });

    ChatClient client("127.0.0.1", 25565);
    client.setOnMessage([](const std::string &message) {
//        std::cout << message << std::endl;
    });

    std::string msg = "test";
    std::cout << "MESSAGE : ";
    for(int i = 0; i < 32; i++) {
        if((msg[i/8] >> (i%8)) & 1) {
            std::cout << "1";
        } else {
            std::cout << "0";
        }
    }
    std::cout << std::endl << std::endl;

    std::thread clientLoop([&]() {
        client.loop();
    });

    std::thread loop([&]() {
       server.loop();
    });

    std::thread sending([&]() {
        std::string message = "daskdlsakd;lsakd;lsakdlaskf;lakf;lakf;lsakfsalfkas;lfja;lfkjas;lkfsa;lkd;laskd;laskd;lsakd;";
        while(true) {
            server.sendMessage(message);
            std::this_thread::sleep_for(std::chrono::seconds (1));
        }
    });

    loop.join();
    sending.join();
    clientLoop.join();


    return 0;
}