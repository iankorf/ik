########################
# Makefile for IK lib  #
########################

LIB = -lm

OBJECTS = \
	sequence.o\
	toolbox.o\
	align.o\

ARC = libik.a

APP1 = ik-test
SRC1 = ik-test.c
OBJ1 = ik-test.o

APP2 = edist
SRC2 = edist.c
OBJ2 = edist.o

DATE = $(shell date +\%Y-\%m-\%d)

###########
# Targets #
###########

default:
	make gcc

$(ARC): $(OBJECTS)
	ar rvs $(ARC) $(OBJECTS)

$(APP1): $(OBJ1) $(OBJECTS)
	$(CC) -o $(APP1) $(CFLAGS) $(OBJ1) $(OBJECTS) $(LIB)

$(APP2): $(OBJ2) $(OBJECTS)
	$(CC) -o $(APP2) $(CFLAGS) $(OBJ2) $(OBJECTS) $(LIB)

clean:
	rm -f *.o $(APP1) $(APP2) $(ARC)

depend: $(OBJECTS:.o=.c)
	gcc -MM $^ > $@

test: $(APP1)
	./ik-test -all


#################
# Architectures #
#################

gcc:
	make $(ARC)
	make $(APP1) CC="gcc" CFLAGS="-O2 -Wall -Werror"
	make $(APP2) CC="gcc" CFLAGS="-O2 -Wall -Werror"

###################
# Inference Rules #
###################

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

################
# Dependancies #
################

include depend

