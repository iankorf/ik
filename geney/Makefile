# Makefile for geney

LIB = -lm -lik
LINC = -L..
INC = -I..
CFLAGS = -O2
OBJECTS =

APP = geney
OBJ = geney.o

default:
	make $(APP)

$(APP): $(OBJ) $(OBJECTS)
	$(CC) -o $(APP) $(OBJ) $(OBJECTS) $(LINC) $(LIB)

clean:
	rm -f *.o $(APP)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
