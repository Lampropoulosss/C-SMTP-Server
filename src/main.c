#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "handleClient.h"

#define PORT 25

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

int main()
{
    int server_fd;
    sockaddr_in server_addr;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket Creation Failed.");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Socket Binding Failed.");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("Socket Listening Failed.");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("SMTP server listening on port %d\n", PORT);

    while (1)
    {
        sockaddr_in client_addr;
        socklen_t client_addr_length = sizeof(client_addr);

        int client_fd = accept(server_fd, (sockaddr *)&client_addr, &client_addr_length);
        if (client_fd < 0)
        {
            perror("Could not accept connection.");
            continue;
        }

        printf("Connection accepted.\n");
        FILE *log_file = fopen("connections.log", "a");
        fprintf(log_file, "Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        fclose(log_file);

        handleClient(client_fd);
        close(client_fd);
    }
}