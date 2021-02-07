#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"
#include "spinlock.h"

struct spinlock sbrklock;
void
sbrkinit(void)
{
  initlock(&sbrklock, "sbrk");
}

int
sys_clone(void)
{
  void(*fcn)(void *, void *);
  void *arg1, *arg2, *stack;

  if(argptr(0, (char **)&fcn, sizeof(uint)) < 0)
    return -1;
  if(argptr(1, (char **)&arg1, sizeof(uint)) < 0)
    return -1;
  if(argptr(2, (char **)&arg2, sizeof(uint)) < 0)
    return -1;
  if(argptr(3, (char **)&stack, sizeof(uint)) < 0)
    return -1;
  return clone(fcn, arg1, arg2, stack);
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_join(void)
{
  void **stack;
  if(argptr(0, (char**)&stack, sizeof(uint)) < 0)
    return -1;
  return join(stack);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;
  int failed = 0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&sbrklock);
  addr = proc->sz;
  if(growproc(n) < 0)
    failed = 1;
  release(&sbrklock);
  if(failed)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
