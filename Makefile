CC = gcc
LIBS = -ledit -lm
CFLAGS  = -g -Wall

# the build target executable:
TARGET = variables

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c mpc.c $(LIBS) -o lispy

clean:
	$(RM) $(TARGET)
