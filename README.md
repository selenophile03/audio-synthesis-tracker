# 🎹 Minimalist C Audio Synthesis Tracker

A lightweight, zero-dependency audio synthesis engine written entirely in C99. This project parses custom musical note sequences, mathematically generates raw waveforms (Sine, Square, Sawtooth, Triangle), and exports them directly into valid PCM `.wav` audio files without relying on external multimedia libraries.

---

## 🚀 Features

* **Zero External Dependencies** — Built using only standard, portable C libraries (`stdio.h`, `stdlib.h`, `math.h`).
* **Multi-Waveform Oscillator** — Supports rendering of classic chip-tune signals (Sine, Square, Sawtooth, Triangle).
* **Explicit RIFF/WAV Exporter** — Manually packs binary structural audio headers from scratch.
* **Anti-Click Phase Tracking** — Resets oscillators cleanly on new notes to eliminate harsh digital distortion popping.

---

## 🛠️ How to Compile & Run

### Prerequisites
You only need a standard C compiler (like `gcc` or `clang`) and the `make` build utility installed on your machine.

### Step-by-Step Execution
1. Clone this repository to your local computer.
2. Build the engine using the automated Makefile configuration:
   ```bash
   make
   ```
3. Run the tracking engine binary to synthesize the musical score:
   ```bash
   ./tracker
   ```
4. Look in your folder! Play the newly created `synth_output.wav` file using any standard audio player.

---

## 📐 How It Works

The program calculates audio values sequentially using a multi-step pipeline:

1. **Timeline Calculations** — The track layout translates target Beats Per Minute (BPM) into strict audio sample blocks. For example, at 120 BPM with 4 rows per beat, each row is exactly 0.125 seconds of sound.
2. **Frequency Mapping** — Note integer values are converted to standard Hertz frequencies using the mathematical equal-temperament scale pitch formula:
   $$Frequency = 440 \times 2^{\frac{\text{Note Index} - 57}{12}}$$
3. **Digital Signal Packaging** — The raw calculated floating-point waves are clamped safely between `-1.0` and `1.0`, mapped to 16-bit signed PCM integers, and written directly behind an explicit RIFF binary file structural header block.

---

## 📂 Project Architecture

* `tracker.h` — Contains configuration definitions, sound wave enum flags, and the packed byte-aligned `.wav` struct schema.
* `main.c` — Houses the oscillator math formulas, file tracking loop blocks, and sample track arrangements.
* `Makefile` — Scripted tool automating compilation flags and directory cleaning commands.

---

## 📝 License
This project is open-source and available under the MIT License. Feel free to fork it, add features, and build your own audio tools!
