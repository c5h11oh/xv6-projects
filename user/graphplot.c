#include "types.h"
#include "pstat.h"
#include "user.h"
#define PROCESSES 3

int main(int argc, char* argv[]){
    if(argc != 4){
        printf(1, "graphplot: enter three positive integers as tickets. See how many ticks the process runs.\n");
        exit();
    }
    
    int totalTicket = 0;
    int tickets[PROCESSES];
    int pid[PROCESSES];
    int parentpid = getpid();
    for(int i = 0; i < PROCESSES; ++i){
        if((tickets[i] = atoi(argv[i+1])) <= 0){
            printf(1, "graphplot: Invalid ticket number.\n");
            exit();
        }
        totalTicket += tickets[i];
    }

    // Give parent the most ticket
    settickets(1);

    // Fork children
    for(int i=0; i<PROCESSES; ++i){
        pid[i] = fork();
        if(pid[i] == 0){ // Child: set tickets and spin.
            settickets(tickets[i]);
            printf(1, "child %d gets %d tickets.\n", i, gettickets());
            for(;;) ;
            exit();
        }
    }

    // Keep running, and do the statistics
    struct pstat ps;
    if(getpinfo(&ps)){
        printf(1, "Error: getpinfo() in graphplot() !\n");
        exit();
    }
    
    // Find pid <-> slot pair
    int parentSlot = -1, slot[PROCESSES];
    for(int j=0; j<NPROC; ++j){
        if(!ps.inuse[j]) continue;
        if(ps.pid[j] == parentpid){
            parentSlot = j;
            break;
        }
    }
    if(parentSlot == -1){
        printf(1, "Error: no parentSlot found.\n");
        exit();
    }

    for(int i=0; i<PROCESSES; ++i){
        for(int j=0; j<NPROC; ++j){
            if(!ps.inuse[j]) continue;
            if(ps.pid[j] == pid[i]){
                slot[i] = j;
                break;
            }
        }
        if(slot[i] >= 0) continue;
        printf(1, "Error: no slot for child process %d found.\n", i);
        exit();
    }

    printf(1, "\n\tparent\tproc1\tproc2\tproc3\n");
    printf(1, "pid\t%d\t%d\t%d\t%d\n", parentpid, pid[0], pid[1], pid[2]);
    printf(1, "slot\t%d\t%d\t%d\t%d\n", parentSlot, slot[0], slot[1], slot[2]);
    printf(1, "ticket\t%d\t%d\t%d\t%d\n", ps.tickets[parentSlot], ps.tickets[ slot[0] ], ps.tickets[ slot[1] ], ps.tickets[ slot[2] ]);
    printf(1, "====================================\n");
    for(int i = 0; i < 10; ++i){
        if(getpinfo(&ps)){
            printf(1, "Error: getpinfo() in graphplot() !\n");
            for(int i=0; i<PROCESSES; ++i){
                if(kill(pid[i]) != 0)
                    printf(1, "Error: kill child %d !\n", i);
            } 
            exit();
        }
        printf(1, "\t%d\t%d\t%d\t%d\n", 
            ps.ticks[ parentSlot ], 
            ps.ticks[ slot[0] ], 
            ps.ticks[ slot[1] ],
            ps.ticks[ slot[2] ]);
        printf(1, "ticket\t%d\t%d\t%d\t%d\n", ps.tickets[parentSlot], ps.tickets[ slot[0] ], ps.tickets[ slot[1] ], ps.tickets[ slot[2] ]);
        printf(1, "====================================\n");
        sleep(100);
    }

    // kill children
    for(int i=0; i<PROCESSES; ++i){
        if(kill(pid[i]) != 0)
            printf(1, "Error: kill child %d !\n", i);
    } 

    exit();
}