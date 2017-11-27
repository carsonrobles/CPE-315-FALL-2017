.text

main:
        # set reg 1 to 0xaa
        ori     $1, $zero, 0xaa

        # and reg 1 to itself assign to reg 2
        and     $2, $1, $1

        # clear all bits in reg 1 but the least significant 4
        andi    $1, $1, 0xf

        # try to assign to $zero
        ori     $zero, $zero, 0xff

.end
