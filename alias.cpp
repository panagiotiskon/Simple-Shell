#include "libs.h"


void add_alias(tokens command)
{ // add command to alias table
    char *cmd;
    char *temp;

    if (command.size() < 2)
    {
        cout << "Incorrect alias Syntax" << endl;
        return;
    }
    else
    {
        cout<<command[2]<<"comand 2"<<endl;
        cout<<command.size()<<endl;

        if (command.size() == 3)             // 
        {
            temp = new char[strlen(command[2]) - 2]; // store the alias without the ""
            int j =0;
            for (int i = 0; i < strlen(command[2]); i++)
            {
                if (command[2][i] != '"' && command[2][i] != ';')
                {
                    temp[j++] = command[2][i];
                }
            }
            temp[j] = '\0';
        }
        else if(command.size()>3)
        {
            int k =0;
            temp = new char[100];

            for (int i = 2; i < command.size(); i++)
            {
                for (int j = 0; j < strlen(command[i]); j++)
                {
                    if (command[i][j] != '"' && command[i][j] != ';')
                    {
                        temp[k++] = command[i][j];
                    }
                }
                temp[k++] = ' ';
            }

            temp[k+1]='\0';
        }

        if(!alias_table.empty())
        {
            for (int i = 0; i < alias_table.size(); i++)
            { // if an alias with the same name is found delete it to update it with the new command
                if (strcmp(alias_table[i].first, command[1]) == 0)
                {
                    auto it = alias_table.begin() + i;
                    alias_table.erase(it);
                }
            }
        }
        cout<<"temp is"<<temp<<endl;
        alias_table.push_back(make_pair(command[1], temp)); // insert it to alias table
        return;
    }
}

void print_alias(){                     //prints alias table
    if(alias_table.empty()){
        return;
    }
    for(auto& p: alias_table){
        cout<<p.first<<" -> "<<p.second<<endl;
    }
}

void remove_alias(tokens command){      //removes alias from alias table

    char* temp = new char[strlen(command[1])];
    int j =0;

    if(alias_table.empty()){
        return ;
    }
    for(int i =0; i<strlen(command[1]); i++){
        if(command[1][i] != ';'){
            temp[j++] = command[1][i];
        }
    }
    temp[j] = '\0';
    cout<<"temp is "<<temp;
    for (int i = 0; i < alias_table.size(); i++)     // if an alias with the same name is found delete it to update it with the new command
    { 
        if (strcmp(alias_table[i].first, temp) == 0)
        {
            auto it = alias_table.begin() + i;
            alias_table.erase(it);
        }
    }
    delete[]temp; 
    return;
}

tokens check_for_aliases(tokens command) //parses command and if it finds alias it replaces it and returns updated command
{
    tokens alias_command;

    alias_command.clear();
    int alias_flag = 0;

    if(strcmp(command[0], "createalias")==0){
        return command;
    }
    else if(strcmp(command[0], "destroyalias")==0){
        return command;
    }
    else if(strcmp(command[0], "myHistory")==0){
        return command;
    }

    for (int i = 0; i < command.size(); i++)
    {
        // cout<<command.size()<<" s1"<<endl;
        for (int j = 0; j < alias_table.size(); j++) 
        { 
            alias_flag = 0;
            // cout<<alias_table[j].first<<" s2"<<endl;
            if (strcmp(alias_table[j].first, command[i]) == 0)
            {
                alias_flag =1;
                // cout<<command[i]<<"comd "<<endl;
                tokens temp2; 
                char * temp = alias_table[j].second;
                temp2 = split_command(temp);
                for(int a=0; a<temp2.size(); a++){
                    cout<<temp2[a]<<endl;
                    alias_command.push_back(temp2[a]);
                }
                break;
            }
        }
        if(alias_flag==0){
            alias_command.push_back(command[i]);        
        }
        else if (alias_flag==1){
            continue;
            }
    }

    // for(int i=0; i<alias_command.size(); i++){
    //     cout<<"new  alias"<<alias_command[i]<<endl;
    // }
    return alias_command;
}