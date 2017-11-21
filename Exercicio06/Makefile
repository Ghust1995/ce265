COMP=-c

CC=mpicc
CCFLAGS=-O3
CCLOAD= $(CCFLAGS) -lm

EXEC=Pi

OBJ= 	wall_time.o\
	Pi.o

$(EXEC):	$(OBJ)
	$(CC) -o $(EXEC) $(CCLOAD) $(OBJ)

clean:
	rm -f *.o $(EXEC)


.SUFFIXES	:	.o .c

.c.o:
	$(CC) $(COMP) $(CCFLAGS) $*.c 



