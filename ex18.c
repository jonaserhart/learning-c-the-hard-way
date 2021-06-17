#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die(char* message) {
	if (errno)
		perror(message);
	else
		printf("ERROR: %s", message);
	exit(1);
}

typedef int (*compare_cb)(int a, int b);

int* bubble_sort(int* numbers, int count, compare_cb cmp) {
	int temp = 0;
	int i = 0;
	int j = 0;
	int* target = malloc(count * sizeof(int));

	if (cmp == NULL) die("not a valid compare function");

	if (!target) die("Memory error");

	memcpy(target, numbers, count * sizeof(int));

	for (i = 0; i < count; i++)
		for (j = 0; j < count - 1; j++)
			if (cmp(target[j], target[j + 1]) > 0) {
				temp = target[j + 1];
				target[j + 1] = target[j];
				target[j] = temp;
				temp = 0;
			}
	return target;
}

int sorted_order(int a, int b) {
	return a - b;
}

int reverse_order(int a, int b) {
	return b - a;
}

int strange_order(int a, int b) {
	if (a == 0 || b == 0)
		return 0;
	else
		return a % b;
}

void test_sort(int* numbers, int count, compare_cb cmp) {
	int i = 0;
	int* sorted = bubble_sort(numbers, count, cmp);

	if (!sorted) die("Failed to sort array of numbers");

	for (i = 0; i < count; i++)
		printf("%d ", sorted[i]);

	printf("\n");
	free(sorted);
}

int main(int argc, char* argv[]) {
	if (argc < 2) die("USAGE: ex18 4 3 1 5 6");
	int count = argc - 1;
	int i = 0;
	char** inputs = argv + 1;
	int* numbers = malloc(count * sizeof(int));
	if (!numbers) die("memory error");

	for (i = 0; i < count; i++)
		numbers[i] = atoi(inputs[i]);

	test_sort(numbers, count, sorted_order);
	test_sort(numbers, count, reverse_order);
	test_sort(numbers, count, strange_order);

	free(numbers);

	return 0;
}
