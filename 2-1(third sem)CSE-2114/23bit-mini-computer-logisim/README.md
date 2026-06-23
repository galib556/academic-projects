# 23-Bit Mini Computer — Logisim Implementation

> **Course:** Computer Architecture Laboratory (CSE-2114)  
> **Institution:** Khulna University of Engineering & Technology (KUET)  
> **Department:** Computer Science & Engineering  
> **Student ID:** 2307113  

---

## Overview

This project implements a fully functional **23-bit mini computer** from the ground up inside **Logisim 2.7.1** — every component built from primitive logic gates, no black-boxes. The design covers two tightly integrated systems:

1. **Booth's Multiplier** — A sequential hardware unit that multiplies two 23-bit signed (2's complement) integers using Booth's algorithm, controlled by an 8-state microprogram ROM.
2. **Simple 8-Instruction CPU** — An accumulator-based processor with a register file, microprogram control unit, 23-bit ALU, and integrated Booth multiplier unit.

The entire design hierarchy — from single full-adder cells up to the complete CPU — was built and verified in Logisim simulation.

---

## Project Structure

```
.
├── circuits/
│   ├── 2307113.xml          # Standalone Booth's Multiplier + 23-bit ALU
│   └── CPU_demo.xml         # Full CPU integrating multiplier, ALU, control unit
├── docs/
│   └── Project_Report.docx  # Full academic project report
├── simulation-guide/
│   └── HOW_TO_RUN.md        # Step-by-step simulation instructions
└── README.md
```

---

## Architecture at a Glance

### Instruction Set (8 Instructions)

| Opcode (3-bit) | Mnemonic | Operation |
|:-:|:-:|---|
| 000 | AND | `Ac ← Ac AND mem[addr]` |
| 001 | ADD | `Ac ← Ac + mem[addr]` |
| 010 | STO | `mem[addr] ← Ac` |
| 011 | OR  | `Ac ← Ac OR mem[addr]` |
| 100 | SUB | `Ac ← Ac − mem[addr]` |
| 101 | BUN | `PC ← addr` (Branch Unconditional) |
| 110 | LDA | `Ac ← mem[addr]` (Load Accumulator) |
| 111 | HLT | Halt |

### Instruction Word Format (23-bit)

```
 [22 .......... 20] [19 ...... 0]
  Opcode (3 bits)   Address (19 bits... 1 unused as per IR = 4-bit split: 3 opcode + 1 unused flag)
```

### CPU Register File

| Register | Width | Purpose |
|---|:-:|---|
| Ac (Accumulator) | 23-bit | Primary data register; ALU source & destination |
| MAR | 19-bit | Memory Address Register |
| MBR | 23-bit | Memory Buffer Register |
| IR  | 4-bit  | Instruction Register (opcode field) |
| PC  | 19-bit | Program Counter (auto-increment) |

### 23-Bit ALU Operations

| Control1 | Control0 | Operation |
|:-:|:-:|:-:|
| 0 | 0 | AND |
| 0 | 1 | OR  |
| 1 | 0 | ADD |
| 1 | 1 | SUB |

**Status Flags:** Carry · Overflow · Zero · Negative

### Booth's Multiplier — Key Specs

| Parameter | Value |
|---|---|
| Operand width | 23-bit signed (2's complement) |
| Product width | 46-bit (High bits + Low bits) |
| Cycles per multiply | Configurable via counter (max = 0x17 = 23) |
| Controller | 3-bit address, 12-bit control word ROM |
| Registers | A (accumulator), Q (multiplier), M (multiplicand), Q−1 (1-bit) |

---

## Circuit Hierarchy

```
CPU_demo.xml
└── main                        (testbench: RAM + CPU + Clock)
    └── CPU                     (datapath: registers, MUXes, tunnels)
        ├── ControlUnit          (microprogram ROM 6-addr × 7-bit)
        └── booths               (Booth multiplier wrapper)

2307113.xml
└── Booth's multiplier          (top-level sequential controller)
    ├── Controller               (microprogram ROM 3-addr × 12-bit)
    ├── 23bit ALU
    │   ├── Add_sub              (adder/subtractor + overflow detection)
    │   │   ├── 23_bit_full_adder  (23× Full_adder cascade)
    │   │   └── 23_bit_xor       (XOR-based 2's complement negation)
    │   ├── 23_bit_and
    │   ├── 23_bit_OR
    │   ├── 23_bit_reduce_or     (zero-detect)
    │   └── sign bit out         (MSB extraction)
    └── Shift Registers: A, Q, M, Q−1
```

---

## How to Run

### Requirements
- [Logisim 2.7.1](http://www.cburch.com/logisim/) (free, cross-platform, Java-based)

### Booth's Multiplier
1. Open `circuits/2307113.xml` in Logisim
2. Set **M input** and **Q input** pins to your 23-bit signed operands
3. Toggle **Reset** high then low
4. Tick the clock — **BUSY** stays HIGH during computation
5. After completion, read **High bits** (upper 23) and **Low bits** (lower 23)

### CPU Simulation
1. Open `circuits/CPU_demo.xml` in Logisim
2. Right-click the **RAM** block → *Edit Contents* → load your program
3. Press the **Reset** button
4. Enable the **Clock** (Simulate → Auto-Tick)
5. Observe register values updating each cycle

See [`simulation-guide/HOW_TO_RUN.md`](simulation-guide/HOW_TO_RUN.md) for sample programs and expected outputs.

---

## Sample Test — Booth's Multiplier

| M | Q | Expected Product | Type |
|---|---|---|---|
| +6 (0x000006) | +5 (0x000005) | +30 | Positive × Positive |
| −6 (0xFFFFFA) | +5 (0x000005) | −30 | Negative × Positive |
| +7 (0x000007) | −3 (0xFFFFFD) | −21 | Positive × Negative |
| −4 (0xFFFFFC) | −4 (0xFFFFFC) | +16 | Negative × Negative |

---

## Key Design Decisions

- **XOR-based subtraction** — The `23_bit_xor` circuit conditionally inverts operand B when `Sub=1`, with carry-in set to 1, implementing 2's complement negation without a dedicated negator unit.
- **Hierarchical gate-level design** — Every bit-wide operation (AND, OR, XOR, full-adder) is implemented as an individual gate instance, making the design transparent and educational.
- **Tunnel-based routing** — Logisim tunnels are used extensively in the CPU datapath to manage routing complexity without crossing wires.
- **Microprogram control** — Both the CPU ControlUnit and Booth's Controller use ROM-based microprogramming, separating control logic from datapath.
- **BUSY-flag handshake** — The CPU polls the multiplier's BUSY signal rather than stalling the clock, allowing clean multi-cycle integration.

---

## Skills Demonstrated

`Digital Logic Design` · `Computer Architecture` · `2's Complement Arithmetic` · `Sequential Circuit Design` · `Microprogramming` · `ALU Design` · `Booth's Algorithm` · `Logisim` · `Hardware Description` · `Accumulator-based CPU`

---

## References

- Booth, A. D. (1951). *A signed binary multiplication technique*. Quarterly Journal of Mechanics and Applied Mathematics.
- Mano, M. M. & Ciletti, M. D. — *Digital Design*, 5th Ed., Pearson.
- Patterson, D. A. & Hennessy, J. L. — *Computer Organization and Design*, 5th Ed., Morgan Kaufmann.
- Logisim Documentation — http://www.cburch.com/logisim/

---

*Built with Logisim 2.7.1 | KUET CSE | 2307113*
