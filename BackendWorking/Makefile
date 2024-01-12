CC = gcc
CFLAGS = -pthread

all: customer team main storageguard

customer: customer.c
	$(CC) $^ -o $@

team: team.c
	$(CC) $(CFLAGS) $^ -o $@

main: main.c
	$(CC) $(CFLAGS) $^ -o $@

storageguard: storageguard.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f customer team main storageguard
