; test .align pseudo-op

* = $1006                   ;-06 10
                            ;now at $1006
    .align 8                ;-00 00
                            ;now at $1008
    .align 4                ;aleady aligned
    .byte $55               ;-55
                            ;now at $1009
    .align 2                ;-00
                            ;now at $100a
    .byte $aa               ;-aa
                            ;now at $100b
    .align 64               ;-00 00 00 00 00
                            ;now at $1010
                            ;-00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
                            ;now at $1020
                            ;-00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
                            ;now at $1030
                            ;-00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
                            ;now at $1040 = 4160
    .align 7                ;-00 00 00 00 00
                            ;now at 4165
    .byte $11               ;-11
                            ;now at 4166
    .align 4-1              ;-00
                            ;now at 4167
    .byte $22               ;-22
