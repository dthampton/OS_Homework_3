// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#define PORT 8080
   
int main(int argc, char const *argv[])
{
while(true) {
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
        
        std::getline(std::cin, input);
        
        std::cout << "Entered the do loop!\n";
        send(sock , input.c_str() , strlen(input.c_str()) , 0 );
        valread = read( sock , buffer, 1024);
        printf("%s\n",buffer );
        std::cout << "Restarting the while loop!\n";
    }
    return 0;
}
