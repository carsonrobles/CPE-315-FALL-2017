CC   = gcc
CFLG = -g

HDR = cachesim.h
SRC = matmul.c
OBJ = $(SRC:.c=.o)
EXE = a.out

.PHONY: all
all : $(EXE)

$(EXE) : $(OBJ)
	$(CC) $(CFLG) $^ -o $@

matmul.o : matmul.c cachesim.h
	$(CC) $(CFLG) -c $< -o $@

.PHONY : clean
clean :
	rm -f $(EXE) $(OBJ)
