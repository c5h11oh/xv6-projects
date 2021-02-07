#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int main(){
    char* pgptr = NULL;
    int returnval = 100;
    // printf(1, "%s\n", *pgptr);

    pgptr = (char *)0x1000;
    printf(1, "page 1 content: %s\n", pgptr);
    
    printf(1, "mprotect() 1 page\n");
    returnval = mprotect(pgptr, 1);
    printf(1, "mprotect()'s returnval: %d\n", returnval);

    pgptr++;
    printf(1, "pgptr = %x\n", pgptr);
    printf(1, "munprotect() 1 page\n");
    returnval = munprotect(pgptr, 1);
    printf(1, "mprotect()'s returnval: %d\n", returnval);

    pgptr = (void *)0x1000;
    printf(1, "pgptr = %x\n", pgptr);
    printf(1, "munprotect() 1 page\n");
    returnval = munprotect(pgptr, 1);
    printf(1, "mprotect()'s returnval: %d\n", returnval);

    printf(1, "Try to change content at pgptr ...\n");
    for(int i=0; i<20; ++i, ++pgptr)
        *(char*)pgptr = 'S';

    pgptr = (void *)0x1000;
    printf(1, "page 1 content: %s\n", pgptr);

    pgptr = (void*)0x2;
    printf(1, "pgptr = %d\n", pgptr);
    printf(1, "%s\n", pgptr);
    exit();
}