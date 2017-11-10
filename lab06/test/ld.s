.data

test: .word 0x0000ffff

.text

    lw $1, test

.end
