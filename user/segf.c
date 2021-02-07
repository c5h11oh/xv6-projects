#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  uint* a = NULL;
  printf(1, "a = NULL; *a = %d\n", *a);
  a = (uint *)0x0;
  printf(1, "a = 0x0; *a = %d\n", *a);
  a = (uint *)0x80100000;
  printf(1, "a = 0x80100000; *a = %d\n", *a);
  a = (uint *)0x80000000;
  printf(1, "a = 0x80000000; *a = %d\n", *a);
  a = (uint *)0xFFFFFFFF;
  printf(1, "a = 0xFFFFFFFF; *a = %d\n", *a);
  exit();
}
