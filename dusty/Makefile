# Makefile for dusty

LIB = -lm -lik
INC = -L.. -I..
CFLAGS = -O2 -Wall -Werror
OBJECTS = 

APP = dusty
OBJ = dusty.o

default:
	make $(APP)

$(APP): $(OBJ) $(OBJECTS)
	$(CC) -o $(APP) $(OBJ) $(OBJECTS) $(INC) $(LIB)

clean:
	rm -f *.o $(APP)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
