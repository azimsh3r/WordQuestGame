#include "player.h"
#include "chlng.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

player_t* player_new(void) {
    player_t* new_player = (player_t*) malloc(sizeof(player_t));

    if(new_player == NULL) {
        fprintf(stderr, "ERROR allocating memory for new_player");
        return NULL;
    }

    new_player->solved = 0;
    new_player->total = 0;
    new_player->finished = false;
    return new_player;
}

int player_get_greeting(player_t* player, char** msg) {
    char* greeting_msg = (char*)malloc(sizeof(char) * 40);

    if (greeting_msg == NULL) {
        fprintf(stderr, "ERROR allocating memory for greeting_msg\n");
        return -1;
    }

    strcpy(greeting_msg, "M: Hello! Welcome to the game");
    *msg = greeting_msg;

    return 0;
}

void player_reset(player_t* player) {
    player->finished = 0;
    player->solved = 0;
    player->finished = false;
    
    chlng_reset(player->chlng);
}

void player_del(player_t* player) {
    chlng_del(player->chlng);
    (void)free(player);
}

int player_fetch_chlng(player_t* player) {
    player->chlng = chlng_new();

    chlng_fetch_text(player->chlng);

    chlng_hide_word(player->chlng);
}

int player_get_challenge(player_t* player, char** msg) {
    if (player == NULL || player->chlng == NULL || player->chlng->text == NULL) {
        return -1;
    }

    const char* prefix = "C: ";
    size_t prefix_len = strlen(prefix);
    size_t text_len = strlen(player->chlng->text);

    *msg = (char*)malloc((prefix_len + text_len + 1) * sizeof(char));
    if (*msg == NULL) {
        perror("malloc failed");
        return -1;
    }

    strcpy(*msg, prefix);
    strcat(*msg, player->chlng->text);

    return 0;
}

int player_post_challenge(player_t* player, char* response, char** msg) {
    if (response == NULL || player->fd <= 0) {
        fprintf(stderr, "Invalid input to player_post_challenge\n");
        return -1;
    }

    printf("%s\n", player->chlng->word);
    if (send(player->fd, *msg, strlen(*msg), 0) < 0) {
        perror("send failed");
        return -1;
    }

    char* buffer = (char*)malloc(128*sizeof(char));
    if (buffer == NULL) {
        perror("malloc error");
        return -1;
    }

    size_t bytes_read = recv(player->fd, buffer, 127, 0);
    if (bytes_read <= 0) {
        perror("recv failed");
        free(buffer);
        return -1;
    }
    buffer[bytes_read] = '\0';

    strncpy(response, buffer, 128);
    response[127] = '\0';

    free(buffer);
    return 0;
}