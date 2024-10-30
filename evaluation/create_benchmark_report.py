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

from create_data_race_report import add_data_race_plot
from create_profiling_slowdown_report import add_slowdown_report
from create_suggestion_complexity_report import add_suggestion_complexity_report
from create_suggestion_correctness_overlap_report import add_suggestion_correctness_overlap_report
from create_suggestion_overlap_report import add_suggestion_overlap_report

def show_benchmark_report(benchmark_name: str) -> None:

    # Some example data to display
    x = np.linspace(0, 2 * np.pi, 400)
    y = np.sin(x ** 2)

    fig, axs = plt.subplots(2, 3)
    fig.set_figwidth(10)
    fig.set_figheight(5)
    fig.suptitle(benchmark_name)

    # collect plots for benchmark
    add_data_race_plot(benchmark_name, axs[0, 0])
    add_suggestion_overlap_report(benchmark_name, axs[1,0])
    add_suggestion_complexity_report(benchmark_name, axs[0,1])
    add_slowdown_report(benchmark_name, axs[1,1])

    add_suggestion_correctness_overlap_report(benchmark_name, axs[0,2])
    axs[1,2].axis("off")
    
    fig.tight_layout()

    plt.show()

if __name__ == "__main__":
    show_benchmark_report("burkardt/md/md_orig")