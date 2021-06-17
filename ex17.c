#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};

struct Database {
	struct Address rows[MAX_ROWS];
};

struct Connection {
	FILE* file;
	struct Database* db;
};

void die(const char* message) {
	if (errno)
		perror(message);
	else
		printf("ERROR: %s", message);
	exit(1);
}

void print_address(struct Address* address) {
	printf("%d %s %s\n", address->id, address->name, address->email);
}

void load_database(struct Connection* conn) {
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) die("Failed to load database\n");
}

struct Connection* open_database(const char* filename, char mode) {
	struct Connection* conn = malloc(sizeof(struct Connection));
	if (!conn) die("Memory error\n");

	conn->db = malloc(sizeof(struct Database));

	if (!conn->db) die("Memory error\n");

	if (mode == 'c')
		conn->file = fopen(filename, "w");
	else {
		conn->file = fopen(filename, "r+");

		if (conn->file) {
			load_database(conn);
		}
	}

	if (!conn->file) die("Failed to open files\n");

	return conn;
}

void close_database(struct Connection* conn) {
	if (conn) {
		if (conn->file) fclose(conn->file);
		if (conn->db) free(conn->db);
		free(conn);
	}
}

void write_database(struct Connection* conn) {
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) die("failed to write database\n");
	rc = fflush(conn->file);
	if (rc == -1) die("Cannot flush database\n");
}

void create_database(struct Connection* conn) {
	int i = 0;

	for (i = 0; i < MAX_ROWS; i++) {
		struct Address addr = { .id = i, .set = 0 };
		conn->db->rows[i] = addr;
	}
}

void set_database_row(struct Connection* conn, int id, const char* name, const char* email) {
	struct Address* addr = &conn->db->rows[id];
	if (addr->set) die("Already set, delete first\n");

	addr->set = 1;
	char* res = strncpy(addr->name, name, MAX_DATA);

	if (!res) die("Name copy not found\n");

	res = strncpy(addr->email, email, MAX_DATA);
	if (!res) die("Email copy failed\n");
}

void print_database_row(struct Connection* conn, int id) {
	struct Address* addr = &conn->db->rows[id];

	if (addr->set)
		print_address(addr);
	else
		die("ID does not exits (yet)");
}

void delete_database_row(struct Connection* conn, int id) {
	struct Address addr = { .id = id, .set = 0 };

	conn->db->rows[id] = addr;
}

void list_database_table(struct Connection* conn) {
	int i = 0;
	struct Database* db = conn->db;

	for (i = 0; i < MAX_ROWS; i++) {
		struct Address* current = &db->rows[i];
		if (current->set) print_address(current);
	}
}

int main(int argc, char* argv[]) {
	if (argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");

	char* filename = argv[1];
	char action = argv[2][0];
	struct Connection* connection = open_database(filename, action);

	int id = 0;

	if (argc > 3) id = atoi(argv[3]);
	if (id >= MAX_ROWS) die("there's not that many rows \n");

	switch (action) {
		case 'c':
			create_database(connection);
			write_database(connection);
			break;
		case 'g':
			if (argc != 4) die("need an id to get");

			print_database_row(connection, id);
			break;
		case 's':
			if (argc != 6) die("Need id, name, email to set");
			set_database_row(connection, id, argv[4], argv[5]);
			write_database(connection);
			break;
		case 'l': list_database_table(connection); break;
		default: die("not known action");
	}
	close_database(connection);
	return 0;
}
