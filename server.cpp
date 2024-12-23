#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>

// Connection Data
char host[] = "ENTER YOUR IP";
int port = 55555;

// Starting Server
int server = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in serverAddr;
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(port);
serverAddr.sin_addr.s_addr = INADDR_ANY;
bind(server, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
listen(server, 5);

// Lists For Clients and Their Nicknames
std::vector<int> clients;
std::vector<std::string> nicknames;

// Sending Messages To All Connected Clients
void broadcast(const char* message) {
    for (int client : clients) {
        send(client, message, strlen(message), 0);
    }
}

// Handling Messages From Clients
void handle(int client) {
    while (true) {
        char message[1024];
        int bytesReceived = recv(client, message, 1024, 0);
        if (bytesReceived <= 0) {
            // Removing And Closing Clients
            int index = std::distance(clients.begin(), std::find(clients.begin(), clients.end(), client));
            close(client);
            clients.erase(clients.begin() + index);
            std::string nickname = nicknames[index];
            broadcast((nickname + " left!").c_str());
            nicknames.erase(nicknames.begin() + index);
            break;
        }
        // Broadcasting Messages
        broadcast(message);
    }
}

// Receiving / Listening Function
void receive() {
    while (true) {
        // Accept Connection
        int client;
        struct sockaddr_in clientAddr;
        socklen_t addrSize = sizeof(clientAddr);
        client = accept(server, (struct sockaddr*)&clientAddr, &addrSize);
        std::cout << "Connected with " << inet_ntoa(clientAddr.sin_addr) << std::endl;

        // Request And Store Nickname
        send(client, "NICK", strlen("NICK"), 0);
        char nickname[1024];
        recv(client, nickname, 1024, 0);
        nicknames.push_back(std::string(nickname));
        clients.push_back(client);

        // Print And Broadcast Nickname
        std::cout << "Nickname is " << nickname << std::endl;
        broadcast((std::string(nickname) + " joined!").c_str());
        send(client, "Connected to server!", strlen("Connected to server!"), 0);

        // Start Handling Thread For Client
        std::thread(handle, client).detach();
    }
}

int main() {
    std::cout << "Server is listening..." << std::endl;
    receive();
    return 0;
}





















































#include <iostream>
#include <winsock2.h>

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8090);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 5);

    int clientSocket = accept(serverSocket, nullptr, nullptr);

    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;

    closesocket(serverSocket);

    return 0;
}