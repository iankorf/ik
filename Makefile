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

APP = ik-test
SRC = ik-test.c
OBJ = ik-test.o

###########
# Targets #
###########

default:
	make $(ARC)
	make $(APP)

$(ARC): $(OBJECTS)
	ar rvs $(ARC) $(OBJECTS)

$(APP): $(OBJ) $(OBJECTS)
	$(CC) -o $(APP) $(OBJ) $(OBJECTS) $(LIB)

clean:
	rm -f *.o $(APP) $(ARC)
	cd dusty && make clean
	cd smithy && make clean
	cd geney && make clean

depend: $(OBJECTS:.o=.c)
	gcc -MM $^ > $@

test: $(APP)
	./ik-test -all

all: $(ARC) $(APP)
	cd dusty && make
	cd smithy && make
	cd geney && make

###################
# Inference Rules #
###################

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

################
# Dependancies #
################

include depend
