# Usual CFLAGS along with a few others that may be needed to build libraries
# OPTFLAGS lets people augment the process
# Example: 'make OPTFLAGS=-pthread'
CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
# These options are used to build a library
# OPTLIBS allows someone to augment the linking options
LIBS=-ldl $(OPTLIBS)
# OPTIONAL variable
# is only set if not defined
# Example: 'make PREFIX=/tmp install' installs the library into /tmp
PREFIX?=/usr/local

# Gets all source files in the 'src' directory
SOURCES=$(wildcard src/**/*.c src/*.c)
# Make '.o' files from the source files
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

# Find all test source files
TEST_SRC=$(wildcard tests/*_tests.c)
# Gets all the test-targets
TESTS=$(patsubst %.c,%,$(TEST_SRC))

# The 'ultimate target'
TARGET=build/libsample.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# The target build
# Default when running 'make'
# Build target then tests 
all: $(TARGET) $(SO_TARGET) tests

# Target for running a 'developer build'
dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

# Build the target itself
# Additional flag for 'Position Indipendant Code'
$(TARGET): CFLAGS += -fPIC
# Make build directory then compile the objects
$(TARGET): build $(OBJECTS)
# Actually makes the target, $@ maps back to TARGET
	ar rcs $@ $(OBJECTS)
# Make the library
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

# Make directories
build:
	@mkdir -p build
	@mkdir -p bin

# Unit Tests
# PHONY ignores the 'tests' directory
.PHONY: tests
# Also link my library for the tests
tests: CFLAGS += $(TARGET)
# Build the tests then run this shell script
tests: $(TESTS)
	sh ./tests/runtests.sh

# Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -rf tests/tests.log
	find . -name "*.gc" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# Install
# Depends on 'all'
install: all
# create the directory for install
	install -d $(DESTDIR)/$(PREFIX)/lib/
# install the target
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The checker
# Digs out use of bad functions with known bugs (mostly unprotected buffer functions)
check:
	@echo Files with potentially dangerous functions.
	@egrep -Hn '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true
		
