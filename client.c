#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int port = 8080;

    int opt;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
        }
    }

    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    int valread = read(client_fd, buffer, sizeof(buffer));
    if (valread > 0) {
        printf("%s\n", buffer);
    } else {
        perror("read error");
        exit(1);
    }

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        valread = read(client_fd, buffer, sizeof(buffer));
        if (valread > 0) {
            printf("Question: %s", buffer);
        } else if (valread < 0) {
            perror("read error");
            exit(1);
        }

        char input[128];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; 

        send(client_fd, input, strlen(input), 0);

        memset(input, 0, sizeof(input));
        memset(buffer, 0, sizeof(buffer));

        valread = read(client_fd, buffer, sizeof(buffer));
        if (valread > 0) {
            printf("%s\n", buffer);
        } else if (valread < 0) {
            perror("read error");
            exit(1);
        }

        if (strncmp(input, "Q:", 2) == 0) {
            break;
        }
    }

    close(client_fd);
    return 0;
}