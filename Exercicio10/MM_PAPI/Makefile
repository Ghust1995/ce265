COMP=-c

CC=cc
CCFLAGS=-I/opt/bullxde/perftools/papi-bull/5.4.3.0/include 
CCLOAD= $(CCFLAGS) -L/opt/bullxde/perftools/papi-bull/5.4.3.0/lib64 -lpapi

EXEC=MM.exe

OBJ=\
	wall_time.o \
	MM.o

all:	$(EXEC)


$(EXEC):	$(OBJ)
	$(CC) -o $(EXEC) $(CCLOAD) $(OBJ)

MM.o:	MM.c wall_time.o

clean:
	rm -f *.o $(EXEC)


.SUFFIXES	:	.o .c

.c.o:
	$(CC) $(COMP) $(CCFLAGS) $*.c 



