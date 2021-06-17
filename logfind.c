#include <dirent.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// debug makros
#include "dbg.h"

#define MAXWORDLENGTH 255

const char* PATHS[] = {
	"/var/log",
};

int and (int x, int y) {
	return (x && y);
}

int or (int x, int y) {
	return (x || y);
}

typedef int (*operator_func)(int x, int y);

typedef struct LinesAndWords {
	int lineNo;
	char* word;
} LineWord_t;

typedef struct File_Result {
	char* fileName;
	int is_match;
	LineWord_t* lineWords;
	int result_size;
} Result_t;

int check_file_aux(FILE* file, char* wordsToFind[], int index, operator_func op, Result_t* result,
                   int lineWordIndex) {

	if (wordsToFind[index] == NULL) goto cont;

	int line_number = 0;
	int found = 0;
	char* line = NULL;
	size_t length = 0;
	ssize_t read = 0;

	// set file pointer to start of file
	fseek(file, 0, SEEK_SET);

	while ((read = getline(&line, &length, file)) != -1) {
		if (strstr(line, wordsToFind[index])) {
			LineWord_t lw = { .lineNo = line_number + 1, .word = wordsToFind[index] };
			result->lineWords[lineWordIndex] = lw;
			found = 1;
			lineWordIndex++;
			result->result_size++;
			result->lineWords =
			    realloc(result->lineWords, sizeof(LineWord_t) * (lineWordIndex + 1));
		}
		line_number++;
	}
	return op(check_file_aux(file, wordsToFind, ++index, op, result, lineWordIndex), found);
cont:
	if (op(0, 1) == 1)
		return (0);
	else
		return (1);
}

void print_result(Result_t result) {
	LineWord_t lw = result.lineWords[0];
	int size = result.result_size;
	if ((result.is_match) == 1 && size > 0) {
		printf("\n\n-------------------------------------------------------------------------------"
		       "-------------------------------");
		log_info("Result for file '%s'\n", result.fileName);
		int i = 0;
		while (i < size) {
			log_info("Found word '%s' at line number '%d'", lw.word, lw.lineNo);
			lw = result.lineWords[++i];
		}
	}
}

int check_file(char* full_path, FILE* file, char* wordsToFind[], operator_func op) {
	Result_t* result = malloc(sizeof(Result_t));
	result->fileName = malloc(sizeof(full_path) * sizeof(char));
	result->fileName = full_path;
	result->lineWords = malloc(sizeof(LineWord_t));
	result->result_size = 0;
	int rc = check_file_aux(file, wordsToFind, 0, op, result, 0);

	result->is_match = rc;

	print_result(*result);

	if (result) {
		free(result);
	}

	return rc;
}

int check_directory(const char* path, char* wordsToFind[], operator_func op, int checked_files) {
	struct dirent* dir_entry;
	DIR* dir = NULL;
	FILE* file = NULL;
	char* full_path = NULL;
	int rc = 0;

	dir = opendir(path);

	check(dir != NULL, "Cannot open directory %s", path);

	while ((dir_entry = readdir(dir)) != NULL) {
		char* full_path =
		    calloc(sizeof(*path) / sizeof(char) + sizeof(dir_entry->d_name) / sizeof(char) + 2,
		           sizeof(char));
		check_mem(full_path);

		strcat(full_path, path);
		strcat(full_path, "/");
		strcat(full_path, dir_entry->d_name);

		if (dir_entry->d_type != DT_DIR) {

			file = fopen(full_path, "r");
			check_continue(file != NULL, "File '%s'  could not be opened, are you sudo?",
			               full_path);

			check_file(full_path, file, wordsToFind, op);
			checked_files++;

			goto cont;
		cont:
			if (file) rc = fclose(file);
			check(rc == 0, "Failed to close file stream");
			free(full_path);
		} else {
			if (strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0)
				checked_files += check_directory(full_path, wordsToFind, op, checked_files);
		}
	}
	closedir(dir);
	return checked_files;
error:
	if (full_path) free(full_path);
	if (file) fclose(file);
	if (dir) closedir(dir);
	return checked_files;
}

int main(int argc, char* argv[]) {

	check(argc >= 2, "USAGE: logfind [-o(optional)] [...words]");

	operator_func op = NULL;

	int wordIndex = 1;

	if (strcmp(argv[1], "-o") == 0) {
		op = or ;
		wordIndex = 2;
	} else {
		op = and;
	}

	check(op != NULL, "Could not set operation");

	int num_of_paths = sizeof(PATHS) / sizeof(char*);

	int files_checked = 0;
	clock_t start, end;
	double time;
	start = clock();
	for (int j = 0; j < num_of_paths; j++) {
		int checked_files = check_directory(PATHS[j], argv + wordIndex, op, 0);
		files_checked += checked_files;
	}
	end = clock();
	time = ((double)(end - start)) / CLOCKS_PER_SEC;
	log_info("%d files checked in %f seconds", files_checked, time);
	return 0;

error:
	return -1;
}
