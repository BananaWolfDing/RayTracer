import matplotlib.pyplot as pyplot
import numpy as N
import pandas as P
import subprocess
import seaborn
import sys

seaborn.set_style('whitegrid')

FILENAME_CSV = "geodesics.csv"
FILENAME_PNG = "geodesics.png"

px = N.zeros((15,3))
px[:,0] = N.linspace(-15,15,15)
px[:,2] = 50

fCsv = open(FILENAME_CSV, "w")
# Execute the geodesic program
command = [
    "./test.sh",
    "Geodesic",
    "--spin", ".05",
    "--radius", "5",
    "--c", "10",
    "--boundary", "0",
    "--stepsize", "0.001",
]
print(" ".join(command))
subprocess.run(command,
        input=f"{px.shape[0]}\n" + "\n".join(" ".join(str(y) for y in x) for x in list(px)),
        encoding='ascii',
        stdout=fCsv)





# Draw the geodesics


df = P.read_csv(FILENAME_CSV)
print(df.columns)

fig, ax = pyplot.subplots(2, 1, figsize=(12, 18))
seaborn.lineplot(x="x", y="z", hue="i", data=df, legend='full', ax=ax[0])
seaborn.lineplot(x="step", y="h", hue="i", data=df, legend='full', ax=ax[1])
ax[1].set_yscale('log')
fig.savefig(FILENAME_PNG, dpi=300)

