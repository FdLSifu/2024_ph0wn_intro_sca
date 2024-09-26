import matplotlib.pyplot as plt
import numpy as np

traces = np.load("traces.npy")

plt.plot(traces[0])
#plt.plot(traces[1])

plt.show()
