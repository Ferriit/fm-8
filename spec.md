# FM-8 ISA spec

## Opcodes
The list ordering defines the machine code for the opcode. (ADD = 0, SUB = 1, etc. etc.)
```
ADD [reg1, reg2]        -   Performs reg1 = reg1 + reg2
SUB [reg1, reg2]        -   Performs reg1 = reg1 - reg2
MUL [reg1, reg2]        -   Performs reg1 = reg1 * reg2
INC [reg]               -   Performs reg++
DEC [reg]               -   Performs reg--
AND [reg1, reg2]        -   Performs reg1 = reg1 & reg2
NOT [reg]               -   Performs reg = ~reg
OR [reg1, reg2]         -   Performs reg1 = reg1 | reg2
EOR (XOR) [reg1, reg2]  -   Performs reg1 = reg1 ^ reg2
SHR [reg]               -   Performs reg = reg >> 1
SHL [reg]               -   Performs reg = reg << 1

TST [reg1, reg2]        -   Performs reg1 & reg2 and updates flags
CMP [reg1, reg2]        -   Performs reg1 - reg2 and updates flags

JMP [label]             -   Jumps to a relative address
BA [label]              -   Branches to an absolute address
(All following branches are relative. The string is what flags have to be on to branch. "!" means that flag has to be negative)
BEQ [label]             -   Z
BNE [label]             -   !Z
BGT [label]             -   !Z && (N == V)
BLT [label]             -   N == V
BGE [label]             -   N != V
BLE [label]             -   Z || (N != V)
BHI [label]             -   C!Z
BLO [label]             -   !C
BMI [label]             -   N
BPL [label]             -   !N
BVS [label]             -   V
BVC [label]             -   !V
BHS [label]             -   C
BLS [label]             -   !C | Z
CALL [label]            -   Absolute. Stores current PC on stack and jumps.
RET                     -   Returns to the value stored on the stack.

INT                     -   Triggers an interrupt.
RTI                     -   Returns execution after interrupt handling.

MOV [reg1, reg2]        -   Copies the value of reg2 into reg1.
LDI [reg, value]        -   Stores value into reg.
SETV [var, reg]         -   Sets var to reg.
LDV [reg, var]          -   Sets reg to var.
SETA [reg1, reg2]       -   Sets the address at reg1 to reg2.
LDA [reg1, reg2]        -   Sets reg1 to the value stored at the address in reg2.

PUSH [reg]              -   Pushes contents of reg to the stack.
POP [reg]               -   Pops last element from the stack to reg.

NOP                     -   Nothing.
HLT                     -   Halt execution.
```

## Watchdog:
- At startup and reset, the CPU populates 0xFFF8 with 255.
- The CPU decrements the value of 0xFFF8 once every 8th clock cycle.
- If the value at 0xFFF8 reaches 0, the CPU will trigger NMI 0x02.

## Interrupts:
- Interrupts can be triggered by a lot of things, including:
    * IO updates (keypresses or other peripherals).
    * Reading from disk.
    * OS Kernel.
    * BIOS.
    * Normal programs.

- To handle interrupts, addresses 0xFFFA - 0xFFFB, and 0xFFFE - 0xFFFF have to be populated.
    * 0xFFFA - 0xFFFB   -   NMI Vector.
    * 0xFFFE - 0xFFFF   -   IRQ Vector.

- The NMI Vector is a the address where the code for urgent interrupts (Out of memory, Low Energy, etc. etc.) lives.
- The IRQ Vector is where the code for normal interrupts lives (Keydown, Keyup, etc. etc)
- The RST Vector is populated by the CPU and is the address at which the CPU will continue execution when `RTI` is called.

- The jobs of both the IRQ and NMI Vectors are to read address  0xFFF9 as it contains the interrupt code and act accordingly.

### List of interrupts:
#### NMI:
```
0x00 - System Reset     (Startup or reset)                  (SR)
0x01 - Power Failure    (Voltage drops significantly)       (PF)
0x02 - Watchdog Timer   (Set 0xFFF8 to 255)                 (WT)
0x03 - Memory Parity Error (Bitflip)                        (MPE)
0x04 - Kernel Interrupt (Interrupt triggered by Kernel)     (KINT)
0x05 - Power Off (Power down button was pressed)            (DOWN)
```

#### IRQ:
```
0x10 - System Tick (Fires once every second)                (STICK)
0x11 - Input (Keypress, mouse movement, etc. etc)           (INPUT)
0x12 - UART / Serial RX (Data arrived from another CPU)     (UART / SRX)
0x13 - V-Blank (Screen done drawing)                        (VBLNK)
0x14 - Storage Ready (Disk has loaded block into RAM)       (SRDY)
0x15 - Sound Buffer Low (Sound chip out of notes)           (SBL)
0x16 - Program Interrupt (Interrupt triggered by program)   (PINT)
0x17 - Kernel IRQ (Less urgent Kernel Interrupt)            (KIRQ)
```

#### Memory Layout:
- Memory is laid out in 256 memory pages of 256 bytes each.
- Page #0 is reserved for variables defined in .data.
- Page #1 is the first page at which execution begins.
##### Memory portions:
- `0xFFFA + 0xFFFB` - NMI Vector. Where the CPU goes after non-maskable interrupts.
- `0xFFFC + 0xFFFD` - RST Vector. Where the CPU starts after reset and bootup.
- `0xFFFE + 0xFFFF` - IRQ Vector. Where the CPU goes after normal interrupt requests.

### Assembler Syntax:
#### Sections:
##### section .setup
- Setup is used to define fields for `nmi` and `irq`.
###### Fields:
- `nmi` - Sets the label at which to set the NMI Vector.
- `irq` - Sets the label at which to set the IRQ Vector.
- `rst` - Sets the label at which to start.

##### section .data
- Data is used to define variable names and sizes.
###### Fields:
- `[name] [size]`. Where size is in bytes. The max size of all variables in total is 256 bytes. This allows the user to access the value stored at `[name]` using `setv` and `ldv`.

##### section .text
- Where the code implementation lives.
- The labels used in `.setup` must exist.

### Examples:
Examples can be found in `tests/`. The smallest possible program can be found in `tests/minimalexample.s`