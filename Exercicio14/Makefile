COMP=-c 

CC=icc
CCFLAGS=-qopenmp 
CCLOAD= $(CCFLAGS) -lm

EXEC=Cholesky.exe

OBJ=\
	wall_time.o \
	BasicBlock.o \
	CholeskyOMP.o \
	CholeskyDF.o \
	MainCholesky.o

all:	$(EXEC)


$(EXEC):	$(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(CCLOAD) 


BasicBlock.o:	BasicBlock.c BasicBlock.h
CholeskyOMP.o:	CholeskyOMP.c BasicBlock.h CholeskyOMP.h wall_time.h
CholeskyDF.o:	CholeskyDF.c BasicBlock.h CholeskyDF.h wall_time.h
MainCholesky.o:	MainCholesky.c BasicBlock.o CholeskyOMP.o CholeskyDF.o

clean:
	rm -f *.o $(EXEC)


.SUFFIXES	:	.o .c

.c.o:
	$(CC) $(COMP) $(CCFLAGS) $*.c 



