CC = gcc
CFLAGS =  -pthread

all: customer team main

customer: customer.c
	$(CC)  $^ -o $@

team: team.c
	$(CC) $(CFLAGS) $^ -o $@

main: main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f customer team main


