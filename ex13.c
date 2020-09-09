#include <stdio.h>

int main(int argc, char* argv[]){

  for (int i = 1; i < argc; i++)
    printf("argument %d: %s\n", i, argv[i]);

  char* states[] = {
		    "California", "Oregon",
		    "Washington", "Texas"
  };

  int num_states = 4;

  for(int i = 0; i < num_states; i++)
    printf("state %d: %s\n", i, states[i]);

  return 0;
  
}


