#include <stdio.h>

int main(int argc, char* argv[]) {

	char full_name[] = {
		'J', 'o', 'n', 'a', 's', ' ', 'A', '.', ' ', 'E', 'r', 'h', 'a', 'r', 't'
	};
	int areas[] = { 10, 12, 13, 14, 20 };
	char name[] = "Jonas";

	printf("The size of an int : %ld\n", sizeof(int));
	printf("The size of areas (int[]): %ld\n", sizeof(areas));
	printf("The number of ints in areas: %ld\n", sizeof(areas) / sizeof(int));
	printf("The first area is %d, the 2nd %d .\n", areas[0], areas[1]);

	printf("The size of a char: %ld\n", sizeof(char));
	printf("The size of name: %ld\n", sizeof(name));
	printf("The number of letters in name: %ld\n", sizeof(name) / sizeof(char));

	printf("The size of full_name (char[]): %ld.\n", sizeof(full_name));
	printf("The number of chars in full_name: %ld.\n", sizeof(full_name) / sizeof(char));

	return 0;
}
