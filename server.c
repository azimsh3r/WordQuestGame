#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include "chlng.h"
#include "player.h"

void* play_game(int current_socket) {
    bool running = true;

    player_t* player = player_new();
    char* greeting_msg = NULL;

    if (player_get_greeting(player, &greeting_msg) != 0) {
        fprintf(stderr, "Failed to get greeting message\n");
        player_del(player);
        close(current_socket);
        return NULL;
    }

    send(current_socket, greeting_msg, strlen(greeting_msg), 0);

    (void)free(greeting_msg);

    do {
        char* response = (char*)malloc(256 * sizeof(char));

        if (response == NULL) {
            perror("malloc error");
            exit(1);
        }
        memset(response, 0, 256);

        player->chlng = chlng_new();
        player->fd = current_socket;

        player_fetch_chlng(player);

        char* question_text = NULL;
        player_get_challenge(player, &question_text);
      
        player_post_challenge(player, response, &question_text);

        if (strncmp(response, "R:", 2) == 0) {
            printf("Received answer: %s, Expected answer: %s\n", response + 2, player->chlng->word);
            if (strcmp(response + 3, player->chlng->word) == 0) {
                player->solved++;
                const char* ok_msg = "O: OK";
                send(player->fd, ok_msg, strlen(ok_msg), 0);
            } else {
                const char* fail_msg = "F: Incorrect";
                send(player->fd, fail_msg, strlen(fail_msg), 0);
            }
            player->total++;
        }else if (strncmp(response, "Q:", 2) == 0) {
            player->finished = true;
        } else {
            const char* wrong_format_msg = "M: Wrong Format";
            send(player->fd, wrong_format_msg, strlen(wrong_format_msg), 0);
        }

        (void)free(response);
    } while(!player->finished);

    char result_message[256];
    sprintf(result_message, "M: You mastered %d/%d challenges. Good bye!", player->solved, player->total);

    send(player->fd, result_message, strlen(result_message), 0);

    player_del(player);
    close(current_socket);
}

int main(int argc, char *argv[]) {
    int forking_mode = -1;
    int port = 8080;

    int opt;
    while ((opt = getopt(argc, argv, "ftp:")) != -1) {
        switch (opt) {
            case 't':
                forking_mode=0;
                return 0;
            case 'f':
                forking_mode=1;
                break;
            case 'p':
                port = atoi(optarg);
                break;
        }
    }

    int server_fd, current_socket;
    struct sockaddr_in address;

    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        if ((current_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        if (forking_mode == 1) {
            int child_pid;

            child_pid = fork();

            if (child_pid == -1) {
                perror("Fork failed");
                return 1;
            }

            if (child_pid == 0) {
                play_game(current_socket);
            }  else {
                close(current_socket);
            }
        } else if (forking_mode == 0) {
            pthread_t thread_id;

            if (pthread_create(&thread_id, NULL, (void*)play_game, (void*)&current_socket) != 0) {
                perror("Thread creation failed");
                close(server_fd);
                exit(EXIT_FAILURE);
            }

            pthread_detach(thread_id);
        } else {
            play_game(current_socket);
        }
    }
    
    close(server_fd);
    return 0;
}
