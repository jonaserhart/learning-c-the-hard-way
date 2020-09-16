#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct Person {
  char* name;
  int height;
  int weight;
  int age;
};

struct Person create_person_on_stack(char* name, int age, int height, int weight){

  struct Person who;

  who.name = name;
  who.age = age;
  who.height = height;
  who.weight = weight;

  return who;
}

void print_person_from_stack(struct Person who){

  printf("Name: %s\n", who.name);
  printf("Age: %d\n", who.age);
  printf("Height: %d\n", who.height);
  printf("Weight: %d\n\n", who.weight);

}

struct Person *create_person(char* name, int age, int height, int weight){

  struct Person *who = malloc(sizeof(struct Person));

  assert(who != NULL);

  who->name = strdup(name);
  who->age = age;
  who->height = height;
  who->weight = weight;

  return who;
}

void destroy_person(struct Person* who){

  assert(who != NULL);
  free(who->name);
  free(who);
}

void print_person(struct Person* who){

  printf("Name: %s\n", who->name);
  printf("Age: %d\n", who->age);
  printf("Height: %d\n", who->height);
  printf("Weight: %d\n\n", who->weight);
}

int main (int argc, char* argv[]){

  struct Person *joe = create_person("Joe Alex", 21, 180, 80);
  struct Person *frank = create_person("Frank Viola", 45, 178, 90);

  struct Person jonas = create_person_on_stack("Jonas Erhart", 21, 181, 70);
  
  printf("Joe is at memory location %p.\n", joe);
  printf("size of joe is %lu\n", sizeof(*joe));
  print_person(joe);

  printf("Frank is at memory location %p.\n", frank);
  printf("size of frank is %lu\n", sizeof(*frank));
  print_person(frank);

  printf("Jonas is at memory location %p.\n", &jonas);
  printf("size of jonas is %lu\n", sizeof(jonas));
  print_person_from_stack(jonas);

  joe->age += 20;
  frank->age += 10;
  jonas.age += 1;

  print_person(joe);
  print_person(frank);
  print_person_from_stack(jonas);

  destroy_person(joe);
  destroy_person(frank);
  //no need to free jonas

  return 0; 
  
}
