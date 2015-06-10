#!/usr/bin/env python3

# this example is a changed copy from
# http://stackoverflow.com/questions/4152457/parameters-for-low-pass-fir-filter-using-scipy

from scipy import signal
import numpy as np
import matplotlib.pyplot as plt
import time

import cilt

fs = 1000.  # sampling rate
fc = 0.05 / (0.5 * fs)  # cutoff frequency at 0.05 Hz
N = 1001  # number of filter taps
a = 1  # filter denominator
b = signal.firwin(N, cutoff=fc, window='hamming')  # filter numerator

m = fs * 60  # number of samples (60 seconds)
n = np.arange(m)  # time index
x1 = np.cos(2 * np.pi * n * 0.025 / fs)  # signal at 0.025 Hz
x = x1 + 2 * np.random.rand(m)  # signal + noise

filt = cilt.FilterTransversal(b)
y = np.zeros(len(x))

start_cilt_tick = time.time()
for i in range(len(x)):
    y[i] = filt.tick(x[i])
end_cilt_tick = time.time()

start_cilt_lfilter = time.time()
z = filt.lfilter(x)
end_cilt_lfilter = time.time()

start_scipy_lfilter = time.time()
y_scipy = signal.lfilter(b, a, x)
end_scipy_lfilter = time.time()

print('cilt.tick:', end_cilt_tick-start_cilt_tick)
print('cilt.lfilter:', end_cilt_lfilter-start_cilt_lfilter)
print('scipy.lfilter:', end_scipy_lfilter-start_scipy_lfilter)

plt.subplot(121)
plt.plot(n / fs, x, label='Noisy Data')
plt.plot(n / fs, y, 'r', label='Filtered Data')
plt.title('Filter')
plt.xlabel('t [s]')
plt.legend()
plt.grid()

plt.subplot(122)
plt.ticklabel_format(style='sci', axis='y', scilimits=(0, 0))  # use scientific format for y-axis
plt.plot(n / fs, y - y_scipy, 'g')
plt.title('Differnece cilt and scipy.signal.lfilter')
plt.xlabel('t [s]')
plt.grid()

plt.show()
