#!/usr/bin/env sage

import sys

N = 10000

with open(sys.argv[1], "r") as f:
    txt = f.read()
    lines = txt.split("\n")[0:-1]
    data = [list(map(int, line.split(" ")[0:2])) for line in lines]
    plt = scatter_plot(data[0:N])
    plt.save(sys.argv[2])
