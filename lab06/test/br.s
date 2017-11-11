.text

        ori     $2, $zero, 0x2

label:  addi    $2, $2, -0x1
        bne     $2, $zero, label

.end
