# Caminho completo para o GCC com suporte a OpenMP
CC = gcc

CFLAGS = -Wall -O2 -fopenmp
LDFLAGS = -fopenmp
PTHREADFLAGS = -pthread

# Fontes
OBJS = matriz.o
MAIN_OBJ = main.o
OPENMP_OBJ = main_openmp.o
PTHREADS_OBJ = main_pthreads.o

# Alvos
TARGETS = main openmp_exec pthreads_exec

all: $(TARGETS)

main: $(MAIN_OBJ) $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

openmp_exec: $(OPENMP_OBJ) $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

pthreads_exec: $(PTHREADS_OBJ) $(OBJS)
	$(CC) $(CFLAGS) $(PTHREADFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *~ $(TARGETS)
