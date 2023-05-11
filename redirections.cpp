#include "libs.h"


tokens redirections_pipes_in_cmd(tokens parameters) // tokenize down given tokens from the parameters and finds hidden >,<, | chars
{
      
    tokens new_parameters;

    int flag = 0;
    
    string temp;

    if(parameters.size()==0){
        return parameters;
    }
    for (int i = 0; i < parameters.size(); i++)
    {
        temp.clear();
        flag = 0;
        
        if (strcmp(parameters[i], ">") == 0 || strcmp(parameters[i], "<") == 0 || strcmp(parameters[i], ">>") == 0 || strcmp(parameters[i], "|")==0)
        {
            new_parameters.push_back(parameters[i]);
            continue;
        }

        for (int a = 0; a < strlen(parameters[i]); a++)
        {
            if (parameters[i][a] == '>')
            {
                if (temp.size() != 0)
                {
                    char * p = new char[temp.size()];
                    strcpy(p, temp.c_str());
                    new_parameters.push_back(p);
                    temp.clear();
                }

                char * sc = new char[1];
                strcpy(sc, ">");
                new_parameters.push_back(sc);
                flag = 1;
            }
            else if (parameters[i][a] == '<')
            {
                if (temp.size() != 0)
                {
                    char * p = new char[temp.size()+1];
                    strcpy(p, temp.c_str());
                    new_parameters.push_back(p);
                    temp.clear();
                }

                char * sc = new char[1];
                strcpy(sc, "<");
                new_parameters.push_back(sc);
                flag = 1;
            }
            else if (parameters[i][a] == '|')
            {
                if (temp.size() != 0)
                {
                    char *p = new char[temp.size() + 1];
                    strcpy(p, temp.c_str());
                    new_parameters.push_back(p);
                    temp.clear();
                }

                char *sc = new char[1];
                strcpy(sc, "|");
                new_parameters.push_back(sc);
                flag = 1;
            }
            else
            {
               temp.push_back(parameters[i][a]);
            }
        }
        if (flag == 1) // edw shmainei oti egine allagh
        {
            char *p = new char[temp.size() + 1];
            strcpy(p, temp.c_str());
            new_parameters.push_back(p);
            temp.clear();
            continue;
        }
        else
        { // an den egine kamia allagh sto string
            if(strlen(parameters[i])!=0)
                new_parameters.push_back(parameters[i]);
        }
    }
    
    for (auto it = new_parameters.begin(); it != new_parameters.end(); ) {   //delete all blank vector elements
        if (strlen(*it) == 0) {
            it = new_parameters.erase(it);
        } else {
            ++it;
        }
    }

    return new_parameters;
}

void call_for_redirection(tokens command)   // call for redirections
{
    for (int i = 0; i < command.size(); i++)
    {

        if (strcmp(command[i], ">") == 0)
        {
            rpos.push_back(i);
            redirection_flag++;
            output_redirection(command[i + 1]);
        }
        else if (strcmp(command[i], "<") == 0)
        {
            rpos.push_back(i);
            redirection_flag++;
            input_redirection(command[i + 1]);
        }
        else if (strcmp(command[i], ">>") == 0)
        {
            rpos.push_back(i);
            redirection_flag++;
            append_redirection(command[i + 1]);
        }
    }
}

void output_redirection(const char *output_file)  // >
{
    int fd = open(output_file, O_CREAT | O_WRONLY, 0666);
    if (fd == -1)
    {
        cout<<"error with open"<<endl;
        exit(-1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        cout<<"error with dup2"<<endl;
        exit(-1);
    }

    close(fd);
}

void input_redirection(const char *output_file)    // <
{
    int fd = open(output_file, O_RDONLY);
    if (fd == -1)
    {
        cout<<"error with open"<<endl;
        exit(-1);
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        cout<<"error with dup2"<<endl;
        exit(-1);
    }

    close(fd);
}

void append_redirection(const char *output_file)  // >>
{
    int fd = open(output_file, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (fd == -1)
    {
        cout<<"error with open"<<endl;
        exit(-1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        cout<<"error with dup2"<<endl;
        exit(-1);
    }

    close(fd);
}
