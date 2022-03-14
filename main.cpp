//
// Created by dryav on 01.03.2022.
//

#include "tcpserver.hpp"
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

int timeIntervalInSeconds = 1000;

std::vector<char> vec(46000, 'A');
const std::string VERY_BIG_MESSAGE = std::string(vec.begin(), vec.end());

std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();
bool isFirst = true;
int count = 0;

std::vector<bool> bitsReceive;

int startClient() {
    TCPSocket tcpSocket([](int errorCode, std::string errorMessage) {
        cout << "Socket creation error:" << errorCode << " : " << errorMessage << endl;
    });

    tcpSocket.onMessageReceived = [](string message) {
        std::cout << "time : " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - lastTime).count() << std::endl;
        lastTime = std::chrono::steady_clock::now();
    };

    tcpSocket.onSocketClosed = [](int errorCode) {
        cout << "Connection closed: " << errorCode << endl;
    };

    tcpSocket.Connect("localhost", 8888, [&] {
                          cout << "Connected to the server successfully." << endl;

                          isFirst = true;
                          // Send String:
                          for (int i = 0; i < 200; i++) {
                              tcpSocket.Send(VERY_BIG_MESSAGE);
                              usleep(1);
                          }
                      },
                      [](int errorCode, std::string errorMessage) {
                          // CONNECTION FAILED
                          cout << errorCode << " : " << errorMessage << endl;
                      });

    string input;
    getline(cin, input);
    while (input != "exit") {
        getline(cin, input);
    }

    tcpSocket.Close();

    return 0;
}

std::string secretMessage = "1";
std::vector<bool> bitsOfMessage;
std::vector<std::string> buffer;
int currentBit = 0;

std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();


void sendBuffered(const std::string &message, TCPSocket *newClient) {
    buffer.push_back(message);

    int j = 0;
    if(buffer.size() == bitsOfMessage.size()) {
        for(int i = 0; i < bitsOfMessage.size(); i++) {
            if(bitsOfMessage[i]) {
                newClient->Send(buffer[j]);
                usleep(1);
                j++;
            } else {
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
        }
    } else {
        return;
    }

    while(j < buffer.size()) {
        newClient->Send(buffer[j]);
        j++;
    }
}

int startServer() {
    // Initialize server socket..
    TCPServer tcpServer;
    isFirst = true;
    // When a new client connected:
    tcpServer.onNewConnection = [&](TCPSocket *newClient) {
        cout << "New client: [";
        cout << newClient->remoteAddress() << ":" << newClient->remotePort() << "]" << endl;

        newClient->onMessageReceived = [newClient](string message) {
//            cout << newClient->remoteAddress() << ":" << newClient->remotePort() << " => " << message << endl;
                sendBuffered(VERY_BIG_MESSAGE, newClient);
        };

        // If you want to use raw bytes
        /*
        newClient->onRawMessageReceived = [newClient](const char* message, int length) {
            cout << newClient->remoteAddress() << ":" << newClient->remotePort() << " => " << message << "(" << length << ")" << endl;
            newClient->Send("OK!");
        };
        */

        newClient->onSocketClosed = [newClient](int errorCode) {
            cout << "Socket closed:" << newClient->remoteAddress() << ":" << newClient->remotePort() << " -> "
                 << errorCode << endl;
            cout << flush;
        };
    };

    tcpServer.Bind(8888, [](int errorCode, string errorMessage) {
        cout << errorCode << " : " << errorMessage << endl;
    });

    // Start Listening the server.
    tcpServer.Listen([](int errorCode, string errorMessage) {
        cout << errorCode << " : " << errorMessage << endl;
    });

    string input;
    getline(cin, input);
    while (input != "exit") {
        getline(cin, input);
    }

    tcpServer.Close();

    return 0;
}

int main() {
    for (auto ch: secretMessage) {
        for (int i = 0; i < 8; i++) {
            bitsOfMessage.push_back(ch & (1 << i));
            std::cout << ((ch & (1 << i)) ? "true" : "false") << std::endl;
        }
    }

    startServer();
//    startClient();
    return 0;
}
