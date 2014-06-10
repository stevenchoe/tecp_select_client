CC = gcc
CFLAGS = -W -Wall -O2
LDFLAGS = -lm

all : tcp_select_client

tcp_select_client : tcp_select_client.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean :
	rm -rf *.o
