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
#define FILENAME "GroupD"

typedef struct user {
  char *login;
  int time;
  int answer;
} user;

 int countLine() {
   int c;
   FILE *fp = fopen("/dev/shm/GroupD", "r");
   if (fp == NULL) return 0;
   int count = 0;
   for (c = getc(fp); c != EOF; c=getc(fp))
     if (c == '\n') count += 1;
   fclose(fp);
   //printf("%d\n", count);
   return count;
 }
 
 int checkIfAnswered(char *name){
//    
   int n = countLine();
   FILE *fp = fopen("/dev/shm/GroupD", "r");
   char login[16];
   int junk[2];
   for (int i = 0; i < n; i++){
     fscanf(fp, "%s %d %d", login, &junk[0], &junk[1]);
     if(!strcmp(login, name)) return 1;
   }
   return 0;
 }

int main(int argc, char * argv[]){
   if(argc<2) exit(0);
   umask(0);
   // Open our virtual file:
   int sfd = shm_open(FILENAME, O_RDWR | O_CREAT /*| O_TRUNC*/, 0666);
   struct stat st;
   if (fstat(sfd, &st) < 0) {
     perror("fstat");
   }
   //printf("size of the file is: %d\n", st.st_size);
   //ftruncate(sfd, 4096);
  
   void *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, sfd, 0);
   if (addr == NULL) {
     perror("mmap");
     return 0;
   }
   struct user u;
   u.time= time(0);
   char *name;
   struct passwd *pass;
   pass = getpwuid(getuid());
   name = pass->pw_name;
   u.login=name;
   u.answer=atoi(argv[1]);
    if(checkIfAnswered(u.login)){
      printf("you've alredy answered. Try again next question\n");
      exit(0);
    }
   lseek(sfd, 0, SEEK_END);
   dprintf(sfd, "%s %d %d\n", u.login, u.answer, u.time);
   return 1;
}