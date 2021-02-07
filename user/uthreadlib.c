#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

#define PGSIZE 4096

int
thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2)
{
  void* tmp = malloc(PGSIZE * 2);
  if(!tmp) return -1;
  printf(1, "thread_create: tmp \t\taddr: %x\n", (uint)tmp);
  
  // In my test, malloc() almost always return page aligned address.
  // To make sure this method uses page aligned memory as the thread stack
  // regardless of the malloc() return value, I add some offset and 
  // make tmp2 to test my `thread_stack`'s calculation works.
  void* tmp2 = tmp + 933;
  printf(1, "thread_create: tmp2 \t\taddr: %x\n", (uint)tmp2);
  void* thread_stack = (void *)((uint)(tmp2 + PGSIZE) & ~(PGSIZE - 1)); // Round up to the next page's start
  printf(1, "thread_create: thread_stack \taddr: %x\n", (uint)thread_stack);
  return clone(start_routine, arg1, arg2, thread_stack);
}

int
thread_join()
{
  int ret_val;
  void* thread_stack = NULL;
  ret_val = join(&thread_stack);
  printf(1, "thread_join: thread_stack \taddr: %x\n", (uint)thread_stack);
  printf(1, "thread_join: thread_stack top\taddr: %x\n", (uint)thread_stack+PGSIZE-sizeof(uint));
  printf(1, "thread_join: thread_stack top \tdata: %x\n", *(uint*)(thread_stack+PGSIZE-sizeof(uint)));
  if(ret_val != -1)
    free(thread_stack);
  printf(1, "thread_join: thread_stack freed.\n");
  // It seems that the stack data still persists after free(), so I assume 
  // free() here is not working, since I did not pass in a valid pointer.
  return ret_val;
}
