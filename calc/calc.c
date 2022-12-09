#include <stdio.h>

#ifdef SELF
int main() {
  printf("%d\n", SELF);
  return 0;
}
#endif
