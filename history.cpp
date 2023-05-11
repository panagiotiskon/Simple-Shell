#include "libs.h"


void print_history(history myhistory)         
{
    if(myhistory.empty()){
        cout<<"0"<<endl;
        return;
    }

    for (int i = 0; i < myhistory.size(); i++)
    {
        cout << i + 1 << " ";
        for (int j = 0; j < myhistory[i].size(); j++)
        {
            cout << myhistory[i][j] << " ";
        }
        cout << endl;
    }
    return; 
}

tokens history_command(history myhistory, tokens command)
{
    history_flag = 0;
    cout<<command.size()<<" h istory size "<<endl;
    if (command.size() == 1)   // if the command is myHistory
    {
        print_history(myhistory);
        history_flag = 1;
        return command;
    }

    int pos = stoi(command[1]);    //else get the number of the command in the History  
    if(pos <= 0 )
    {
        cout<<"Wrong History Value"<<endl;
        exit(-1);
    }
    if (pos > myhistory.size())
    {
        cout << "Wrong History Value" << endl;
        exit(-1);
    }

    tokens temp = myhistory[pos - 1];    // search for the command

    if (strcmp(temp[0], "myHistory") == 0)    // if the command myHistory the recursively find it 
    {
        return history_command(myhistory, temp);
    }
    else
    {
        command.clear();
        int new_size = temp.size();
        for (int i = 0; i < new_size; i++)
        {
            command.push_back(temp[i]);
        }
        return command;
    }    
}

