# Circuit Diagram & Wiring Guide

## ExamWatch — Arduino Uno Wiring

---

## Component Connections

### KY-038 Sound Sensor → Arduino

| KY-038 Pin | Arduino Pin | Note |
|------------|-------------|------|
| VCC | 5V | Power |
| GND | GND | Ground |
| AO (Analog Out) | A0 | Main signal input |
| DO (Digital Out) | Not used | Analog mode used instead |

> Use the **AO** (analog output) pin for fine-grained variance-based whisper detection. The DO pin only gives binary threshold output, which is insufficient for this system.

---

### HC-SR501 PIR Motion Sensor → Arduino

| PIR Pin | Arduino Pin | Note |
|---------|-------------|------|
| VCC | 5V | Power |
| GND | GND | Ground |
| OUT | D2 | Motion signal |

> HC-SR501 has two onboard potentiometers: one for sensitivity (detection range), one for time delay. Set time delay to minimum (~3s) for more responsive detection.

---

### 16×2 I2C LCD Display → Arduino

| LCD Pin | Arduino Pin | Note |
|---------|-------------|------|
| VCC | 5V | Power |
| GND | GND | Ground |
| SDA | A4 | I2C data |
| SCL | A5 | I2C clock |

> Default I2C address is `0x27`. If the LCD doesn't display anything, try `0x3F`. Use an I2C scanner sketch to confirm.

---

### Active Buzzer → Arduino

| Buzzer Pin | Arduino Pin | Note |
|------------|-------------|------|
| + (positive) | D8 | Signal |
| − (negative) | GND | Ground |

> Active buzzer has built-in oscillator — only needs HIGH/LOW signal. Do not use a passive buzzer without modifying the firmware.

---

### LED Indicator → Arduino

| LED Leg | Arduino Pin | Note |
|---------|-------------|------|
| Anode (+, longer) | D9 via 220Ω resistor | Signal |
| Cathode (−, shorter) | GND | Ground |

> Always use a current-limiting resistor (220Ω recommended for 5V Arduino). Without it you risk burning the LED or the Arduino output pin.

---

### Push Button → Arduino

| Button Terminal | Arduino Pin | Note |
|----------------|-------------|------|
| One side | D4 | INPUT_PULLUP mode |
| Other side | GND | Ground |

> Configured as `INPUT_PULLUP` — no external resistor needed. Button press pulls D4 LOW.

---

## ASCII Wiring Diagram

```
                        Arduino Uno
                    ┌───────────────┐
    PIR OUT ────────┤ D2            │
                    │               │
    Buzzer+ ────────┤ D8            │
                    │               │
    LED (220Ω) ─────┤ D9            │
                    │               │
    Button ─────────┤ D4            │
                    │               │
    Sound AO ───────┤ A0            │
                    │               │
    LCD SDA ────────┤ A4            │
    LCD SCL ────────┤ A5            │
                    │               │
    VCC (5V) ───────┤ 5V            │
    GND ────────────┤ GND           │
                    └───────────────┘
```

---

## Power Supply

- **Recommended:** USB power via PC/laptop or 5V USB adapter
- All components (Arduino, sensors, LCD) run on 5V
- Estimated total current draw: ~150–200mA under normal operation

---

## Notes

- Keep sensor wires away from the buzzer to avoid interference
- The KY-038 onboard sensitivity potentiometer should be tuned to mid-range for the adaptive baseline algorithm to work effectively
- PIR sensor has a warm-up time of ~30 seconds after power-on before it gives reliable readings
