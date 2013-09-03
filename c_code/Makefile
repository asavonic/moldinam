 # Library Makefile
# -------------------------------------------------------------------
NAME        =    moldinam
LIBNAME     =    lib$(NAME).so
ARCHIVE     =    lib$(NAME).a
BINARY		=	 moldinam

DEBUG       =    -DDEBUG -ggdb
OPT         =    -O2
ERR         =    -Wall
INC_PATH    =    -I.
LIB_PATH    =   

#----------------------------------------------------------------
CC          =    gcc
LD          =    ld
AR          =    ar rc
RANLIB      =    ranlib
RM          =    rm -f

LIBS        =    -lm -lcheck
CC_FLAGS    =     $(INC_PATH) $(DEBUG) $(OPT) $(ERR) -fPIC -std=gnu99
LD_FLAGS    =    $(LIB_PATH) $(LIBS) -shared -soname=$(LIBNAME)
LD_FLAGS_BIN =    $(LIB_PATH) $(LIBS)

		   

SRC         =     moldinam.c

OBJ         =     moldinam.o

#---------------------------------------------------------- Targets
all: $(LIBNAME)
.PHONY: all clean check

$(ARCHIVE): $(OBJ)
	$(AR) $(ARCHIVE) $^
	$(RANLIB) $(ARCHIVE)

$(LIBNAME): $(ARCHIVE)
	$(LD) $(LD_FLAGS) --whole-archive $< --no-whole-archive -o $@

.c.o: $(SRC)
	$(CC) $(CC_FLAGS) -o $@ -c $<
	$(CC) $(CC_FLAGS) $(LD_FLAGS_BIN) -o $(BINARY) $@

clean:
	[ -f $(LIBNAME) ] && $(RM) $(LIBNAME)|| [ 1 ]
	[ -f $(ARCHIVE) ] && $(RM) $(ARCHIVE)|| [ 1 ]
	[ -f $(OBJ) ] && $(RM) $(OBJ) || [ 1 ]
	cd tests && make clean

check: $(LIBNAME)
	cd tests && make && make check