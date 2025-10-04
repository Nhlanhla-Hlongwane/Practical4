import wave
import numpy as np
import matplotlib.pyplot as plt

# Parameters
LUT_SIZE = 512
DAC_RESOLUTION = 4095

def wav_to_lut(filename, lut_size=LUT_SIZE):
    # Open wav file
    with wave.open(filename, 'rb') as wav:
        n_channels = wav.getnchannels()
        sampwidth = wav.getsampwidth()
        n_frames = wav.getnframes()
        framerate = wav.getframerate()
        
        raw_data = wav.readframes(n_frames)

    # Convert raw bytes to numpy array
    if sampwidth == 1:
        dtype = np.uint8   # 8-bit PCM
    elif sampwidth == 2:
        dtype = np.int16   # 16-bit PCM
    elif sampwidth == 4:
        dtype = np.int32   # 32-bit PCM
    else:
        raise ValueError("Unsupported sample width: {}".format(sampwidth))

    data = np.frombuffer(raw_data, dtype=dtype)

    # If stereo, keep only one channel
    if n_channels > 1:
        data = data[::n_channels]

    # Normalize to 0–1
    data = data.astype(np.float32)
    data = (data - np.min(data)) / (np.max(data) - np.min(data))

    # Downsample / pick LUT_SIZE points
    indices = np.linspace(0, len(data)-1, lut_size, dtype=int)
    lut = data[indices]

    # Scale to 0–4095 (12-bit DAC)
    lut = (lut * DAC_RESOLUTION).astype(int)

    return lut

# Example usage with your three wav files
files = ["drum.wav", "guitar.wav", "piano.wav"]
luts = {}

for f in files:
    lut = wav_to_lut(f)
    luts[f] = lut

    # Plot waveform
    plt.figure()
    plt.plot(lut, marker='o')
    plt.title(f"LUT from {f}")
    plt.xlabel("Sample index")
    plt.ylabel("DAC value (0–4095)")
    plt.grid(True)

plt.show()

# Print arrays for copy-paste into main.c
for f, lut in luts.items():
    print(f"\n// Lookup Table for {f}")
    print("{", ", ".join(map(str, lut)), "};")
