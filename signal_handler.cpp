#include "libs.h"

void signal_handler(int){
    cout<<"eeeeeeeeeeeeeeeeee"<<endl;
    kill(getpid(), SIGTSTP);
}

void signal_handler2(int){
    stopped_processes_counter++;
    cout<<"\n["<<stopped_processes_counter<<"]+ Stopped                 "<<endl;
}