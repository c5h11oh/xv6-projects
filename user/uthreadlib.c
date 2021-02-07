#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

#define PGSIZE 4096

int
thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2)
{
  void* thread_stack = malloc(PGSIZE);
  return clone(start_routine, arg1, arg2, thread_stack);
}

int
thread_join()
{
  int ret_val;
  void* thread_stack = NULL;
  ret_val = join(&thread_stack);
  if(ret_val != -1)
    free(thread_stack);
  return ret_val;
}
