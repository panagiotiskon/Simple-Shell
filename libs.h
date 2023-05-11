#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "fcntl.h"
#include "sys/wait.h"
#include "unistd.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <sys/types.h>
#include <signal.h>
#include <unordered_map>

using namespace std;

#define MAX_COMMAND_SIZE 1024
#define MAX_ARGS 10

typedef std::vector<char* > tokens;
typedef std::vector<tokens > history;
typedef std::vector<pair<char *,char *>> table;

extern int history_flag;
extern int output_redirection_flag;
extern int input_redirection_flag;
extern int append_flag;
extern int pipe_flag;
extern int redirection_flag;
extern list<int> rpos;
extern int stopped_processes_counter;
extern table alias_table;


tokens read_command();
tokens split_command(char * command);

void print_history(history myhistory);
tokens history_command(history myhistory, tokens command);

tokens redirections_pipes_in_cmd(tokens parameters);
void call_for_redirection(tokens command);

void output_redirection(const char * output_file);
void input_redirection(const char * output_file);
void append_redirection(const char * output_file);

void pipes(tokens command1, tokens command2);
void check_for_pipes(tokens command);
void call_for_pipes(tokens command);

void signal_handler(int);
void signal_handler2(int);

void add_alias(tokens command);  
void remove_alias(tokens command);
void print_alias();
tokens check_for_aliases(tokens command);

tokens env_variables(tokens command);    //parses command and replaces environment variables
