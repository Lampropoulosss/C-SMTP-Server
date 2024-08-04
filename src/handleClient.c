#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "handleClient.h"

#define BUFF_SIZE 1024

void handleClient(int client_fd)
{
    char buffer[BUFF_SIZE] = {0};

    while (1)
    {
        if (recv(client_fd, buffer, BUFF_SIZE, 0) <= 0)
        {
            printf("Connection closed.\n");
            return;
        }

        if (strncmp(buffer, "QUIT", 4) == 0)
        {
            printf("Connection closed by client.\n");
            return;
        }
        else if (strncmp(buffer, "HELO", 4) == 0)
        {
            char domain[256];
            char response[BUFF_SIZE];

            sscanf(buffer, "HELO %s", domain);
            int response_length = snprintf(response, BUFF_SIZE, "250 %s", domain);
            if (send(client_fd, response, response_length, 0) < 0)
            {
                perror("Could not send response.");
                return;
            }
        }
        else
        {
            send(client_fd, "500 Command not recognized", 26, 0);
        }
    }
}