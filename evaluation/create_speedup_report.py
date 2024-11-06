# report profiling time

import sys
from typing import Dict, List, Set, Tuple
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
import json

from discopop_library.GlobalLogger.setup import setup_logger
from discopop_library.ArgumentClasses.GeneralArguments import GeneralArguments
import logging

def add_speedup_report(benchmark_name: str, ax) -> None:

    # load speedup information
    speedup_information: Dict[str, Dict[str, Dict[str, int]]] = dict()
    with open("data/speedup/runtime_data.json", "r") as f:
        speedup_information = json.load(f)

    # collect benchmark names and profiling times
    benchmark_names: List[str] = list(speedup_information.keys())

    # get benchmark name
    name = None
    for bmn in benchmark_names:
        if benchmark_name in bmn:
            name = bmn
    if name is None:    
        # no data available:
        return

    # collect profiling times
    values = dict()

    species = (
        "vanilla",
        "extended",
    )

    values = {
    "SEQ": np.array([speedup_information[name]["without_metadata"]["SEQ"], speedup_information[name]["with_metadata"]["SEQ"]]),
    "BEST": np.array([speedup_information[name]["without_metadata"]["BEST"], speedup_information[name]["with_metadata"]["BEST"]]),
    "REF": np.array([speedup_information[name]["without_metadata"]["REF"], speedup_information[name]["with_metadata"]["REF"]]),
    }

    max_value = 0.0
    for key in values:
        for entry in values[key]:
            if entry > max_value:
                max_value = entry

    x = np.arange(len(species))  # the label locations
    width = 0.25  # the width of the bars
    multiplier = 0

    for attribute, measurement in values.items():
        offset = width * multiplier
        rects = ax.bar(x + offset, measurement, width, label=attribute)
        ax.bar_label(rects, padding=3)
        multiplier += 1

    ax.set_ylabel("runtime [s]")
    ax.set_title("Execution times")
    ax.set_xticks(x + width, species)
    ax.legend(loc='upper right', ncols=1)
    ax.set_ylim(0, max_value*1.25)

    
    ax.plot()

