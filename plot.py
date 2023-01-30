"""
python3 -m venv .venv
. .venv/bin/activate
pip install numpy matplotlib
python3 plot.py
"""

import numpy
import matplotlib.pyplot as plt

filename = "a.matrix"
width = 1000
height = 1000000

data = numpy.fromfile(filename, "double")

# Source for following code: https://www.geeksforgeeks.org/how-to-calculate-and-plot-a-cumulative-distribution-function-with-matplotlib-in-python/
count, bins_count = numpy.histogram(data, bins=2000)
pdf = count / sum(count)
cdf = numpy.cumsum(pdf)
plt.plot(bins_count[1:], cdf, label="CDF")
plt.legend()
plt.savefig("cdf.png", bbox_inches="tight")
