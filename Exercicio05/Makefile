CC=nvcc -arch=sm_30 -O3 
COMP=-c
CCFLAGS=
CCLOAD= $(CCFLAGS)

EXEC=Vida.exe

OBJ=\
	wall_time.o \
	ModVida.o \
	MainVida.o


$(EXEC):	$(OBJ) 
	$(CC) -o $(EXEC) $(CCLOAD) $(OBJ) 

clean:
	rm -f *.o $(EXEC)


.SUFFIXES	:	.o .cu

.cu.o:
	$(CC) $(COMP) $(CCFLAGS) $*.cu 

