########################
# Makefile for IK lib  #
########################

LIB = -lm
CFLAGS = -O2 -Wall -Werror

OBJECTS = \
	feature.o\
	model.o\
	sequence.o\
	toolbox.o\

ARC = libik.a

APP1 = ik-test
SRC1 = ik-test.c
OBJ1 = ik-test.o

DATE = $(shell date +\%Y-\%m-\%d)

###########
# Targets #
###########

default:
	make gcc

$(ARC): $(OBJECTS)
	ar rvs $(ARC) $(OBJECTS)

$(APP1): $(OBJ1) $(OBJECTS)
	$(CC) -o $(APP1) $(OBJ1) $(OBJECTS) $(LIB)

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
	make $(APP1)

###################
# Inference Rules #
###################

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

################
# Dependancies #
################

include depend

