#include <stdio.h>
#include <dlfcn.h>
#include "dbg.h"

typedef int (*lib_function) (const char* data);

void makeassertion(int condition, char* message) {
    if (!condition) {
        printf("Failed: %s\n", message);
    }
}

int main() {
    int rc = 0;
    char *data;

    char *lib_file = "./build/libex29.so";
    char* functions_to_run[] = {
        "print_a_message",
        "uppercase",
        "lowercase",
        "fail_on_purpose"
    };

    void *lib = dlopen(lib_file, RTLD_NOW);
    check(lib != NULL, "Failed to open the library %s: %s", lib_file, dlerror());

    lib_function func = dlsym(lib, functions_to_run[0]);
    check(func != NULL, "Failed to load symbol %s from library %s: %s", functions_to_run[0], lib_file, dlerror());
    data = "testdata";
    rc = func(data);
    check(rc == 0, "print_a_message returned not 0");

    func = dlsym(lib, functions_to_run[1]);
    check(func != NULL, "Failed to load symbol %s from library %s: %s", functions_to_run[1], lib_file, dlerror());
    data = "testdata";
    rc = func(data);
    check(rc == 0, "uppercase returned not 0");

    func = dlsym(lib, functions_to_run[2]);
    check(func != NULL, "Failed to load symbol %s from library %s: %s", functions_to_run[2], lib_file, dlerror());
    data = "testdata";
    rc = func(data);
    check(rc == 0, "lowercase returned not 0");

    func = dlsym(lib, functions_to_run[3]);
    check(func != NULL, "Failed to load symbol %s from library %s: %s", functions_to_run[3], lib_file, dlerror());
    data = "testdata";
    rc = func(data);
    check(rc != 0, "fail_on_purpose returned 0");

    rc = dlclose(lib);
    check(rc == 0, "Failed to close %s", lib_file);
    
    return 0;
error:
    return -1;
}