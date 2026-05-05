section .setup
nmi _nmi_handle     ; Set up labels for nmi and irq vectors
irq _irq_handle
rst _start

section .data

section .text
; Entry point just halts
_start:
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
    beq nmi_shutdown

    ; Check for Watchdog Timer
    ldi rb, 0x02
    cmp ra, rb
    beq nmi_watchdog

    nmi_done:
    pop f
    pop p
    pop rd
    pop rc
    pop rb
    pop ra
    rti

    ; Reset Watchdog Timer since there is no infinite loop
    nmi_watchdog:
        ldi rb, 255
        ldi rc, 0xF8
        stra rc, rb

        jmp nmi_done

    ; Halt the CPU
    nmi_shutdown:
        hlt

_irq_handle:
    rti
