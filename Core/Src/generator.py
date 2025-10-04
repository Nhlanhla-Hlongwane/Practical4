import numpy as np
import matplotlib.pyplot as plt

N = 128
max_val = 4095

# Sine LUT
sine = ((np.sin(2 * np.pi * np.arange(N) / N) + 1) / 2 * max_val).astype(int)

# Sawtooth LUT
saw = np.linspace(0, max_val, N, endpoint=False).astype(int)

# Triangle LUT
triangle = np.concatenate([
    np.linspace(0, max_val, N // 2, endpoint=False),
    np.linspace(max_val, 0, N // 2, endpoint=False)
]).astype(int)

# ---- Plotting ----
plt.figure(figsize=(10, 6))

plt.plot(sine, label="Sine Wave", marker='o')
plt.plot(saw, label="Sawtooth Wave", marker='s')
plt.plot(triangle, label="Triangle Wave", marker='^')

plt.title("Waveform Lookup Tables (LUTs)")
plt.xlabel("Sample Index")
plt.ylabel("Amplitude (0 - 4095)")
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()
plt.show()
