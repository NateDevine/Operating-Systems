#include <stdio.h>
#include <stdlib.h>

#define MAXSHOW  25


unsigned long nextpower2(unsigned long n);

int main(int argc, char *argv[]){
  int i, reps;
  long m, n;
  
  srand48(2);             // everyone uses the same seed
  
  reps = (argc > 1) ? atol(argv[1]) : 10;
  
  for(i = 0 ; i < reps ; i++){
    n = lrand48() >> 1;
    m = nextpower2(n);
    if(reps <= MAXSHOW)
      printf("%20ld %20ld\n", n, m);
  }
}

unsigned long nextpower2(unsigned long n){
    long i=1;
    for(;i<=n;i*=2);
    return i;
}