//PANAGIOTIS KONTOEIDIS
//1115201900266

#include "libs.h"

tokens cmd_tokens;

tokens read_command()
{
    cmd_tokens.clear();
    char line[MAX_COMMAND_SIZE];
    char *pch;
    char ch;

    fgets(line, MAX_COMMAND_SIZE, stdin); // read the line of stdin

    line[strcspn(line, "\n")] = '\0'; // replace newline character with null char
    char *command = new char[strlen(line) + 1];
    strcpy(command, line);
    pch = strtok(command, " ");

    while (pch != NULL)
    {
        cmd_tokens.push_back(pch);
        pch = strtok(NULL, " ");
    } // tokenise command line
    return cmd_tokens;
}

tokens split_command(char *command)
{ // takes a command and tokenises it
    tokens temp;
    if (strlen(command) == 0)
    {
        exit(-1);
    }
    char *temp_cmd = strtok(command, " ");
    while (temp_cmd != NULL)
    {
        temp.push_back(temp_cmd);
        temp_cmd = strtok(NULL, " ");
    }
    return temp;
}

tokens env_variables(tokens command)    //parses command and replaces environment variables
{
    tokens new_command;
    int env_flag = 0;
    int k = 0;
    for (int i = 0; i < command.size(); i++)
    {
        env_flag = 0;
        char *var = new char[100];

        for (int j = 0; j < strlen(command[i]); j++)
        {
            if (command[i][j] == '$' && strlen(command[i])>1)
            {
                env_flag = 1;
                continue;
            }
            if (env_flag == 1)
            {
                var[k++] = command[i][j];
            }
            else
                continue;
        }
        if (env_flag == 1)
        {
            var[k] = '\0';
            char *env = getenv(var);
            new_command.push_back(env);
        }
        else
        {
            new_command.push_back(command[i]);
        }
    }
    return new_command;
}

void cd_call(tokens command)      // change working directory with command cd
{
    if (command.size() == 1)                // if cmd = "cd"
    {
        char *h = getenv("HOME");   
        if(chdir(h)==-1){
            cout<<"error chdir"<<endl;
            exit(-1);
        }
        return;
    }
    else                                    // if we have args
    {
        char *dir = command[1];
        if(chdir(dir)==-1){
            cout<<"error chdir"<<endl;
            exit(-1);
        }
        return;
    }
}
