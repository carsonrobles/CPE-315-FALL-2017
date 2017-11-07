HDR = mips_asm_header.h mipssim.h
SRC = main.c mipssim.c
OBJ = $(SRC:.c=.o)
EXE = a.out

$(EXE): $(HDR) $(SRC)
	gcc -c $(SRC)
	gcc $(OBJ) -o $(EXE)

clean:
	rm -f $(OBJ) $(EXE)
