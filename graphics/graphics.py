import matplotlib.pyplot as plt
import numpy as np

partitions = ['2', '4', '8', '16', '32', '64', '128', '256', '512', '1024', '2048', '4096', '8192', '16384', '32768']
valores_par = [22.99979, 23.594355, 23.732769, 24.053491, 24.672473, 24.861465, 26.509583, 28.371828, 32.327432, 38.802901, 52.968377, 80.819062, 136.69979, 247.203535, 466.880835]
valores_seq = [23.546402, 24.655672, 25.105033, 26.511129, 28.067256, 30.396845, 34.925427, 40.566481, 52.493737, 73.333065, 115.214107, 195.151829, 354.349519, 671.936711, 1298.055453]

partitions2 = ['2', '4', '8', '16', '32', '64', '128', '256', '512', '1024', '2048', '4096', '8192', '16384', '32768']
valores_par2 = [176.6, 176.6, 230.4, 370.8, 661.8, 1200, 2500, 2900, 5200, 5200, 5300, 5500, 5500, 5500, 5500]
valores_seq2 = [176.6, 176.6, 176.6, 176.6, 176.6, 202.2, 221.5, 241.1, 261, 280.9, 300.8, 320.7, 340.6, 360.6, 380.5]

"""
matplotlib.pyplot.plot(partitions, valores_par, valores_seq)
matplotlib.pyplot.ylim(0, 1400)

matplotlib.pyplot.plot(partitions2, valores_par2, valores_seq2)
matplotlib.pyplot.ylim(0, 6000)
"""
"""
matplotlib.pyplot.title('Execution Time')
matplotlib.pyplot.xlabel('Number of Partitions')
matplotlib.pyplot.ylabel('Time (s)')
plt.xticks(rotation=45)

"""

fig, ax = plt.subplots(2)
plt.tight_layout(0.8)

ax[0].set_title('Execution Time')
ax[0].set(ylabel='Time (s)')
plt.setp(ax[0].xaxis.get_majorticklabels(), rotation=45)
ax[0].xaxis.set_major_formatter(plt.NullFormatter())
ax[0].plot(partitions, valores_seq, valores_par)

ax[1].set_title('Memory Consumption')
ax[1].set(xlabel='Number of Partitions', ylabel='Memory (MiB)')
plt.setp(ax[1].xaxis.get_majorticklabels(), rotation=45)
ax[1].plot(partitions, valores_seq2, valores_par2)

plt.tight_layout()
plt.savefig('/home/tales/graph.eps', format='eps')
#matplotlib.pyplot.show()

