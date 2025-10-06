import numpy as np

N = 256
max_val = 4095

# Sine LUT
sine = ( (np.sin(2*np.pi*np.arange(N)/N) + 1) / 2 * max_val ).astype(int)

# Sawtooth LUT
saw = ( np.linspace(0, max_val, N, endpoint=False) ).astype(int)

# Triangle LUT
triangle = np.concatenate([
    np.linspace(0, max_val, N//2, endpoint=False),
    np.linspace(max_val, 0, N//2, endpoint=False)
]).astype(int)

print("Sine LUT:", sine.tolist())
print("Sawtooth LUT:", saw.tolist())
print("Triangle LUT:", triangle.tolist())
