# Makefile para o código paralelo

CC = gcc
CFLAGS = -Wall -O2 -pthread
TARGET = matriz_paralela

# Regras de compilação
all: $(TARGET)

matriz.o: matriz.c matriz.h
	$(CC) $(CFLAGS) -c matriz.c

main_par.o: main_par.c matriz.h
	$(CC) $(CFLAGS) -c main_par.c

$(TARGET): main_par.o matriz.o
	$(CC) $(CFLAGS) main_par.o matriz.o -o $(TARGET)

clean:
	rm -f *.o $(TARGET)
