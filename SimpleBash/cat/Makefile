CC = gcc
CFLAGS = -Wall -Werror -Wextra

all: s21_cat

s21_cat: rebuild
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

test: test_files/tests.sh
	bash test_files/tests.sh
	
clean:	
	rm -rf s21_cat

rebuild: clean