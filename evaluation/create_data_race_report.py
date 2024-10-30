# report profiling time

from typing import Dict, List, Set, Tuple
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
import json

from discopop_library.GlobalLogger.setup import setup_logger
from discopop_library.ArgumentClasses.GeneralArguments import GeneralArguments
import logging

setup_logger(GeneralArguments(log_level="INFO", write_log=False)) 
logger = logging.getLogger("DataCollection").getChild("DataRace").getChild("Report")

# load data race information
data_race_information: Dict[str, Dict[str, Dict[str, int]]] = dict()
with open("data/data_races/data_races.json", "r") as f:
    data_race_information = json.load(f)

print("CI: ", data_race_information)

# collect benchmark names and profiling times
benchmark_names: List[str] = list(data_race_information.keys())

# collect profiling times
values = dict()

fig, axes = plt.subplots(len(benchmark_names))

species = (
    "without metadata",
    "with metadata",
)

width = 0.5

for idx, ax in enumerate(fig.axes):
    values = {
    "without race": np.array([data_race_information[benchmark_names[idx]]["without_metadata"]["without_race"], data_race_information[benchmark_names[idx]]["with_metadata"]["without_race"]]),
    "with race": np.array([data_race_information[benchmark_names[idx]]["without_metadata"]["with_race"], data_race_information[benchmark_names[idx]]["with_metadata"]["with_race"]]),
    }

    bottom = np.zeros(2)
    for boolean, value in values.items():
        p = ax.bar(species, value, width, label=boolean, bottom=bottom)
        bottom += value

    ax.legend(loc="upper left")
    ax.set_ylabel("# suggestions")
    ax.set_title(benchmark_names[idx])
    ax.plot()

fig.tight_layout()

plt.savefig("report_data_races.pdf")

plt.show()
