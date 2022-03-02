// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <thread>
#define PORT 8080

void ServerFunc(std::string myStorage, std::string theirStorage) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    for(int i = 0; i <1024; i++) buffer[i] = 0;
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while(true) {
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        
        std::string preface = "The message you sent was: ";
        std::string reply = preface + buffer;
        printf("Trying to send the message...\n");
        send(new_socket , reply.c_str() , strlen(reply.c_str()) , 0 );
        printf("Reply sent!\n");
    }
}
    
int main(int argc, char const *argv[])
{
    std::string One_to_Two;
    std::string Two_to_One;
    
    ServerFunc(One_to_Two, Two_to_One);
    
    std::thread firstUser(ServerFunc, One_to_Two, Two_to_One);
    std::thread secondUser(ServerFunc, Two_to_One, One_to_Two);
    
    firstUser.join();
    secondUser.join();
    return 0;
}

