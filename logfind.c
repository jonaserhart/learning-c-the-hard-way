#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>

#include "dbg.h"

const char * PATHS[] = {
			"/home/jonas/dev/logfilestest"
};

int and(int x, int y){
  return (x && y);
}

int or(int x, int y){
  return (x || y);
}

typedef int (*operator_func) (int x, int y);

int check_file_aux(FILE* file, char* wordsToFind[], int index, operator_func op){

  check_continue(wordsToFind[index] != NULL, "Reading for file finished");

  int line_number = 0;
  int found = 0;
  char *line = NULL;
  size_t length = 0;
  ssize_t read = 0;

  //set file pointer to start of file
  fseek(file, 0, SEEK_SET);
  
  while((read = getline(&line, &length, file)) != -1){
    if(strstr(line, wordsToFind[index])){
      log_info("found '%s': line %d",wordsToFind[index], line_number+1);
      found = 1;
    }
    line_number++;
  }
  return op(check_file_aux(file, wordsToFind, ++index, op), found);
 cont:
  if (op(0,1) == 1)
    return(0);
  else
    return(1);
}

int check_file(FILE* file, char* wordsTofind[], operator_func op){
  return check_file_aux(file, wordsTofind, 0, op);
}

int check_directory(const char* path, char* wordsToFind[], operator_func op){
  struct dirent *dir_entry;
  DIR *dir = NULL;
  FILE *file = NULL;
  char *full_path = NULL;
  int rc = 0;
  
  dir = opendir(path);

  check(dir != NULL, "Cannot open directory %s", path);

  while((dir_entry = readdir(dir)) != NULL){
    char *full_path = calloc(
			     sizeof(*path)/sizeof(char)
			     + sizeof(dir_entry->d_name)/sizeof(char) + 2,
			     sizeof(char));
    check_mem(full_path);
    
    strcat(full_path, path);
    strcat(full_path, "/");
    strcat(full_path, dir_entry->d_name);

    if (dir_entry->d_type != DT_DIR){

      file = fopen(full_path, "r");
      check_continue(file != NULL, "File '%s'  could not be opened", full_path);
	
      log_info("Searching in file '%s'", full_path);

      int result = check_file(file, wordsToFind, op);

      if (result)
	log_info("File was a match");
      goto cont;
    cont:
      rc = fclose(file);
      check(rc == 0, "Failed to close file stream");
      free(full_path);
    }
    else{
      if(strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0)
	check_directory(full_path, wordsToFind, op);
    }
  }
    closedir(dir);
    return 0;
 error:
    if (full_path) free(full_path);
    if(file) fclose(file);
    if(dir) closedir(dir);
    return -1;
}

int main (int argc, char* argv[]){
  
  check(argc >= 2, "USAGE: logfind [-o(optional)] [...words]");

  operator_func op = NULL;

  int wordIndex = 1;
  
  if(strcmp(argv[1], "-o") == 0){
    op = or;
    wordIndex = 2;
  }
  else{
    op = and;
  }

  check(op != NULL, "Could not set operation");

  int num_of_paths = sizeof(PATHS)/sizeof(char*);
  
  for (int j = 0; j < num_of_paths; j++){
    check_directory(PATHS[j], argv + wordIndex, op);
  }   
  return 0;

 error:
  return -1;
}


