#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *connection_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char *hello = "Hello, world!";
    char buffer[1024] = {0};
    char method[16] = {0};
    char url[1024] = {0};
    char protocol[16] = {0};
    char resp[2000] = {0};

    // Read the request
    read(sock, buffer, sizeof(buffer)-1);

    // Parse the request
    sscanf(buffer, "%s %s %s", method, url, protocol);

    // Append the method, url, and protocol to the hello string
    sprintf(resp, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12\nMethod: %s\nURL: %s\nProtocol: %s\n\n%s\n\n", method, url, protocol, hello);

    // Send the response
    write(sock, resp, strlen(resp));

    printf("Response sent\n");
    close(sock);
    free(socket_desc);
    return NULL;
}


int main() {
    int server_fd, new_socket, *new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        pthread_t thread;
        new_sock = malloc(sizeof(int));
        *new_sock = new_socket;
        
        if (pthread_create(&thread, NULL, connection_handler, (void*)new_sock) < 0) {
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }
        
        // Detach the thread so it doesn't need to be joined
        pthread_detach(thread);
    }
    
    return 0;
}
