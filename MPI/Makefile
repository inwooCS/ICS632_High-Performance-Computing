CC=mpicc
CFLAGS=-O3 -march=native
TARGET=brute

$(TARGET): mpi_brute.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(TARGET)
