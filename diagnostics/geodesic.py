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
px[:,0] = N.linspace(-30,30,15)
px[:,1] = 0
px[:,2] = 50
nSteps = 5000

fCsv = open(FILENAME_CSV, "w")
# Execute the geodesic program
command = [
    "./test.sh",
    "Geodesic",
    "--spin", ".01",
    "--radius", ".01",
    "--c", "4",
    "--steps", str(nSteps),
    "--boundary", "1000",
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
seaborn.scatterplot(x="x", y="z", hue="i", data=df, legend='full', ax=ax[0][0])
seaborn.lineplot(x="step", y="h", hue="i", data=df, legend='full', ax=ax[0][1])
seaborn.lineplot(x="step", y="z", hue="i", data=df, legend='full', ax=ax[1][0])
fig.savefig(FILENAME_PNG, dpi=300)


incomplete = 0
for i in range(px.shape[0]):
    if (df["i"] == i).sum() < nSteps:
        incomplete += 1

print(f"{incomplete}/{px.shape[0]} geodesics are incomplete")
