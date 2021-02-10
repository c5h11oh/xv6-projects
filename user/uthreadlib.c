#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

#define PGSIZE 4096

int
thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2)
{
  void* alloc_addr = malloc(PGSIZE * 2); // real

  /* debug */
  // void* alloc_addr = malloc(PGSIZE * 3);
  // printf(1, "thread_create: original alloc_addr \t\taddr: %x\n", (uint)alloc_addr);
  // alloc_addr = (void *)((uint)(alloc_addr + PGSIZE) & ~(PGSIZE - 1)) - 5;
  /* end of debug */
  
  if(!alloc_addr) return -1;
  printf(1, "thread_create: alloc_addr \t\taddr: %x\n", (uint)alloc_addr);
  
  void* thread_stack = (void *)((uint)(alloc_addr + PGSIZE) & ~(PGSIZE - 1)); // Round up to the next page's start
  printf(1, "thread_create: thread_stack \taddr: %x\n", (uint)thread_stack);
  
  // We need to store address of alloc_addr so that we can free it when calling thread_join.
  // Should store it in the space allocated by malloc but not used by the thread.
  // We usually store the addr at `thread_stack`-5, if such space is allocated.
  // If `thread_stack`-5 is not allocated to us, store the addr at `thread_stack` + PGSIZE
  // In any case, `thread_stack`-1 must be valid. We store a byte-long number 
  // indicating whether the addr is stored at `thread_stack`-5 or at `thread_stack` + PGSIZE.
  // The number is 0 for the former case, and 1 for the latter.
  void* ptr = thread_stack - 1;
  if( (uint)alloc_addr % PGSIZE > PGSIZE - 5){
    // store the addr at `thread_stack` + PGSIZE
    *(char*)ptr = (char)1;
    ptr = thread_stack + PGSIZE;
  }
  else{
    // store the addr at `thread_stack`-5
    *(char*)ptr = (char)0;
    ptr -= 4;
  }
  *(void**)ptr = alloc_addr;

  return clone(start_routine, arg1, arg2, thread_stack);
}

int
thread_join()
{
  int ret_val;
  void* thread_stack = NULL, *alloc_addr = NULL;
  ret_val = join(&thread_stack);
  printf(1, "thread_join: thread_stack \taddr: %x\n", (uint)thread_stack);
  if(ret_val != -1){
    // find the `alloc_addr` and free it
    void* ptr = thread_stack - 1;
    if(*(char*)ptr == (char)1)
      ptr = thread_stack + PGSIZE;
    else
      ptr -= 4;
    alloc_addr = *(void**)ptr;
    printf(1, "thread_join: alloc_addr \t\taddr: %x\n", (uint)alloc_addr);
    if(alloc_addr)
      free(alloc_addr);
    printf(1, "thread_join: thread_stack freed.\n");
  }
  return ret_val;
}
