#
#MAKEFILE tlb.c
#Autor: Gina Kokoska
#Datum: 14.11.21
#
#

CC = gcc
CFLAGS = -g -Wall -Wextra -std=c11
RM = rm -f

TARGET = tlb
OBJECTS = $(TARGET).o
SOURCES = $(TARGET).c
HEADERS = $(TARGET).h

$(TARGET): $(TARGET).o
        $(CC) $(CFLAGS) $(TARGET).c

tlb.o: $(TARGET).c $(TARGET).h
        $(CC) $(CFLAGS) -c $(TARGET).c -o $(TARGET).o 

# Pattern Rule
%.o: %.c
        $(CC) $(CFLAGS) -c $< -o $@

# STANDARD EXECUTION
.PHONY: all clean

all: $(TARGET) $(TARGET).o

clean:
        $(RM) $(TARGET) $(OBJECTS) depend

depend: $(SOURCES) $(HEADERS)
        $(CC) $(CFLAGS) -MM $(SOURCES) > $@

# PLACE OF POGRAM EXECUTION
$(TARGET): $(OBJECTS)
        $(CC) $(LDFLAGS) $^ -o $@ 

# Dependecies
include depend

