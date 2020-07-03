########################
# Makefile for IK lib  #
########################

LIB = -lm

OBJECTS = \
	sequence.o\
	toolbox.o\
	align.o\


APP1 = ik-test
SRC1 = ik-test.c
OBJ1 = ik-test.o

DATE = $(shell date +\%Y-\%m-\%d)

###########
# Targets #
###########

default:
	make gcc
	make archive

$(APP1): $(OBJ1) $(OBJECTS)
	$(CC) -o $(APP1) $(CFLAGS) $(OBJ1) $(OBJECTS) $(LIB)

clean:
	rm -f *.o $(APP1) libik.a

depend: $(OBJECTS:.o=.c)
	gcc -MM $^ > $@

test: $(APP1)
	./ik-test -all


#################
# Architectures #
#################

gcc:
	make $(APP1) CC="gcc" CFLAGS="-O2 -Wall -Werror"

archive:
	ar rvs libik.a $(OBJECTS)


###################
# Inference Rules #
###################

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

################
# Dependancies #
################

include depend

