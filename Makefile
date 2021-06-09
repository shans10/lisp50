# -*- MakeFile -*- #

TARGET = lisp50

# Windows does not have editline library so do not include '-ledit' flag when compiling on windows.
ifdef OS
	LIBS = -lm
else
	LIBS = -lm -ledit
endif

CC = gcc
CFLAGS = -g -Wall -MMD -MP -std=c99

default: $(TARGET)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
DEPS = $(OBJECTS:.o=.d)

-include $(DEPS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	    $(CC) $(OBJECTS) -Wall $(LIBS) -o $@

.PHONY: clean

# Specifying remove file command for Windows(Powershell) and Unix/Linux
ifeq ($(OS),Windows_NT)
RM = del /Q /F
CP = copy /Y
EXEC = $(TARGET).exe
ifdef ComSpec
SHELL := $(ComSpec)
endif
ifdef COMSPEC
SHELL := $(COMSPEC)
endif
else
RM = rm -rf
CP = cp -f
EXEC = $(TARGET)
endif

clean:
	-$(RM) $(EXEC) *.d *.o
