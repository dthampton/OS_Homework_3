// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread>
#define PORT 8080

void checkMail(int sock, char buffer[]) {
    int valread;
    while(true) {
        valread = read( sock , buffer, 1024);
        printf("%s\n",buffer );
    }
}

void sendMail(int sock) {
    std::string input;
    while(true) {
        std::getline(std::cin, input);
        std::cout << '\n';
        send(sock , input.c_str() , strlen(input.c_str()) , 0 );
    }
}
   
int main(int argc, char const *argv[])
{
        std::cout << "Trying to connect.  If this takes longer than two seconds, please quit and restart the Client.\n";
        int sock = 0, valread = 0;
        struct sockaddr_in serv_addr;
        std::string input;
        char buffer[1024] = {0};
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Socket creation error \n");
            return -1;
        }
       
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
           
        // Convert IPv4 and IPv6 addresses from text to binary form
    
        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
        {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }
       
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("\nConnection Failed \n");
            return -1;
        }
        
        std::string start = "A user has connected.";
        send(sock , start.c_str() , strlen(start.c_str()) , 0 );
        
        std::thread check(checkMail, sock, buffer);
        std::thread send(sendMail, sock);

    check.join();
    send.join();
    return 0;
}




//Keep track of the process id?
