#include "libs.h"

tokens command;
history myhistory;

char **parameters;
char **environ;
int history_flag = 0;
int redirection_flag;
int pipe_flag;
int stopped_processes_counter = 0;
table alias_table;

list<int> rpos; // save in a list all the positions of the redirections

int main()
{
    signal(SIGINT, SIG_IGN);  //  ignore ctrl c
    signal(SIGTSTP, SIG_IGN); // ignore ctrl z

    while (1)
    {
        rpos.clear();
        command.clear();
        redirection_flag = 0;
        pipe_flag = 0;

        printf("in-mysh-now>:");

        int counter = 0;

        command = read_command(); // get vector with cmd arguments

        if (command.empty())
            continue;

        if (strcmp(command[0], "exit") == 0) // when command is exit terminate
        {
            return 1;
        }

        if (!alias_table.empty()) // if we have stored aliases
        {
            command = check_for_aliases(command); // parse for aliases and replace them in the command vector;
        }

        command = redirections_pipes_in_cmd(command); // parse command and correct vector if there are redirections

        command = env_variables(command);

        int size = command.size();

        parameters = new char *[size + 1]; // get a new array of pointers to store parameters

        if (!command.empty() && strcmp(command[0], "myHistory") != 0) // if we have a command push it to myhistory
        {
            if (myhistory.size() >= 20) // history size is 20
            {
                myhistory.erase(myhistory.begin());
            }
            myhistory.push_back(command);
        }

        if (strcmp(command[0], "myHistory") == 0) // when command is myHistory
        {
            tokens history_cmd;
            history_cmd = history_command(myhistory, command); // return the correct command to be run (ex. myHistory 1 or myHistory)

            if (myhistory.size() >= 20)
            {
                myhistory.erase(myhistory.begin());
            }
            myhistory.push_back(command); // add the command to history

            if (history_flag == 1) // if the command was "myHistory" then print history continue
            {
                continue;
            }
            else
            {
                command.clear(); // reset command vector
                for (int i = 0; i < history_cmd.size(); i++)
                { // pass the command from history to be run in child process
                    command.push_back(history_cmd[i]);
                }
            }
        }

        if (strcmp(command[0], "createalias") == 0)    //createalias
        {
            add_alias(command);
            continue;
        }
        else if (strcmp(command[0], "destroyalias") == 0)    //destroyalias
        {
            remove_alias(command);
            continue;
        }
        else if (strcmp(command[0], "print") == 0)   //print alias_table
        {
            print_alias();
            continue;
        }
        else if (strcmp(command[0], "cd") == 0)    //cd 
        {
            cd_call(command);
            continue;
        }

        pid_t pid = fork();

        if (pid < 0)
        {
            exit(-1);
        }

        else if (pid == 0)
        {

            signal(SIGINT, SIG_DFL);   // reset signal for CTRL C
            signal(SIGTSTP, SIG_DFL); // reset signal for CTRL Z

            call_for_redirection(command); // check for redirections in command
            check_for_pipes(command);      // check for pipes in command

            if (pipe_flag > 0)    
            {
                call_for_pipes(command);
            }

            if (redirection_flag == 0) // if we have no redirections pass the command to the parameters as it is
            {
                for (int i = 0; i < size; i++)
                {
                    parameters[i] = command[i];
                }
                parameters[size] = nullptr;
            }

            else
            {
                int k = rpos.front(); // get first redirection symbol position to run the command

                for (int i = 0; i < k; i++)
                {
                    parameters[i] = command[i];
                }

                parameters[k] = nullptr; // pass the command to the parameters
            }
            if (pipe_flag == 0) // if we have no pipes
            {
                cout << parameters[0] << endl;

                if (execvp(parameters[0], parameters) == -1)
                {
                    cerr << "Command not found" << endl;
                    exit(-1);
                }
            }
        }
        else
        {
            waitpid(pid, NULL, WUNTRACED);
        }
    }
    return 0;
}