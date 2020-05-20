#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  int *sched;
  int **buf;

  FILE *fp;
  char ch[100];
  unsigned long int bsize=0;
  fp=fopen(argv[1], "r");
  if(fp==NULL){
   printf("cant open file %s\n", argv[1]);
   exit(1);
  }
  fseek(fp, 0L, SEEK_END);
  int sz = ftell(fp);
  buf=malloc(sizeof(int)*sz);
  fseek(fp, 0L, SEEK_SET);
  int count=0;
  while(1==fscanf(fp, " %s", &ch)){
      if(count==0){
        buf[bsize]=malloc(sizeof(int)*4);
      }
      buf[bsize][count++]=atoi(ch);
      if(count==3){
        bsize++;
        count=0;
      }
  }
  unsigned long int time=0;
  unsigned long int sindex=0;
  unsigned long int dindex=0;
  int tu[bsize];
  sched=malloc(sizeof(int)*sz);
  for(int i=0;i<bsize;i++) tu[i]=0;
  for(;dindex<bsize;time++){
    int smallest=9999;
    for(int i=0;i<bsize;i++){
      if(buf[i][0]>time || buf[i][2]<=0) continue;
      if(smallest==9999 || tu[smallest]>tu[i]) smallest=i;
    }
    if(smallest==9999){
      sched[sindex++]=-1; 
      continue; //I really dont understand why i need this
    }
    tu[smallest]++;
    buf[smallest][2]--;
    sched[sindex++]=buf[smallest][1];
    if(buf[smallest][2]<=0) dindex++;
  }
  for(unsigned long int i=0;sched[i];i++){
    if(i<10) printf("   %d  %d\n", i, sched[i]);
    if(i>=10&&i<100) printf("  %d  %d\n", i, sched[i]);
    if(i>=100&&i<1000) printf(" %d  %d\n", i, sched[i]);
    if(i>=1000&&i<10000) printf("%d  %d\n", i, sched[i]);
  }
  return 0;
}