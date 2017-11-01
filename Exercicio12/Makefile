CC=mpicc
COMP=-c
CCFLAGS=
CCLOAD= $(CCFLAGS) -lm

EXEC1=SomaSeq.exe
EXEC2=SomaPar.exe

all: $(EXEC1) $(EXEC2)

OBJ1=SomaSeq.o
$(EXEC1):	$(OBJ1) 
	$(CC) -o $(EXEC1) $(CCLOAD) $(OBJ1) 

OBJ2=SomaPar.o
$(EXEC2):	$(OBJ2) 
	$(CC) -o $(EXEC2) $(CCLOAD) $(OBJ2) 

clean:
	rm -f *.o $(EXEC1) $(EXEC2)


.SUFFIXES	:	.o .c

.c.o:
	$(CC) $(COMP) $(CCFLAGS) $*.c

