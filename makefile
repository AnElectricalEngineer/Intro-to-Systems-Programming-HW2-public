# Makefile for Airport program
CC=gcc
CFLAGS= -g -Wall -std=c99
CCLiNK= $(CC)
OBJS = main.o airport.o runway.o flight.o
RM = rm -f



#Link the final executable 
airport: $(OBJS)
	$(CCLiNK) -o airport $(OBJS)
	
# Creating object files using default rules
main.o: main.c flight.h ex2.h runway.h airport.h
airport.o: airport.c flight.h ex2.h runway.h airport.h
runway.o: runway.c flight.h ex2.h runway.h
flight.o: flight.c flight.h ex2.h

# Cleaning old files before new make 
clean:
	$(RM) airport  *.o
