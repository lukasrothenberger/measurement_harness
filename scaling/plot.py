# load measurements_inputs
from typing import List
import os
from matplotlib import pyplot as plt
import sys

print("argv: ", sys.argv)

if len(sys.argv) < 2: 
    raise ValueError("Path to the experiment folder to be plotted missing!")

experiment_path = sys.argv[1]
inputs_path = os.path.join(experiment_path, "measurements_inputs.csv")
sequential_path = os.path.join(experiment_path, "measurements_sequential.csv")
vanilla_path = os.path.join(experiment_path, "measurements_vanilla.csv")
extended_path = os.path.join(experiment_path, "measurements_extended.csv")

# check existence of files
for fp in [experiment_path, inputs_path, sequential_path, vanilla_path, extended_path]:
    if not os.path.exists(fp):
        raise FileNotFoundError(fp)

measurements_inputs: List[str] = []
with open(inputs_path, "r") as f:
    first_line = True
    for line in f.readlines():
        if first_line:
            first_line = False
            continue
        line = line.replace(";", "").replace("\n", "")
        measurements_inputs.append(line)

# load measurements_sequential
measurements_sequential: List[float] = []
with open(sequential_path, "r") as f:
    first_line = True
    for line in f.readlines():
        if first_line:
            first_line = False
            continue
        line = line.replace(";", "").replace("\n", "")
        measurements_sequential.append(float(line))

# load measurements_vanilla
measurements_vanilla: List[float] = []
with open(vanilla_path, "r") as f:
    first_line = True
    for line in f.readlines():
        if first_line:
            first_line = False
            continue
        line = line.replace(";", "").replace("\n", "")
        measurements_vanilla.append(float(line))

# load measurements_extended
measurements_extended: List[float] = []
with open(extended_path, "r") as f:
    first_line = True
    for line in f.readlines():
        if first_line:
            first_line = False
            continue
        line = line.replace(";", "").replace("\n", "")
        measurements_extended.append(float(line))

# create plot
plt.plot(measurements_inputs, measurements_sequential, label="sequential")
plt.plot(measurements_inputs, measurements_vanilla, label='vanilla')
plt.plot(measurements_inputs, measurements_extended, label='extended')

# configure plot
dir_name = os.path.basename(os.getcwd())
parent_name = os.path.basename(os.path.dirname(os.getcwd()))
plot_title = os.path.join(parent_name, dir_name)
plt.title(plot_title)
plt.legend()
plt.show()
