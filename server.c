# include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void get_file(int sock){
    int n;
    FILE *file;
    char *filename = "transfered_file.txt";
    char buff[2048];

    file = fopen(filename, "w");

    //reciving while there is data to receive
    while(1 == 1){
        n = recv(sock, buff, 2048, 0);
        if (n <= 0){
            printf("Got the file \n");
            fclose(file);
            return;
        }
        fprintf(file, "%s", buff);
        bzero(buff, 2048);
    }
    return;
}

void print_file(){
    FILE *file;
    char *filename = "transfered_file.txt";
    int c;
    file = fopen(filename, "r");
    //reading the file
    while(1==1){
    c = fgetc(file);
    if(feof(file)){
        break;
    }
    printf("%c", c);
    }
    fclose(file);
    return;
}


int main(int argc, char *argv[]){
    int sock, new_socket;
    int connection;
    int PORT;
    struct sockaddr_in address, new_address;
    socklen_t address_size;

    
    //make sure the address and the port were set
    if(argc != 3){
        printf("Enter address and port");
        exit(-1);
    }
    PORT = atoi(argv[2]);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    //set socket and port

    if(sock < 0){
        printf("couldn't create the socket");
        exit(1);
    }

    // using sockaddr struct for handling internet address
    address.sin_family = AF_INET;
    //address.sin_addr.s_addr = inet_addr(htonl(argv[1]));
    //use argv1 if you want specific address, I will use localhost
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    //converting from host byte order to network byte order with htons

    connection = bind(sock, (struct sockaddr *)&address, sizeof(address));
    if (connection < 0){
        printf("error connecting with the client");
        exit(1);
    }

    //listening and getting the file
    while(1){
        listen(sock, 10);
        address_size = sizeof(new_address);
        new_socket = accept(sock, (struct sockaddr*)&new_address, &address_size);
        get_file(new_socket);
        printf("Data: \n");
        print_file();
        printf("\n");
    }

}