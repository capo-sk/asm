; test .align pseudo-op

* = $1006                   ;-06 10
                            ;now at $1006
    .align 8                ;-00 00
                            ;now at $1008
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
                            ;now at $1040
