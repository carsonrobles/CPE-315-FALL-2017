CC  = gcc
HDR = mips_asm_header.h mipssim.h
SRC = main.c mipssim.c
OBJ = $(SRC:.c=.o)
EXE = a.out

$(EXE): $(HDR) $(SRC)
	$(CC) -c $(SRC)
	$(CC) $(OBJ) -o $(EXE)

.PHONY: clean
clean:
	rm -f $(OBJ) $(EXE)
