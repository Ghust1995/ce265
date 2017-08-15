COMP=-c

#CC=icc
#CCFLAGS= -qopenmp -qopt-report-phase=openmp
CC=cl
CCFLAGS= -openmp
CCLOAD= $(CCFLAGS)

EXEC1=Tempo.exe
EXEC2=Funciona.exe

OBJ=\
	wall_time.o \
	ModVida.o

OBJ1= Tempo.o

OBJ2= Funciona.o

all:	$(EXEC1) $(EXEC2)


$(EXEC1):	$(OBJ) $(OBJ1)
	$(CC) -o $(EXEC1) $(CCLOAD) $(OBJ) $(OBJ1)

$(EXEC2):	$(OBJ) $(OBJ2)
	$(CC) -o $(EXEC2) $(CCLOAD) $(OBJ) $(OBJ2)

Tempo.o:	Tempo.c ModVida.o wall_time.o
Funciona.o:	Funciona.c ModVida.o

clean:
	rm -f *.o $(EXEC1) $(EXEC2)


.SUFFIXES	:	.o .c

.c.o:
	$(CC) $(COMP) $(CCFLAGS) $*.c 



