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

def show_benchmark_report(benchmark_name: str) -> None:

    # Some example data to display
    x = np.linspace(0, 2 * np.pi, 400)
    y = np.sin(x ** 2)

    fig, axs = plt.subplots(2, 2)
    fig.suptitle(benchmark_name)

    # collect plots for benchmark
    add_data_race_plot(benchmark_name, axs[0, 0])

    # plot
    #axs[0, 0] = ax_data_races
    #axs[0, 0].set_title("data races")
    axs[1, 0].plot(x, y**2)
    axs[1, 0].set_title("shares x with main")
    axs[1, 0].sharex(axs[0, 0])
    axs[0, 1].plot(x + 1, y + 1)
    axs[0, 1].set_title("unrelated")
    axs[1, 1].plot(x + 2, y + 2)
    axs[1, 1].set_title("also unrelated")
    fig.tight_layout()

    plt.show()

if __name__ == "__main__":
    show_benchmark_report("polybench/doitgen/orig")