<div align="center">

# ExamWatch: A Microcontroller-Based Smart Exam Surveillance System

**An embedded system that assists invigilators by intelligently detecting suspicious activity during examinations — without cameras, without cloud, without compromising privacy.**

[![Platform](https://img.shields.io/badge/Platform-Arduino%20Uno-teal?logo=arduino)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/Language-Embedded%20C%2FC%2B%2B-blue)](https://www.arduino.cc/)
[![Course](https://img.shields.io/badge/Course-CSE--2104-orange)]()
[![Status](https://img.shields.io/badge/Status-Complete-brightgreen)]()
[![License](https://img.shields.io/badge/License-MIT-lightgrey)](../../LICENSE)

</div>

---

## Overview

ExamWatch is a low-cost, privacy-friendly exam monitoring system built on the **Arduino Uno** microcontroller. It uses sound and motion sensors, combined with adaptive signal processing and a suspicion scoring engine, to detect cheating behaviors like whispering and suspicious movement — with minimal false alarms.

Designed as a practical tool for real exam halls, ExamWatch **assists invigilators rather than replacing them**, operating entirely on-device with no camera, no internet, and no data storage.

---

## The Problem It Solves

Modern automated proctoring systems — while powerful — come with serious trade-offs:

| Problem | How ExamWatch Addresses It |
|--------|---------------------------|
| Privacy concerns from video recording | No camera, no audio recording — anonymous sensor data only |
| High cost (servers, cameras, cloud) | Runs entirely on ~$10 Arduino hardware |
| Environmental noise triggering false alarms | Adaptive baseline + burst validation logic |
| Missing subtle audio cues (whispering) | Variance-based whisper detection algorithm |
| Single-sensor unreliability | Sensor fusion: sound + motion combined scoring |

---

## System Architecture

```
┌──────────────────────────────────────────────────────────┐
│                      Arduino Uno                         │
│                                                          │
│  KY-038 Sound ──► Adaptive Baseline ──► Burst Analysis  │
│      Sensor        Variance Calc        Whisper Count    │
│                          │                    │          │
│                          ▼                    ▼          │
│  HC-SR501 PIR  ──► Motion Pattern ──► SENSOR FUSION      │
│      Sensor         Analysis          Score Engine       │
│                                            │             │
│                          ┌─────────────────┤             │
│                          ▼                 ▼             │
│                    LCD Display       Alert System        │
│                   (Score/Status)   (Buzzer + LED)        │
└──────────────────────────────────────────────────────────┘
```

---

## Working Principle

The system operates in a continuous real-time loop:

1. **Sound Sampling** — Collects 40 audio samples from the KY-038 microphone every cycle and computes the mean and standard deviation.

2. **Adaptive Baseline** — Uses an exponential moving average (`α = 0.01`) to track ambient noise level, ensuring the system adapts to different room environments automatically.

3. **Whisper Detection** — Identifies whispers as small but consistent fluctuations just above the ambient baseline (`avg > baseline + 5`, `10 < stdDev < 120`) — the characteristic signature of hushed speech.

4. **Burst Validation** — A whisper is only counted after **3 consecutive audio bursts** within a 2-second window, filtering out single spikes from random noise.

5. **Motion Detection** — The PIR sensor independently detects body movement and incrementally adds to the suspicion score.

6. **Sensor Fusion** — When a whisper and motion event occur together, a **combined score penalty** is applied (higher than either alone), reflecting the increased likelihood of coordinated cheating.

7. **Score Decay** — The suspicion score passively decays every 3 seconds, preventing stale events from accumulating unfairly.

8. **Alert Trigger** — An alert fires when either:
   - The suspicion score exceeds the threshold (`≥ 20`), or
   - **5 consecutive whispers** are detected without pause

9. **Silent Mode** — A push button lets the invigilator toggle buzzer alerts off, keeping only the LED blink for quiet environments.

---

## Suspicion Scoring System

| Event | Score Added |
|-------|-------------|
| Whisper detected | +4 |
| Motion detected | +0.05 |
| Whisper + Motion together | +8 (combined bonus) |
| Score decay (every 3 sec) | −1 |
| Alert threshold | ≥ 20 |

**Confidence Levels displayed on LCD:**

| Score Range | Level |
|-------------|-------|
| 0 – 6 | LOW |
| 7 – 14 | MED |
| 15+ | HIGH |

---

## Hardware

| Component | Role |
|-----------|------|
| Arduino Uno | Central microcontroller |
| KY-038 Sound Sensor | Analog audio input (whisper detection) |
| HC-SR501 PIR Sensor | Passive infrared motion detection |
| 16×2 I2C LCD Display | Real-time status display |
| Active Buzzer | Audible alert |
| LED Indicator | Visual alert (blink pattern) |
| Push Button | Silent mode toggle |
| Breadboard + Jumper Wires | Prototyping connections |

---

## Pin Configuration

| Component | Arduino Pin |
|-----------|-------------|
| PIR Sensor OUT | D2 |
| Sound Sensor AO | A0 |
| Buzzer | D8 |
| LED | D9 |
| Push Button | D4 |
| LCD SDA | A4 |
| LCD SCL | A5 |

---

## Circuit Diagram

> See [`docs/circuit_diagram.md`](docs/circuit_diagram.md) for the full wiring breakdown.

```
Arduino Uno
│
├── D2  ────────────── PIR HC-SR501 (OUT)
├── A0  ────────────── KY-038 Sound Sensor (AO)
├── D8  ────────────── Active Buzzer (+)
├── D9  ──[220Ω]────── LED (+)
├── D4  ──[PULLUP]──── Push Button
├── A4  ────────────── LCD SDA (I2C)
├── A5  ────────────── LCD SCL (I2C)
├── 5V  ────────────── VCC (all sensors)
└── GND ────────────── GND (all components)
```

---

## How to Run

### Requirements

- [Arduino IDE 2.x](https://www.arduino.cc/en/software)
- Library: `LiquidCrystal_I2C` by Frank de Brabander

### Steps

1. Install the `LiquidCrystal_I2C` library via **Tools → Manage Libraries**
2. Wire components per the pin configuration table above
3. Open `ExamWatchUpgdLv2/ExamWatchUpgdLv2.ino` in Arduino IDE
4. Select board: **Arduino Uno** and the correct COM port
5. Click **Upload**
6. Open **Serial Monitor** at `9600 baud` to observe live debug output

### First Boot

The LCD will display `ExamWatch / Initializing` for 2 seconds, then enter live monitoring mode showing suspicion score, confidence level, event count, and buzzer status.

---

## Features at a Glance

- Adaptive whisper detection with variance analysis
- Burst-based false-alarm suppression
- Real-time suspicion scoring with time decay
- Sensor fusion (sound + motion)
- Confidence level classification (LOW / MED / HIGH)
- Event counter for post-exam review
- Silent mode (LED-only alerts)
- 16×2 LCD live status dashboard
- Serial debug output at 15-second intervals

---

## Project Report

The complete academic report is available at [`docs/Complete_Project_Report_on_ExamWatch.pdf`](docs/Complete_Project_Report_on_ExamWatch.pdf).

It covers:
- Full literature review (6 referenced papers)
- System architecture and working principle
- Results and discussion
- Limitations addressed vs. prior work
- Future improvement roadmap

---

## Literature Context

ExamWatch was designed in response to limitations identified across the following published works:

| Reference | Limitation Addressed |
|-----------|---------------------|
| Leong (2025) — AI-Driven Proctoring | Privacy (no recording/storage) |
| Arumugam (2025) — Deep Learning Invigilation | Cost, complexity, camera dependency |
| Lee & Fanguy (2022) — Online Proctoring Ethics | Privacy-friendly local processing |
| Xue et al. (2023) — Intelligent Invigilator | Audio analysis + false alarm reduction |
| Nigam et al. (2021) — AI Proctoring Review | Single-modality limitations → sensor fusion |
| Jia & He (2021) — Online Proctoring System | High infrastructure cost |

---

## Future Improvements

- Wireless alert transmission (ESP8266/ESP32 module)
- Multi-zone classroom deployment (multiple nodes)
- Camera integration for hybrid detection
- Cloud-based event logging and dashboard
- Automatic startup noise calibration (Kalman filtering)
- Machine learning–based behavioral pattern analysis

---

## Project Info

| Field | Detail |
|-------|--------|
| Course | CSE-2104 — Microprocessors and Microcontroller Lab |
| Institution | Khulna University of Engineering & Technology (KUET) |
| Department | Computer Science and Engineering |
| Semester | 3rd Semester (Year 2, Semester 1) |
| Submission Date | May 14, 2026 |

---

## Authors

**Farhan Shariar** — Roll: 2307113, Lab Group: B2
GitHub: [@galib556](https://github.com/galib556)

**Md. Al Intejar Hasnath Prodhan** — Roll: 2307094, Lab Group: B2
GitHub: [@intejarhasnathprodhanalif](https://github.com/intejarhasnathprodhanalif)

---

## Supervisors

**Md. Sakhawat Hossain** — Assistant Professor, Dept. of CSE, KUET

**Md Tajmilur Rahman** — Lecturer, Dept. of CSE, KUET

---

## License

MIT — see [LICENSE](../../LICENSE)

---

<div align="center">
Farhan Shariar · KUET CSE · <a href="mailto:f.s.galib91@gmail.com">f.s.galib91@gmail.com</a>
</div>
