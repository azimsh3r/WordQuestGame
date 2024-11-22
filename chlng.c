#include "chlng.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>

int chlng_hide_word(chlng_t* chlng) {
    if (chlng->text == NULL) {
        fprintf(stderr, "No text available to hide word\n");
        return 1;
    }

    char* start = chlng->text;
    while (*start != '\0' && !isalpha(*start)) {
        start++;
    }

    if (*start == '\0') {
        fprintf(stderr, "No alphabetic word found in text\n");
        return 1;
    }

    char* end = start;
    while (*end != '\0' && isalpha(*end)) {
        end++;
    }

    size_t word_length = end - start;

    chlng->word = malloc(word_length + 1);
    if (!chlng->word) {
        perror("Memory allocation for word failed");
        return 1;
    }

    strncpy(chlng->word, start, word_length);
    chlng->word[word_length] = '\0';

    for (size_t i = 0; i < word_length; i++) {
        start[i] = '_';
    }

    return 0;
}

chlng_t* chlng_new(void) {
    chlng_t* new_chlng = (chlng_t*)malloc(sizeof(chlng_t));
    if (new_chlng == NULL) {
        fprintf(stderr, "Error allocating memory for new_chlng\n");
        return NULL;
    }
    return new_chlng;
}

void chlng_reset(chlng_t* chlng) {
    if (chlng->text != NULL) {
        (void)free(chlng->text);
    }
    if (chlng->word != NULL) {
        (void)free(chlng->word); 
    }
}

void chlng_del(chlng_t* chlng) {
    if (chlng->text != NULL) {
        (void)free(chlng->text);
    }
    if (chlng->word != NULL) {
        (void)free(chlng->word); 
    }
    (void)free(chlng);
}

int chlng_fetch_text(chlng_t *chlng) {
    int pipe_fd[2];
    pid_t child_pid;

    if (pipe(pipe_fd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        close(pipe_fd[0]);

        char* message = (char*) malloc(1024 * sizeof(char));
        FILE *pipe = popen("fortune -s", "r");

        if (!pipe) {
            perror("popen failed");
        }

        if (fgets(message, 1024, pipe) == NULL) {
            perror("Error reading from fortune");
            free(message);
            
            pclose(pipe);
            return -1;
        }

        if (message == NULL) {
            perror("fortune -s error");
        }

        pclose(pipe);

        ssize_t bytes_written = write(pipe_fd[1], message, strlen(message) + 1);

        if (bytes_written == -1) {
            perror("Error writing to pipe");
            exit(EXIT_FAILURE);
        }

        close(pipe_fd[1]);
    } else {
        close(pipe_fd[1]);

        char* buffer = malloc(1024 * sizeof(char));

        ssize_t bytes_read = read(pipe_fd[0], buffer, 1024);

        if (bytes_read == -1) {
            perror("Error reading from pipe");
            exit(EXIT_FAILURE);
        }
        
        chlng->text = strdup(buffer);
        close(pipe_fd[0]);
    }

    return 0;
}