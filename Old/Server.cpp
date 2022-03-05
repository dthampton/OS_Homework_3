// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <thread>
#include<signal.h>
#define PORT 8080

std::string One_to_Two;
std::string Two_to_One;
bool mail[2] = {0};

void sig_handler(int signo)
{
  if (signo == SIGINT)
    printf(" Received SIGINT\n");
}

std::string convertToString(char* buffer, int size) {
    std::string holder = "";
    for(int i = 0; i < size; i++) holder = holder + buffer[i];
    return holder;
}

void sendMail(int new_socket, int position) {
    std::string myMessage;
    std::string preface;
    
    printf("%i has sendMail running!\n", position);
    if (position == 0) std::string preface = "The message Client 0 sent was: ";
    if (position == 1) std::string preface = "The message Client 1 sent was: ";
    std::string reply;
    while(true) {
        if (position == 0) myMessage = One_to_Two;
        else myMessage = Two_to_One;
        
        if(mail[position] == true) {
            mail[position] = false;
            reply = preface + myMessage;
            printf("I want to send %i the message '%s'.\n", position, reply.c_str());
            send(new_socket , reply.c_str() , reply.size() , 0 );
            printf("Message sent!\n\n");
        }
    }
}

void getMail(int new_socket, int position) {
    printf("%i has getMail running!\n", position);
    int valread;
    std::string theirMessage;
    std::string returnMessage = "Got the message!\n";
    char buffer[1024] = {0};
    
    while(true) {  
        if (position == 1) theirMessage = One_to_Two;
        else theirMessage = Two_to_One;
        for (int i = 0; i < 1024; i++) buffer[i] = 0;
        valread = read( new_socket , buffer, 1024);
        theirMessage = convertToString(buffer, 1024);
        
        if (position == 1) {One_to_Two = theirMessage; sleep(1); mail[0] = true;}
        else {Two_to_One = theirMessage; sleep(1); mail[1] = true;}
        
        printf("Their message is :%s\n", theirMessage.c_str());
        send(new_socket , returnMessage.c_str() , returnMessage.size() , 0 );
    }
}

void ServerFunc(std::string myMessage, std::string theirMessage, int position, bool mail[2]) {
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
    printf("%i is starting the threads!\n", position);
    
    std::thread get(getMail, new_socket, position);
    std::thread send(sendMail, new_socket, position);
    
    get.join();
    send.join();
//    while(true) {
//        for (int i = 0; i < 1024; i++) buffer[i] = 0;
//        valread = read( new_socket , buffer, 1024);
//        theirMessage = convertToString(buffer, 1024);
//        if(position == 0) mail[1] = true;
//        if(position == 1) mail[0] = true;
//        printf("%s\n",buffer );
//        send(new_socket , returnMessage.c_str() , returnMessage.size() , 0 );
//        
//        if(mail[position] == true) {
//            mail[position] = false;
//            std::string preface = "The message someone sent was: ";
//            std::string reply = preface + myMessage;
//            printf("Trying to send the message...\n");
//            send(new_socket , myMessage.c_str() , myMessage.size() , 0 );
//            printf("Reply sent!\n");
//        }
//    }
}
    
int main(int argc, char const *argv[])
{
    if (signal(SIGINT, sig_handler) == SIG_ERR) printf("\ncan't catch SIGINT\n");
    
    std::cout << "Starting the threads.\n";
    std::thread firstUser(ServerFunc, One_to_Two, Two_to_One, 0, mail);
    std::thread secondUser(ServerFunc, Two_to_One, One_to_Two, 1, mail);
    
    firstUser.join();
    secondUser.join();
    return 0;
}

