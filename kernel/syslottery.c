#include "types.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "mmu.h"
#include "proc.h"
#include "fs.h"
#include "file.h"
#include "fcntl.h"
#include "sysfunc.h"
#include "pstat.h"

int
sys_getpinfo(void){
    struct pstat * ps;
    if(argptr(0, (char **)&ps, sizeof(struct pstat)) != 0) return -1;

    return getpinfo(ps);
}

// Set tickets for lottery scheduler
int 
sys_settickets(void){
    int number;
    if (argint(0, &number) != 0) return -1;
    if(number < 0) return -1;

    proc->ticket = number;
    return 0;
}

// get tickets
int 
sys_gettickets(void){
    return proc->ticket;
}