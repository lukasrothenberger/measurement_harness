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

def add_suggestion_correctness_overlap_report(benchmark_name: str, ax) -> None:

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


    # inverted

    level_1_patches, level_1_texts = ax.pie(level_1_vals, radius=1-size, colors=level_1_colors,
        wedgeprops=dict(width=3*size, edgecolor='k'), hatch=["||", "..", "--"])

    level_2_patches, level_2_texts = ax.pie(level_2_vals, radius=1, colors=level_2_colors,
        wedgeprops=dict(width=size, edgecolor='k'))
    # end inverted

    # define level_0_legend
    valid_patch = mpatches.Patch(color=level_1_colors[0], label='valid')
    valid_patch.set_hatch("||")
    valid_patch.set_edgecolor("k")
    invalid_patch = mpatches.Patch(color=level_1_colors[1], label='invalid')
    invalid_patch.set_hatch("...")
    invalid_patch.set_edgecolor("k")
    level_0_legend = plt.legend(handles=[valid_patch, invalid_patch], loc=1)

    # define level 2 legend
    vanilla_patch = mpatches.Patch(color="lightgrey", label='vanilla')
    both_patch = mpatches.Patch(color="grey", label='both versions')
    extended_patch = mpatches.Patch(color="black", label='extended')
    level_2_legend = plt.legend(handles=[vanilla_patch, both_patch, extended_patch], loc=2)

    # register legends
    plt.gca().add_artist(level_0_legend)
    #plt.gca().add_artist(level_1_legend)

    ax.set(aspect="equal", title="Distribution")

    



def show_suggestion_correctness_overlap_report() -> None:

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

    fig, axes = plt.subplots(len(benchmark_names))

    species = (
        "Benchmark1",
    )

    width = 0.5

    for idx, ax in enumerate(fig.axes):
#        values = {
#        "without_metadata": np.array([len(overlap_information[benchmark_names[idx]]["without_metadata"])]),
#        "overlap": np.array([len(overlap_information[benchmark_names[idx]]["overlap"])]),
#        "with_metadata": np.array([len(overlap_information[benchmark_names[idx]]["with_metadata"])]),
#        }

        benchmark_name = benchmark_names[idx]

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


        # sample correctness and overlap chart
        vals = np.array([[len(valid)], [len(invalid)]])

        level_1_vals = [len(valid), len(invalid)]

        level_2_vals = [len(valid_vanilla), len(valid_both), len(valid_extended), len(invalid_vanilla), len(invalid_both), len(invalid_extended)]

        cmap = plt.get_cmap("Pastel1")

        level_1_colors = cmap([2,0,0])
        level_2_colors = ["black", "grey",  "lightgrey", "black", "grey", "lightgrey"]


        # inverted

        level_1_patches, level_1_texts = ax.pie(level_1_vals, radius=1-size, colors=level_1_colors,
            wedgeprops=dict(width=3*size, edgecolor='k'), hatch=["||", "..", "--"])

        level_2_patches, level_2_texts = ax.pie(level_2_vals, radius=1, colors=level_2_colors,
            wedgeprops=dict(width=size, edgecolor='k'))
        # end inverted

        # define level_0_legend
        valid_patch = mpatches.Patch(color=level_1_colors[0], label='valid')
        valid_patch.set_hatch("||")
        valid_patch.set_edgecolor("k")
        invalid_patch = mpatches.Patch(color=level_1_colors[1], label='invalid')
        invalid_patch.set_hatch("...")
        invalid_patch.set_edgecolor("k")
        level_0_legend = plt.legend(handles=[valid_patch, invalid_patch], loc=1)

        # define level 2 legend
        vanilla_patch = mpatches.Patch(color="lightgrey", label='vanilla')
        both_patch = mpatches.Patch(color="grey", label='both versions')
        extended_patch = mpatches.Patch(color="black", label='extended')
        level_2_legend = plt.legend(handles=[vanilla_patch, both_patch, extended_patch], loc=2)

        # register legends
        plt.gca().add_artist(level_0_legend)
        #plt.gca().add_artist(level_1_legend)

        ax.set(aspect="equal", title="Distribution")

    fig.tight_layout()
    plt.show()

if __name__ == "__main__":
    show_suggestion_correctness_overlap_report()