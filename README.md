# Spectral Gate

Spectral gate audio effect plugin created with C++, JUCE and CMake

## Overview

A spectral Gate is an audio effect that analyses incoming signal using a Fast Fourier Transform (FFT) and applies gain reduction to individual frequency bins whose magnitude falls below a user-set threshold.

I built this project for use in music production, namely reducing noise. It was also a way for me to explore DSP and learn a bit of C++ development. 

## Features

- Real-time spectral gating
- FFT-based frequency analysis
- Per-frequency-bin gain reduction
- Temporal gain smoothing
- Hann windowing
- Overlap-add reconstruction
- Adjustable threshold control
- VST3, AU and Standalone formats

## DSP Approach

The plugin processes audio using the following pipeline:

1. Incoming audio is divided into overlapping frames.
2. A Hann window is applied to each frame.
3. An FFT converts the signal into the frequency domain.
4. The magnitude of each frequency bin is compared against the threshold.
5. Each bin is attenuated or passed depending on its magnitude.
6. Gain changes are smoothed over time.
7. An inverse FFT converts the processed signal back into the time domain.
8. Windowing and overlap-add reconstruction produce the output signal.

## Technical Details

- **Language:** C++23
- **Framework:** JUCE
- **Build system:** CMake
- **FFT size:** 1024 samples
- **Hop size:** 512 samples
- **Overlap:** 50%
- **Window:** Hann
- **Frequency bins:** 513

## Plugin Formats

- VST3
- Audio Unit (AU)
- Standalone

## Development

Developed and tested on macOS using Ableton Live 11.

## Build

This project uses JUCE and CMake.

JUCE should be available in the project root as:

    SpectralGate/
    ├── JUCE/
    ├── Source/
    ├── CMakeLists.txt
    └── README.md

After JUCE is available, the project can be configured and built with:

    cmake -B build -S .
    cmake --build build

## Project Structure

    Source/
    ├── PluginProcessor.cpp
    ├── PluginProcessor.h
    ├── PluginEditor.cpp
    └── PluginEditor.h

## Purpose

This project was built as a hands-on exploration of C++, digital signal processing and real-time audio software development.
