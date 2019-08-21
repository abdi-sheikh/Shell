# See Makefile-commented for explanation
CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

commando : commando.o util.o cmd.o cmdctl.o
	$(CC) -o commando util.o commando.o cmd.o cmdctl.o

commando.o : commando.c commando.h
	$(CC) -c commando.c

cmd.o : cmd.c
	$(CC) -c $<

cmdctl.o : cmdctl.c
	$(CC) -c $<

util.o : util.c
	$(CC) -c $<

binary_tests.o : binary_tests.c commando.h tests.h
	$(CC) -c $<

test_utils.o : test_utils.c tests.h
	$(CC) -c $<

binary_tests : binary_tests.o cmd.o cmdctl.o test_utils.o
	$(CC) -o $@ $^

test-binary : binary_tests
		valgrind ./binary_tests

test-shell : commando
		./shell_tests.sh

clean:
		@echo Cleaning up object files
		rm -f *.o actual.txt expect.txt valgrind.txt
