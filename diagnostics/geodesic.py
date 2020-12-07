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
px[:,0] = 0
px[:,1] = N.linspace(-50,50,15)
px[:,2] = 50
nSteps = 5000

fCsv = open(FILENAME_CSV, "w")
# Execute the geodesic program
command = [
    "./test.sh",
    "Geodesic",
    "--spin", ".8",
    "--radius", ".4",
    "--c", "2",
    "--steps", str(nSteps),
    "--boundary", "100",
    "--stepsize", ".1",
    "--solver", "euler",
]
print(" ".join(command))
subprocess.run(command,
        input=f"{px.shape[0]}\n" + "\n".join(" ".join(str(y) for y in x) for x in list(px)),
        encoding='ascii',
        stdout=fCsv)





# Draw the geodesics


df = P.read_csv(FILENAME_CSV)
print(df.columns)

print(f"Range: {df['x'].min()}, {df['x'].max()}, {df['z'].min()}, {df['z'].max()}")

fig, ax = pyplot.subplots(2, 2, figsize=(24, 18))
seaborn.scatterplot(x="x", y="z", hue="i", data=df, legend='full', ax=ax[0][0], linewidth=0)
seaborn.scatterplot(x="y", y="z", hue="i", data=df, legend='full', ax=ax[0][1], linewidth=0)
seaborn.lineplot(x="step", y="z", hue="i", data=df, legend='full', ax=ax[1][0])
seaborn.lineplot(x="step", y="h", hue="i", data=df, legend='full', ax=ax[1][1])
fig.savefig(FILENAME_PNG, dpi=300)


incomplete = 0
for i in range(px.shape[0]):
    if (df["i"] == i).sum() < nSteps:
        incomplete += 1

print(f"{incomplete}/{px.shape[0]} geodesics are incomplete")
