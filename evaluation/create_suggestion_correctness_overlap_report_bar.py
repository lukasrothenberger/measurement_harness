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
import matplotlib.patches as mpatches

def add_suggestion_correctness_overlap_report_bar(benchmark_name: str, ax) -> None:

    # load complexity information
    overlap_information: Dict[str, Dict[str, Dict[str, int]]] = dict()
    with open("data/overlap/suggestion_overlap.json", "r") as f:
        overlap_information = json.load(f)

    # load data race information
    data_race_information: Dict[str, Dict[str, Dict[str, int]]] = dict()
    with open("data/data_races/data_races.json", "r") as f:
        data_race_information = json.load(f)

    # collect benchmark names and profiling times
    benchmark_names: List[str] = [k for k in data_race_information if k in overlap_information]

    # collect profiling times
    values = dict()

    species = (
        "Benchmark1",
    )

    width = 0.5


#        values = {
#        "without_metadata": np.array([len(overlap_information[benchmark_names[idx]]["without_metadata"])]),
#        "overlap": np.array([len(overlap_information[benchmark_names[idx]]["overlap"])]),
#        "with_metadata": np.array([len(overlap_information[benchmark_names[idx]]["with_metadata"])]),
#        }

    if benchmark_name not in benchmark_names:
        return

    size = 0.25

    # get total list of suggestions
    total: List[str] = overlap_information[benchmark_name]["without_metadata"] + overlap_information[benchmark_name]["with_metadata"] + overlap_information[benchmark_name]["overlap"]

    # calculate correctness and overlap values
    valid: List[str] = []
    valid_vanilla: List[str] = []  # level 1
    valid_both: List[str] = []
    valid_extended: List[str] = []

    # level 2: copy of level 1 for valid suggestions, since no issue might be detected
    invalid: List[str] = []  # level 0
    invalid_vanilla: List[str] = [] # level 2
    invalid_both: List[str] = []
    invalid_extended: List[str] = []

    # categorize suggestions
    for suggestion_tag in overlap_information[benchmark_name]["without_metadata"]:
        if [suggestion_tag] in data_race_information[benchmark_name]["without_metadata"]["with_race"]:
            invalid.append(suggestion_tag)
            invalid_vanilla.append(suggestion_tag)                
        if [suggestion_tag] in data_race_information[benchmark_name]["without_metadata"]["without_race"]:
            valid.append(suggestion_tag)
            valid_vanilla.append(suggestion_tag)
    for suggestion_tag in overlap_information[benchmark_name]["with_metadata"]:
        if [suggestion_tag] in data_race_information[benchmark_name]["with_metadata"]["with_race"]:
            invalid.append(suggestion_tag)
            invalid_extended.append(suggestion_tag)                
        if [suggestion_tag] in data_race_information[benchmark_name]["with_metadata"]["without_race"]:
            valid.append(suggestion_tag)
            valid_extended.append(suggestion_tag)
    for suggestion_tag in overlap_information[benchmark_name]["overlap"]:
        if [suggestion_tag] in data_race_information[benchmark_name]["with_metadata"]["with_race"] or [suggestion_tag] in data_race_information[benchmark_name]["without_metadata"]["with_race"]:
            invalid.append(suggestion_tag)
            invalid_both.append(suggestion_tag)                
        if [suggestion_tag] in data_race_information[benchmark_name]["with_metadata"]["without_race"] or [suggestion_tag] in data_race_information[benchmark_name]["without_metadata"]["without_race"]:
            valid.append(suggestion_tag)
            valid_both.append(suggestion_tag)

    
    if False:
        print("Benchmark: ", benchmark_name)
        print("\tvalid: ", len(valid))
        print("\t\tvanilla: ", len(valid_vanilla))
        print("\t\tboth: ", len(valid_both))
        print("\t\textended: ", len(valid_extended))
        print("\tinvalid: ", len(invalid))
        print("\t\tvanilla: ", len(invalid_vanilla))
        print("\t\tboth: ", len(invalid_both))
        print("\t\textended: ", len(invalid_extended))

    # sample correctness and overlap chart
    vals = np.array([[len(valid)], [len(invalid)]])

    level_1_vals = [len(valid), len(invalid)]

    level_2_vals = [len(valid_vanilla), len(valid_both), len(valid_extended), len(invalid_vanilla), len(invalid_both), len(invalid_extended)]

    cmap = plt.get_cmap("Pastel1")

    level_1_colors = cmap([2,0,0])
    level_2_colors = ["lightgrey", "grey",  "black", "lightgrey", "grey", "black"]


#    # DEBUG define dummy data
#    invalid_extended = [1,2]
#    invalid_both = [1,2,3,4]
#    invalid_vanilla = [1,2,4,6,7,3]
#
#    valid_extended = [1,2,3,4,5,6,7]
#    valid_both = [1,2,3]
#    valid_vanilla = [2,3]
#    # END DEBUG

    a = [-len(invalid_both),-len(invalid_both)]
    b = [-len(invalid_vanilla),-len(invalid_extended)]
    
    c = [len(valid_both),len(valid_both)]
    d = [len(valid_vanilla),len(valid_extended)]
    data = np.array([a, b, c, d])

    data_shape = np.shape(data)

    # Take negative and positive data apart and cumulate
    def get_cumulated_array(data, **kwargs):
        cum = data.clip(**kwargs)
        cum = np.cumsum(cum, axis=0)
        d = np.zeros(np.shape(data))
        d[1:] = cum[:-1]
        return d  

    cumulated_data = get_cumulated_array(data, min=0)
    cumulated_data_neg = get_cumulated_array(data, max=0)

    # Re-merge negative and positive data.
    row_mask = (data<0)
    cumulated_data[row_mask] = cumulated_data_neg[row_mask]
    data_stack = cumulated_data

    cols = ["grey", "darksalmon", "grey", "darkseagreen"]

    labels = ("vanilla", "extended")

    width = 0.5

    for i in np.arange(0, data_shape[0]):
        #np.arange(data_shape[1])
        ax.bar(labels, data[i], width, bottom=data_stack[i], color=cols[i],)

    ax.set_ylabel("# Suggestions")
    ax.axhline(0, color='black')
    ax.set_title("Distribution")

    
if __name__ == "__main__":
    fig, axs = plt.subplots(2, 2)
    fig.set_figwidth(10)
    fig.set_figheight(7.5)
    add_suggestion_correctness_overlap_report_bar("burkardt/md/md_orig", axs[0,1])

    fig.tight_layout()

    plt.show()
