# cmpe220-program-layout-and-execution
Demonstrates how a recursive C-style factorial function is executed using our Software CPU design from the previous CMPE 220 project. The demo shows the original C function and driver program, the equivalent Software CPU assembly source, the commands used to build and run the program, and how the executable is laid out in memory

---
# Program Layout & Execution Report

## Recursive Function Call Demonstration  
**CMPE 220 - Spring 2026**  
San Jose State University  

**Group 13**  
- Muhammad Tokhi  
- Miles Nguyen  
- Myles Easter  

---

## GitHub Repository
**This Repository: Program Layout & Execution**  
https://github.com/muhammadtokhi-sjsu/cmpe220-program-layout-and-execution  

**Associated Repository: CPU Design**  
https://github.com/0x-000000/cmpe-220.git  

The repository contains the Software CPU project and the added recursive function demonstration for this assignment. The project includes the assembler, emulator, CPU, bus, BIOS, terminal device, ISA files, example programs, and build configuration.

### Directory Structure

| Directory/File | Purpose |
|---------------|--------|
| `src/` | C++ source files for assembler, encoder, CPU, bus, BIOS, terminal, emulator, CLI |
| `examples/factorial.c` | C recursive factorial program |
| `examples/factorial.fasm` | Assembly version for Software CPU |
| `docs/` | Report, diagrams, notes |
| `CMakeLists.txt` | Build configuration |
| `README.md` | Overview, instructions, video link |
---

## Video Demonstration
### Factorial
https://youtu.be/7rK0Fj6bZWw

---

## Download, Compile, and Run

### Requirements
- CMake 3.29+
- C++20 compiler (Clang 14+, GCC 11+, MSVC 19.30+)
- Git  
- *(Windows users: WSL recommended)*

### Build

```bash
git clone https://github.com/0x-000000/cmpe-220.git
cd cmpe-220
cmake -B build -S .
cmake --build build --config Release -j
````

### Run (Software CPU)

```bash
./build/vm -f examples/factorial.fasm
```

**Expected output:**

```
120
```

### Optional Native C Run

```bash
gcc examples/factorial.c -o factorial
./factorial
```

Output:

```
factorial(5) = 120
```

---

## Project Overview

### Goal

Demonstrate how a recursive C-style function is laid out in memory and executed using a custom Software CPU.

### Factorial Definition

```
factorial(n) = n * factorial(n - 1)
factorial(0) = 1
factorial(1) = 1
```

Input: `n = 5`
Output: `120`

### Key Concepts

1. **Program Layout** – Instructions loaded at address `0x0000`
2. **Function Calls** – `bl` and `ret` manage control flow
3. **Recursion** – Stack frames store return addresses and arguments

---

## C Driver Program

```c
#include <stdint.h>
#include <stdio.h>

uint16_t factorial(uint16_t n) {
    if (n <= 1) {
        return 1;
    }

    return n * factorial(n - 1);
}

int main(void) {
    uint16_t n = 5;
    uint16_t result = factorial(n);

    printf("factorial(%u) = %u\n", n, result);
    return 0;
}
```

---

## Software CPU Assembly Version

```asm
_start:
    movl r0, #5
    movh r0, #0
    bl fact
    bl print_num

    movl r0, #0x0A
    movl r1, #0x02
    movh r1, #0x80
    str r0, r1
    halt

fact:
    push rbl
    movl r1, #1
    movh r1, #0
    cmp r0, r1
    ble fact_base

    push r0
    sub r0, r0, r1
    bl fact

    pop r1
    mul r0, r1, r0
    ret

fact_base:
    movl r0, #1
    movh r0, #0
    ret
```

---

## Executable Memory Layout

### Address Space

| Region         | Address Range   | Purpose              |
| -------------- | --------------- | -------------------- |
| User RAM       | `0x0000–0x7FFE` | Program, data, stack |
| Terminal MMIO  | `0x8000–0x800F` | Terminal registers   |
| BIOS ROM / IVT | `0xFF00–0xFFFF` | Boot + interrupts    |

### Program Layout

| Address Range   | Section     | Description       |
| --------------- | ----------- | ----------------- |
| `0x0000–0x0010` | `_start`    | Driver code       |
| `0x0012–0x002C` | `fact`      | Recursive logic   |
| `0x002E–0x0054` | `print_num` | Output conversion |
| `0x7FFE ↓`      | Stack       | Grows downward    |
| `0x8002`        | Terminal TX | Output register   |

---

## Function Calls

* `bl fact` → Jump + save return address in `rbl`
* `push rbl` → Save return address on stack
* `ret` → Restore return address into `rip`

### Call Flow

```
fact(5)
 → fact(4)
   → fact(3)
     → fact(2)
       → fact(1) → returns 1
```

### Stack Unwinding

```
fact(2) = 2 * 1   = 2
fact(3) = 3 * 2   = 6
fact(4) = 4 * 6   = 24
fact(5) = 5 * 24  = 120
```

---

## Recursion and Stack Usage

| Address  | Value              | Purpose      |
| -------- | ------------------ | ------------ |
| `0x7FFC` | return to `_start` | from fact(5) |
| `0x7FFA` | 5                  | argument     |
| `0x7FF8` | return to fact(5)  |              |
| `0x7FF6` | 4                  |              |
| `0x7FF4` | return to fact(4)  |              |
| `0x7FF2` | 3                  |              |
| `0x7FF0` | return to fact(3)  |              |
| `0x7FEE` | 2                  |              |
| `0x7FEC` | return to fact(2)  |              |

---

## CPU Execution Flow

| Stage     | Description                  |
| --------- | ---------------------------- |
| Fetch     | Read instruction from memory |
| Decode    | Determine instruction type   |
| Execute   | Perform operation            |
| Writeback | Store result                 |

### Key Instructions

* `cmp r0, r1`
* `ble fact_base`
* `sub r0, r0, r1`
* `mul r0, r1, r0`

---

## Memory-Mapped Output

```asm
movl r1, #0x02
movh r1, #0x80
str  r0, r1
```

* Address `0x8002` → Terminal TX
* Writing triggers console output in emulator

---

## Video Demonstration Checklist

* C factorial program
* Assembly version
* Build & run
* Memory layout
* Function calls (`bl`, `ret`)
* Recursion + stack behavior


## Conclusion

This project demonstrates how a recursive function can be executed on a custom CPU. It showcases:

* Function calls
* Stack frames
* Recursion
* Memory layout
* CPU execution cycle
* Memory-mapped I/O

Final result:

```
5! = 120
```
