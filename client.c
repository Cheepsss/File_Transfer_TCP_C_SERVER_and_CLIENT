#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


void send_file(FILE *file, int sock){
    int n;
    char data[2048] = {0};

    //receive until there is no data left
    while(fgets(data, 2048, file) != NULL) {
        if (send(sock, data, sizeof(data), 0) == -1) {
            printf("error while sending data");
            exit(1);
        }
        bzero(data, 2048);
    }
}

int main(){
    char *ip = "127.0.0.1";
    int port = htons(8080);
    int e;
    int sock;
    struct sockaddr_in server_addr;
    FILE *file;
    char *filename = "send.txt";

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        printf("Error while creating socket");
        exit(1);
    }

    //declare the type and connect to the server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e == -1) {
        printf("Error while connecting");
        exit(1);
    }
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error in reading file.");
        exit(1);
    }
    send_file(file, sock);
    close(sock);

    return 0;
}