from pwn import *
import matplotlib.pyplot as plt
p = remote('localhost',6000)
p.sendline(b'a'*32)
buf = p.readall()
data = [int(b) for b in buf]

plt.plot(data)
plt.show()

