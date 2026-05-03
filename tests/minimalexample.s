section .setup
nmi _nmi_handle     ; Set up labels for nmi and irq vectors
irq _irq_handle
start _start

section .data
vara 32             ; Variable of length 32 bytes
varb 1              ; Single letter or number

section .text

; Entry point just halts
_start:
    ldi ra, 1
    setv varb, ra

    hlt

_nmi_handle:
    push ra
    push rb
    push rc
    push rd
    push p
    push f

    ldi p, 0xFF     ; Last page
    ldi rc, 0xF9
    lda ra, rc

    ; Check for Shutdown
    ldi rb, 0x05
    cmp ra, rb
    beq .shutdown

    ; Check for Watchdog Timer
    ldi rb, 0x02
    cmp ra, rb
    beq .watchdog

    .done:
    pop f
    pop p
    pop rd
    pop rc
    pop rb
    pop ra
    rti

    ; Reset Watchdog Timer since there is no infinite loop
    .watchdog:
        ldi rb, 255
        ldi rc, 0xF8
        stra rc, rb

        jmp .done

    ; Halt the CPU
    .shutdown:
        hlt

_irq_handle:
    rti
