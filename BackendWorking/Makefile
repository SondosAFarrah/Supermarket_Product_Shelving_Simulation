CC = gcc
CFLAGS = -pthread
OPENGLFLAG = -lglut -lGLU -lGL -lm -lrt
FF = -g -Wall

all: customer team main storageguard openGL

customer: customer.c
	$(CC) $^ -o $@

team: team.c
	$(CC) $(CFLAGS) $^ -o $@

main: main.c
	$(CC) $(CFLAGS) $^ -o $@

storageguard: storageguard.c
	$(CC) $(CFLAGS) $^ -o $@

openGL: openGL.c
	$(CC) $(FF) -o openGL openGL.c $(OPENGLFLAG) $(CFLAGS)



clean:
	rm -f customer team main storageguard openGL
