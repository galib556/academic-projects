# Simulation Guide — 23-Bit Mini Computer

## Prerequisites

Download and install **Logisim 2.7.1**:  
http://www.cburch.com/logisim/  
Requires Java 8 or later.

---

## Running the Booth's Multiplier (2307113.xml)

### Step 1 — Open the file
File → Open → select `circuits/2307113.xml`

### Step 2 — Select the top-level circuit
In the left panel, double-click **"Booth's multiplier"** to make it the active circuit.

### Step 3 — Set inputs
Click the **Poke tool** (hand icon) and set:
- **M input** — your 23-bit signed multiplicand (hex or binary)
- **Q input** — your 23-bit signed multiplier

### Step 4 — Reset
Click the **Reset** pin HIGH, then back LOW.

### Step 5 — Clock
Go to **Simulate → Tick Once** (or press Ctrl+T) repeatedly.  
Watch the **BUSY** pin — it stays HIGH while computing.  
After it goes LOW, read:
- **High bits** — upper 23 bits of the 46-bit product
- **Low bits** — lower 23 bits of the 46-bit product

### Example
| M input | Q input | High bits | Low bits | Decimal |
|---|---|---|---|---|
| 0x000006 (+6) | 0x000005 (+5) | 0x000000 | 0x00001E | +30 |
| 0x7FFFFA (−6) | 0x000005 (+5) | 0x7FFFFE | 0x7FFFE2 | −30 |

---

## Running the CPU (CPU_demo.xml)

### Step 1 — Open the file
File → Open → select `circuits/CPU_demo.xml`

### Step 2 — Load a program into RAM
Right-click the **RAM** block → **Edit Contents**

Enter instructions in hex. Each 23-bit word encodes:
```
[22:20] = Opcode (3 bits)
[19:0]  = Address (19 bits, 1 bit unused)
```

### Sample Program
```
Address  | Hex Word  | Assembly     | Comment
---------+-----------+--------------+---------------------------
000      | 0C00014   | LDA [0x14]   | Load value from addr 20
001      | 0200015   | ADD [0x15]   | Add value from addr 21
002      | 0400016   | STO [0x16]   | Store result to addr 22
003      | 0E00000   | HLT          | Halt

Data:
014      | 0000019   | Data: 25
015      | 000000B   | Data: 11
```

### Step 3 — Reset and run
1. Click the **Reset button** (in the main circuit)
2. Go to **Simulate → Auto-Tick Enable** and set a tick frequency (e.g. 8 Hz)
3. Watch the **Ac** register accumulate results

---

## Opcode Reference

| Mnemonic | Opcode (3-bit) | Operation |
|---|:-:|---|
| AND | 000 | Ac ← Ac AND mem[addr] |
| ADD | 001 | Ac ← Ac + mem[addr] |
| STO | 010 | mem[addr] ← Ac |
| OR  | 011 | Ac ← Ac OR mem[addr] |
| SUB | 100 | Ac ← Ac − mem[addr] |
| BUN | 101 | PC ← addr (branch) |
| LDA | 110 | Ac ← mem[addr] |
| HLT | 111 | Stop execution |

---

## Troubleshooting

| Issue | Fix |
|---|---|
| Blue wires (undefined signal) | Click Reset before starting clock |
| Red wires (error/conflict) | Check for disconnected pins in sub-circuits |
| BUSY never goes LOW | Ensure Counter max is set to 0x17 in Booth's controller |
| CPU loops forever | Check HLT opcode (111) is at the end of your program |
