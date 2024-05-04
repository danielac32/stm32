#ifndef SYSCALL_H_
#define SYSCALL_H_

enum syscall_code {
    //FILES
    SYSCALL_READ = 1,
    SYSCALL_READBUFF = 2,
    SYSCALL_WRITE = 3,
    SYSCALL_WRITEBUFF = 4,
    SYSCALL_OPEN = 5,
    SYSCALL_CLOSE = 6,
    SYSCALL_SEEK = 7, 
    SYSCALL_FSIZE = 8, 
    SYSCALL_OPENDIR = 9,
    SYSCALL_READDIR = 10,
    SYSCALL_CLOSEDIR = 11,
    
    //MALLOC
    SYSCALL_MALLOC = 12,
    SYSCALL_FREE = 13,
    
    //TASK
    SYSCALL_CREATETASK = 14,
    SYSCALL_DELETETASK = 15,
    SYSCALL_PID = 16,
    SYSCALL_GETPID = 17,
    SYSCALL_YIELD = 18,
    SYSCALL_WAIT = 19,
    SYSCALL_SLEEP = 20,
    SYSCALL_EXIT = 21,
    SYSCALL_KILL = 22,
    //SERIAL
    SYSCALL_PRINTSTR = 23,
    SYSCALL_PRINTCHAR = 24,

    SYSCALL_EXIT2 = 25
};

void syscall_handler(uint32_t code);
void svc_handler();
#endif
