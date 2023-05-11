#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

// Function to redirect output to file
void redirect_output(const char* file_name, bool append_mode=false) {
    int fd;
    if (append_mode) {
        fd = open(file_name, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    } else {
        fd = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    }

    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

// Function to redirect input from file
void redirect_input(const char* file_name) {
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

int main() {
    // Sample command
    const char* command = "ls -l > output.txt";

    char* args[256];
    char* token = strtok((char*)command, " ");

    int i = 0;
    bool output_redirect = false;
    bool input_redirect = false;
    bool append_mode = false;
    const char* output_file_name;
    const char* input_file_name;

    // Parse the command and check for redirection symbols
    while (token != NULL) {
        if (strcmp(token, ">") == 0) {
            output_redirect = true;
            output_file_name = strtok(NULL, " ");
        } else if (strcmp(token, ">>") == 0) {
            output_redirect = true;
            append_mode = true;
            output_file_name = strtok(NULL, " ");
        } else if (strcmp(token, "<") == 0) {
            input_redirect = true;
            input_file_name = strtok(NULL, " ");
        } else if (strcmp(token, "<<") == 0) {
            // Here you can handle redirection from a here document
            // See https://en.wikipedia.org/wiki/Here_document for details
            cout << "Here document redirection not implemented" << endl;
            return 1;
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    // Redirect output if necessary
    if (output_redirect) {
        redirect_output(output_file_name, append_mode);
    }

    // Redirect input if necessary
    if (input_redirect) {
        redirect_input(input_file_name);
    }

    // Execute the command
    execvp(args[0], args);

    // If execvp returns, an error occurred
    perror("execvp");
    return 1;
}