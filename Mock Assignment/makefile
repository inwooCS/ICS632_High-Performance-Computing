# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -Ofast makes it fast!
CFLAGS  = -Ofast

# the build target executable:
TARGET = brute

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c 

clean:
	$(RM) $(TARGET) 