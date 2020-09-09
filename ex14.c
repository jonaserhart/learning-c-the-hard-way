#include <stdio.h>

int main(void){

  int ages[] = { 23, 43, 12, 89, 2};
  char *names[] = {
		     "Alan", "Frank",
		     "Mary", "John", "Lisa"
  };

  int count = sizeof(ages) / sizeof(int);
  int i = 0;

  for(i = 0; i < count; i++)
    printf("%s is %d years alive\n", names[i], ages[i]);

  printf("~~~~\n");

  int *cur_age = ages;
  char **cur_name = names;

  for (int i = 0; i < count; i++)
    printf("%s is %d years old\n", *(cur_name + i), *(cur_age +i));


  printf("~~~~\n");
  
  //stupid way to use pointers
  for (cur_name = names, cur_age = ages; (cur_age - ages) < count; cur_name++, cur_age++)
    printf("%s lived %d years so far\n", *cur_name, *cur_age);

  return 0;

}
