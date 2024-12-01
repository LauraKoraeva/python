#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>

std::string nickname;

void receive(SOCKET client) {
    while (true) {
        try {
            char message[1024];
            // Receive Message From Server
            // If 'NICK' Send Nickname
            recv(client, message, sizeof(message), 0);
            if (std::string(message) == "NICK") {
                send(client, nickname.c_str(), nickname.size(), 0);
            } else {
                std::cout << message << std::endl;
            }
        } catch (...) {
            // Close Connection When Error
            std::cout << "An error occurred!" << std::endl;
            closesocket(client);
            break;
        }
    }
}

void write(SOCKET client) {
    while (true) {
        std::string message;
        message = nickname + ": " + std::cin.get();
        send(client, message.c_str(), message.size(), 0);
    }
}

int main() {
    // Choosing Nickname
    std::cout << "Choose your nickname: ";
    std::cin >> nickname;

    // Connecting To Server
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }
    SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(55555);
    inet_pton(AF_INET, "ENTER YOUR IP SERVER ADDRESS", &addr.sin_addr);
    connect(client, (sockaddr*)&addr, sizeof(addr));

    // Starting Threads For Listening And Writing
    std::thread receive_thread(receive, client);
    std::thread write_thread(write, client);

    receive_thread.join();
    write_thread.join();

    closesocket(client);
    WSACleanup();

    return 0;
}