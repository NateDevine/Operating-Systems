#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILENAME "/dev/shm/GroupD"

typedef struct user {
  char login[16];
  int time;
  int answer;
} USER;

int countLine() {
  int c;
  FILE *fp = fopen(FILENAME, "r");
  if (fp == NULL) return 0;
  int count = 0;
  for (c = getc(fp); c != EOF; c=getc(fp))
    if (c == '\n') count += 1;
  fclose(fp);
  return count;
}

void setUp() {
  int sfd = shm_open("GroupD", O_RDWR | O_CREAT /*| O_TRUNC*/, 0666);
  struct stat st;
  if (fstat(sfd, &st) < 0) {
    perror("fstat");
  }
  int n = countLine();
  USER data[n];
  FILE *fp = fopen(FILENAME, "r");
  for (int i = 0; i < n; i++){
    fscanf(fp, "%s %d %d", &data[i].login, &data[i].answer, &data[i].time);
    //if(smallestTime>data[i].time) smallestTime=data[i].time;
  }
  int smallestTime=data[0].time;
  fclose(fp);
  FILE *f=fopen("GroupD.log", "w");
  //printf("%d\n",(smallestTime));
  for(int i=0;i<n;i++)
    fprintf(f, "User: %s Score: %d Answer: %d\n", data[i].login, (100-(data[i].time-smallestTime)%100), data[i].answer);  
  fclose(f);
  
}

int main () {
  setUp();
  FILE *f=fopen(FILENAME, "w");
  fclose(f);
  return 0;
}