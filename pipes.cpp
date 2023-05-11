#include "libs.h"

void pipes(tokens cmd1, tokens cmd2) //opens a pipe between two commands
{

    int fd[2];
    pid_t pid;

    int s1 = cmd1.size();
    int s2 = cmd2.size();

    char **par1 = new char *[s1 + 1];
    char **par2 = new char *[s2 + 1];
    for (int i = 0; i < s1; i++)
    { // cmd 1
        par1[i] = cmd1[i];
    }
    par1[s1] = nullptr;

    for (int i = 0; i < s2; i++)
    { // cmd 2
        par2[i] = cmd2[i];
    }
    par2[s2] = nullptr;

    
    if (pipe(fd) == -1) // call pipe
    {
        perror(" pipe ");
        exit(1);
    }

    if ((pid = fork()) == -1) // lecture 9 p.53
    {
        perror(" fork ");
        exit(1);
    }
    if (pid == 0)
    { // child and writer
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execvp(par1[0], par1);
        perror(" execlp ");
        exit(-1);
    }
    else
    { // parent and reader
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execvp(par2[0], par2);
        exit(-1);
    }
}

void check_for_pipes(tokens command) // turns on pipe flag if pipe symbol found
{
    int pipe_pos = -1;

    for (int i = 0; i < command.size(); i++)
    { // find pipe symbol position to split the command into two commands
        if (strcmp(command[i], "|") == 0)
        {
            pipe_flag++;
            return;
        }
    }
    return;
}

void call_for_pipes(tokens command) // splits command to two sub commands to pass to pipes()
{
    int pipe_pos = -1;
    tokens cmd1;
    tokens cmd2;

    for (int i = 0; i < command.size(); i++) // find pipe symbol position to split the command into two commands
    { 
        if (strcmp(command[i], "|") == 0)
        {
            pipe_pos = i;
            break;
        }
    }

    if (redirection_flag == 0) // if we have no redirections
    {
        for (int i = 0; i < pipe_pos; i++)
        { // cmd1
            cmd1.push_back(command[i]);
        }
        for (int i = pipe_pos + 1; i < command.size(); i++)
        { // cmd2
            cmd2.push_back(command[i]);
        }
    }
    else
    {
        int k = rpos.front(); // get first redirection symbol position to run the command
        for (int i = 0; i < pipe_pos; i++)
        { // cmd1
            cmd1.push_back(command[i]);
        }
        for (int i = pipe_pos + 1; i < k; i++)
        { // cmd2
            cmd2.push_back(command[i]);
        }
    }
    pipes(cmd1, cmd2);
}