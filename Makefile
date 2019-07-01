CC = gcc
CFLAGS = -pedantic -Werror -Wall -Wextra -std=c99 -lreadline
LDFLAGS =
EXEC = minishell

all: $(EXEC)

minishell: src/minishell.o
	$(CC) -o $@ $^ $(LDFLAGS)

minishell.o: src/minishell.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean
clean:
	$(RM) $(OBJ) minishell src/minishell.o

.PHONY: test
test:
	chmod 755 tests/test_suite.sh
	tests/./test_suite.sh
