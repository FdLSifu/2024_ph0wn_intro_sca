from pwn import *
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.backends.backend_qt5agg
import os

NB_TRACES = 50000
TRACE_LEN = 4096
PT_LEN = 16

traces = np.zeros((NB_TRACES, TRACE_LEN))
plaintexts = np.zeros((NB_TRACES, PT_LEN))

for i in range(NB_TRACES):
    p = remote('localhost', 5000)
    plaintext = os.urandom(PT_LEN)
    p.send(plaintext)
    
    trace = np.frombuffer(p.read(), dtype=np.uint8)

    traces[i] = trace

    plaintext = np.frombuffer(plaintext, dtype=np.uint8)
    plaintexts[i] = plaintext
    
    p.close()

print("Saving ...")
np.save('traces.npy', traces)
np.save('plaintexts.npy', plaintexts)
