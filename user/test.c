#include "types.h"
#include "stat.h"
#include "user.h"

char str1[] = "Hello, world. I am arg1. \n";
char str2[] = "So I can only be arg2. \n";
int x;
lock_t theLock;

void fcntoberun(void *arg1, void *arg2)
{
  char *s_arg1 = (char *)arg1, *s_arg2 = (char *)arg2;
  printf(1, "Successfully entered the fcntoberun().\n");
  printf(1, "Assuming that both args are strings, \n\targ1 = %s, and \n\targ2 = %s", s_arg1, s_arg2);
  for (int i = 0; i < 100; ++i)
  {
    ++x;
    printf(1, "fcnt: perform ++x. x = %d\n", x);
  }
  printf(1, "fcntoberun: exiting...\n");
  exit();
}

void locktest(void *arg1, void *arg2){
  int a = 0;
  lock_acquire(&theLock);
  printf(1, "test: pid = %d: In the lock, (next_)ticket_number = %d,\tturn = %d\n", getpid(), theLock.ticket_number, theLock.turn);
  for(int i=0; i<200000; ++i)
    a += i;
  printf(1, "test: pid = %d: About to release the lock. ticket_number = %d,\tturn = %d\n", getpid(), theLock.ticket_number, theLock.turn);
  lock_release(&theLock);
  exit();
}

int main(int argc, char *argv[])
{
  if (argc != 2){
    printf(1, "test: usage: test <number>\n\t1 = thread test\n\t2 = lock test\n");
    exit();
  }

  int selection = atoi(argv[1]);
  if (selection == 1)
  {
    x = 0;
    int pid = thread_create(fcntoberun, &str1, &str2);
    printf(1, "test: finished thread_create(). thread pid=%d.\n", pid);
    for (int i = 0; i < 100; ++i)
    {
      ++x;
      printf(1, "test: perform ++x. x = %d\n", x);
    }

    pid = thread_join();
    printf(1, "test: Joined the thread. thread pid=%d.\n", pid);
    printf(1, "test: exiting...\n");
  }
  else if (selection == 2){
    int a = 0;
    lock_init(&theLock);
    printf(1, "test: pid = %d: lock_init(&theLock): ticket_number = %d,\tturn = %d\n", getpid(), theLock.ticket_number, theLock.turn);
    
    lock_acquire(&theLock);
    printf(1, "test: pid = %d: Lock acquired. ticket_number = %d,\tturn = %d\n", getpid(), theLock.ticket_number, theLock.turn);
    int child[4];
    for(int i=0; i<4; ++i)
      child[i] = thread_create(locktest, NULL, NULL);
    for(int i=0; i<100000; ++i)
      a += i;
    printf(1, "test: pid = %d: About to release the lock. ticket_number = %d,\tturn = %d\n", getpid(), theLock.ticket_number, theLock.turn);
    lock_release(&theLock);

    lock_acquire(&theLock);
    printf(1, "test: pid = %d: Lock acquired. ticket_number = %d,\tturn = %d\n", getpid(), theLock.ticket_number, theLock.turn);
    for(int i=0; i<100000; ++i) ;
    for(int i=100001; i<200000; ++i)
      a += i;
    printf(1, "test: pid = %d: About to release the lock. ticket_number = %d,\tturn = %d\n", getpid(), theLock.ticket_number, theLock.turn);
    lock_release(&theLock);

    int child2[4];
    for(int i=0; i<4; ++i){
      child2[i] = thread_join();
      if(child[i] != child2[i])
        printf(2, "Error! child[%d] = %d; child2[%d] = %d\n", i, child[i], i, child2[i]);
    }
  }
  else{

  }
  exit();
}
