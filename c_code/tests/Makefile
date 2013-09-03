 # Unit-test Makefile
#--------------------------------------------------------- Definitions
TGT_NAME    =    moldinam
TGT_SRC     =    ../moldinam.c

OPT         =    -O2 -fprofile-arcs -ftest-coverage
ERR         =    -Wall
INC_PATH    =    -I. -I..
LIB_PATH    =    -L..
LD_PATH     =     ..

#---------------------------------------------------------
CC          =     gcc
RM          =    rm -f

# NOTE: check libs must be enclosed by --whole-archive directives
CHECK_LIBS  =    -Wl,--whole-archive -lcheck -Wl,--no-whole-archive
LIBS        =    -lm $(CHECK_LIBS)  

# NOTE: UNIT_TEST enables the static-function test case in moldinam.c
CC_FLAGS    =     $(INC_PATH) $(OPT) $(ERR) -DUNIT_TEST -std=gnu99
# NOTE: check libs must be enclosed by --whole-archive directives
LD_FLAGS    =    $(LIB_PATH)


# Test Definitions (to be added later)

# Test 1 : Simple test to ensure that linking against the library succeeds
TEST1        =    check_molecule
TEST1_SRC    =    check_molecule.c

TEST1_FLAGS  =    $(CC_FLAGS) $(LD_FLAGS) -g
TEST1_LIBS   =    $(LIBS) -l$(TGT_NAME)

# ---------------------------------------------
# Dinamic tests

TEST2        =    dinamicTest
TEST2_SRC    =    dinamicTest.c

TEST2_FLAGS  =    $(CC_FLAGS) $(LD_FLAGS) -g
TEST2_LIBS   =    $(LIBS) -l$(TGT_NAME)



TESTS        =    $(TEST1) $(TEST2)


#---------------------------------------------------------- Targets

all: $(TESTS)
.PHONY: all clean check

$(TEST1): $(TEST1_SRC)
	$(CC) $(TEST1_FLAGS) -o $@ $^ $(TEST1_LIBS)

$(TEST2): $(TEST2_SRC)
	$(CC) $(TEST2_FLAGS) -o $@ $^ $(TEST2_LIBS)

clean:
	$(RM) $(TESTS) *.gcno *.gcda


check:
		@for t in $(TESTS); do                          \
				LD_LIBRARY_PATH='$(LD_PATH)' ./$$t;     \
		done