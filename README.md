
# Spectral Gate

Spectral gate audio effect plugin created with C++, JUCE and CMake

## Overview

A spectral Gate is an audio effect that analyses incoming signal using a Fast Fourier Transform (FFT) and applies gain reduction to individual frequency bins whose magnitude falls below a user-set threshold.

I built this project for use in music production, namely for reducing noise or simply as a creative effect. It was also a way for me to explore DSP and learn a bit of C++ development. 

https://github.com/user-attachments/assets/cc815198-59ea-4b71-917b-0d23c2385f74

## DSP Pipeline

The plugin processes audio using the following pipeline:

1. Incoming audio is divided into overlapping frames.
2. A Hann window is applied to each frame.
3. An FFT converts the signal from the time domain into the frequency domain.
4. The magnitude of each frequency bin is compared against the threshold.
5. Each bin is attenuated or passed depending on its magnitude.
6. Gain changes are smoothed over time.
7. An inverse FFT converts the processed signal back into the time domain.
8. Windowing and overlap-add reconstruction produce the output signal.

## Further Developments

- Investigate and reduce transient clicking caused by rapid spectral changes, perhaps via spectral smoothing.
- Add user-controlled attack and release sliders.
- Improve GUI.

## Details

- **Language:** C++23
- **Framework:** JUCE
- **Build system:** CMake
- **FFT size:** 1024 samples
- **Hop size:** 512 samples
- **Overlap:** 50%
- **Window:** Hann
- **Frequency bins:** 513
