#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>

#include "dbg.h"

const char * PATHS[] = {
			"/var/log",
			"/home/jonas/dev/learning-c-the-hard-way"
};

int and(int x, int y){
  return (x && y);
}

int or(int x, int y){
  return (x || y);
}

typedef int (*operator_func) (int x, int y);

int main (int argc, char* argv[]){
  
  check(argc <= 3, "USAGE: logfind [-o(optional)] ...words");

  operator_func op = NULL;
  
  if(strcmp(argv[1], "-o") == 0){
    op = or;
  }
  else{
    op = and;
  }

  check(op != NULL, "Could not set operation");

  struct dirent *dir_entry;
  DIR *dir;

  int num_of_paths = sizeof(PATHS)/sizeof(char*);
  int i = 0;
  char **files;
  for (int j = 0; j < num_of_paths; j++){
  
    dir = opendir(PATHS[j]);

    check(dir != NULL, "Cannot open directory %s", PATHS[j]);

    files = (char**)calloc(100, sizeof(char**));
    i = 0;
    while((dir_entry = readdir(dir)) != NULL){
      if (dir_entry->d_type != DT_DIR){ 
	files[i] = (char*)malloc(sizeof(char)*(strlen(dir_entry->d_name) + 1));
	check_mem(files[i]);
	strcpy(files[i], dir_entry->d_name);
	i++;
	files = (char**)reallocarray(files, i+1, sizeof(char**));
      }
    }

    for (int j = 0; j < i; j++){
      log_info("Searching in file: %s", files[j]);
    }
  }

  closedir(dir);

  for(int j = 0; j < i; j++){
    free(files[j]);
  }
  
  return 0;

 error:
  if (dir) closedir(dir);
  if (files){
    for(int j = 0; j < i; j++){
      free(files[j]);
    }
  }
  return -1;
}
