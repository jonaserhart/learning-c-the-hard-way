#include <stdio.h>

int main() {

	int distance = 100;
	float power = 2.345f;
	double super_power = 456789.4532;
	char initial = 'A';
	char first_name[] = "First";
	char last_name[] = "Last";

	printf("You are %d miles aray.\n", distance);
	printf("You have %f levels of power.\n", power);
	printf("You have %f awesome super powers.\n", super_power);
	printf("I have an initial %c.\n", initial);
	printf("I have a first name: %s.\n", first_name);
	printf("I have a last name: %s.\n", last_name);
	printf("My full name is %s %c. %s.\n", first_name, initial, last_name);

	int bugs = 100;
	char nul_byte = '\0';

	int care_percentage = bugs * nul_byte;

	printf("I care %d%%.\n", care_percentage);
	printf("nul byte as string; %s.\n", nul_byte);
	printf("nul byte as char: %c.\n", nul_byte);
}
