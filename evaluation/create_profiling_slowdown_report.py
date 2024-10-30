# report profiling time

from typing import Dict, List, Set, Tuple
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
import json

from discopop_library.GlobalLogger.setup import setup_logger
from discopop_library.ArgumentClasses.GeneralArguments import GeneralArguments
import logging


def show_slowdown_report() -> None:

    # load profiling times
    profiling_times: Dict[str, Dict[str, int]] = dict()
    for path in Path('data/profiling_slowdown/').rglob('profiling_times.json'):
        clean_path = str(path)[len("data/profiling_slowdown/"):]
        dp_variant = clean_path[:clean_path.index("/")]
        if dp_variant not in profiling_times:
            profiling_times[dp_variant] = dict()
        
        
        # profiling_times: Dict[str, int] = dict()
        with open(path, "r") as f:
            d = json.load(f)
            for key in d:
                profiling_times[dp_variant][key] = d[key]

    # collect benchmark names and profiling times
    benchmark_names: List[str] = []
    for dp_v in profiling_times:
        for benchmark_name in profiling_times[dp_v]:
            benchmark_names.append(benchmark_name)
    benchmark_names = list(set(benchmark_names))

    # collect profiling times
    values = dict()
    for dp_v in profiling_times:
        raw_tuple: List[int] = []

        for benchmark_name in benchmark_names:
            if benchmark_name in profiling_times[dp_v]:
                raw_tuple.append(int(profiling_times[dp_v][benchmark_name]))
            else:
                raw_tuple.append(0)
        values[dp_v] = tuple(raw_tuple)

    # normalize values to calculate slowdowns
    for dp_v in values:
        if dp_v == "VANILLA":
            continue
        raw_tuple: List[int] = []
        for idx, elem in enumerate(values[dp_v]):
            if values["VANILLA"][idx] == 0:
                raw_tuple.append(0.0)
                continue
            raw_tuple.append(round((float(elem) / float(values["VANILLA"][idx])), 2))
        values[dp_v] = tuple(raw_tuple)
    values["VANILLA"] = tuple([1.0 if e != 0 else 0.0 for e in values["VANILLA"]])

    x = np.arange(len(benchmark_names))  # the label locations
    width = 0.25  # the width of the bars
    multiplier = 0

    fig, ax = plt.subplots(layout='constrained')

    for attribute, measurement in values.items():
        offset = width * multiplier
        rects = ax.bar(x + offset, measurement, width, label=attribute)
        ax.bar_label(rects, padding=3)
        multiplier += 1

    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_ylabel('Slowdown')
    ax.set_title('Profiling Slowdown')
    ax.set_xticks(x + width, benchmark_names)
    ax.legend(loc='upper left', ncols=3)
    #ax.set_ylim(0, 250)

    plt.show()


if __name__ == "__main__":
    show_slowdown_report()