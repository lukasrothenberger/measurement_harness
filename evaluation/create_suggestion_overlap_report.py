# report profiling time

from typing import Dict, List, Set, Tuple
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
import json

from discopop_library.GlobalLogger.setup import setup_logger
from discopop_library.ArgumentClasses.GeneralArguments import GeneralArguments
import logging
import sys

def add_suggestion_overlap_report(benchmark_name: str, ax) -> None:

    # load complexity information
    complexity_information: Dict[str, Dict[str, Dict[str, int]]] = dict()
    with open("data/overlap/suggestion_overlap.json", "r") as f:
        complexity_information = json.load(f)

    # collect benchmark names and profiling times
    benchmark_names: List[str] = list(complexity_information.keys())

    # collect profiling times
    values = dict()

    species = (
        "Results",
        ""
    )

    width = 0.5

    # get benchmark name
    name = None
    for bmn in benchmark_names:
        if benchmark_name in bmn:
            name = bmn
    if name is None:    
        # no data available:
        return

    values = {
    "vanilla": np.array([len(complexity_information[name]["without_metadata"]), 0]),
    "overlap": np.array([len(complexity_information[name]["overlap"]), 0]),
    "extended": np.array([len(complexity_information[name]["with_metadata"]), 0]),
    }

    bottom = np.zeros(2)
    for boolean, value in values.items():
        p = ax.bar(species, value, width, label=boolean, bottom=bottom)
        bottom += value

    ax.legend(loc="upper right")
    ax.set_ylabel("# valid suggestions")
    ax.set_title("Suggestion overlap")
    ax.plot()


def show_suggestion_overlap_report() -> None:

    # load complexity information
    complexity_information: Dict[str, Dict[str, Dict[str, int]]] = dict()
    with open("data/overlap/suggestion_overlap.json", "r") as f:
        complexity_information = json.load(f)

    # collect benchmark names and profiling times
    benchmark_names: List[str] = list(complexity_information.keys())

    # collect profiling times
    values = dict()

    fig, axes = plt.subplots(len(benchmark_names))

    species = (
        "Benchmark1",
    )

    width = 0.5

    for idx, ax in enumerate(fig.axes):
        values = {
        "without_metadata": np.array([len(complexity_information[benchmark_names[idx]]["without_metadata"])]),
        "overlap": np.array([len(complexity_information[benchmark_names[idx]]["overlap"])]),
        "with_metadata": np.array([len(complexity_information[benchmark_names[idx]]["with_metadata"])]),
        }

        bottom = np.zeros(2)
        for boolean, value in values.items():
            p = ax.bar(species, value, width, label=boolean, bottom=bottom)
            bottom += value

        ax.legend(loc="upper left")
        ax.set_ylabel("# valid suggestions")
        ax.set_title(benchmark_names[idx])
        ax.plot()

    fig.tight_layout()
    plt.show()

if __name__ == "__main__":
    show_suggestion_overlap_report()