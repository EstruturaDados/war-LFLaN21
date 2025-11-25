# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJS = main.o missao.o
TARGET = war_missoes

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c missao.h territorio.h
	$(CC) $(CFLAGS) -c main.c

missao.o: missao.c missao.h territorio.h
	$(CC) $(CFLAGS) -c missao.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
