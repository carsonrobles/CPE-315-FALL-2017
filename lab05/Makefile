HDR = mips_asm_header.h decode.h
SRC = load_testcase.c decode.c
OBJ = $(SRC:.c=.o)
EXE = a.out

$(EXE): $(HDR) $(SRC)
	gcc -c $(SRC)
	gcc $(OBJ) -o $(EXE)

clean:
	rm -f $(OBJ) $(EXE)
