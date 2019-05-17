CC     = gcc
CFLAGS = -Wall -std=c99 -g
# modify the flags here ^
EXE    = crack
OBJ    = proj-2_sha256.o sha256-helper.o passwords.o four_password_strategy.o six_password_strategy.o decrypt.o hashtable.o
# add any new object files here ^
# top (default) target
all: $(EXE)

# how to link executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm

# ^ add any new dependencies here (for example if you add new modules)


# phony targets (these targets do not represent actual files)
.PHONY: clean cleanly all CLEAN

# `make clean` to remove all object files
# `make CLEAN` to remove all object and executable files
# `make cleanly` to `make` then immediately remove object files (inefficient)
clean:
	rm -f $(OBJ)
CLEAN: clean
	rm -f $(EXE)
cleanly: all clean