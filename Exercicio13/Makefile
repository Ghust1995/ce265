COMP=-c

CC=gcc
CCFLAGS=-O3
CCLOAD= $(CCFLAGS) -lm

EXEC=Crivo.exe

OBJ=\
	wall_time.o \
	Crivo.o

all:	$(EXEC)


$(EXEC):	$(OBJ)
	$(CC) -o $(EXEC) $(CCLOAD) $(OBJ)

clean:
	rm -f *.o $(EXEC) *.lst


.SUFFIXES	:	.o .c

.c.o:
	$(CC) $(COMP) $(CCFLAGS) $*.c 



