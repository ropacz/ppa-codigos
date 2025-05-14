# Makefile para o código sequencial

CC = gcc
CFLAGS = -Wall -O2
TARGET = matriz_sequencial

# Regras de compilação
all: $(TARGET)

matriz.o: matriz.c matriz.h
	$(CC) $(CFLAGS) -c matriz.c

main_seq.o: main_seq.c matriz.h
	$(CC) $(CFLAGS) -c main_seq.c

$(TARGET): main_seq.o matriz.o
	$(CC) $(CFLAGS) main_seq.o matriz.o -o $(TARGET)

clean:
	rm -f *.o $(TARGET)
