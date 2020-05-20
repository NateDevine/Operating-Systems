#include <string.h>
#include <stdlib.h>

char **split(char *path){
  int len=strlen(path);
  char **done=malloc(sizeof(char)*len+1);
  char *temp=malloc(sizeof(char)*len);
  int i,j,k;
  for(i=0, j=0, k=0;path[i];i++){
    if(path[i]=='/'){
      char *cpy=malloc(sizeof(char)*j);
      if(i==0) continue;
      for(int l=0;l<j;l++) cpy[l]=temp[l];
      done[k]=malloc(sizeof(char) * j);
      strcpy(done[k++],cpy);
      j=0;
      continue;
    }
    temp[j++]=path[i];
  }
  done[k+1]=NULL;
  return done;
}

void freelist(char **list){
  for(int i=0;list[i];i++) free(list[i]);
  free(list);
}
