from enum import IntEnum
import os
import logging
from typing import Dict, List, Tuple
from discopop_library.GlobalLogger.setup import setup_logger
from discopop_library.ArgumentClasses.GeneralArguments import GeneralArguments
from pathlib import Path
import json
import sys
from enums import DPVersion

setup_logger(GeneralArguments(log_level="INFO", write_log=False)) 
logger = logging.getLogger("DataCollection").getChild("SuggestionOverlap").getChild("Driver")

logger.info("collecting suggestion overlap")

# prepare data folder strucutre
if not os.path.exists("data"):
    os.mkdir("data")
if not os.path.exists("data/overlap"):
    os.mkdir("data/overlap")

# collect paths to suggestions
benchmark_name_and_paths: Dict[str, Dict[bool, Path]] = dict()  # bool: metadata used?
for path in Path('../auto_tuner_benchmarks').rglob('*/.discopop/explorer/patterns.json'):
    clean_path = str(path)[len("../auto_tuner_benchmarks") + 1 : str(path).index("/.discopop/")]

    if ("_with_metadata") in clean_path:
        benchmark_name = clean_path[:clean_path.index("_with_metadata")]
    elif "_without_metadata" in clean_path:
        benchmark_name = clean_path[:clean_path.index("_without_metadata")]
    else:
        logger.debug("Skipping path: " + str(path) + " since no categorization based on the file name was possible")
        continue
    
    if benchmark_name not in benchmark_name_and_paths:
        benchmark_name_and_paths[benchmark_name] = dict()

    if "_with_metadata" in clean_path:
        benchmark_name_and_paths[benchmark_name][True] = path
    else:
        benchmark_name_and_paths[benchmark_name][False] = path

# print("BMNP: ", benchmark_name_and_paths)


# collect overlap data
overlap_data: Dict[str, Dict[str, List[str]]] = dict()
for benchmark_name in benchmark_name_and_paths:
    with_metadata_suggestions_tags: List[str] = []
    without_metadata_suggestions_tags: List[str] = []
    
    # collect data for suggestions generated with metadata
    with open(benchmark_name_and_paths[benchmark_name][True], "r") as f:        
        suggestion_results = json.load(f)
        for pattern_type in suggestion_results["patterns"]:
            for pattern in suggestion_results["patterns"][pattern_type]:
                if "pattern_tag" in pattern:
                    with_metadata_suggestions_tags.append(pattern["pattern_tag"])

    # collect data for suggestions generated without metadata
        with open(benchmark_name_and_paths[benchmark_name][False], "r") as f:        
            suggestion_results = json.load(f)
            for pattern_type in suggestion_results["patterns"]:
                for pattern in suggestion_results["patterns"][pattern_type]:
                    if "pattern_tag" in pattern:
                        without_metadata_suggestions_tags.append(pattern["pattern_tag"])

    # get overlap
    overlapping_suggestion_tags: List[str] = []

    for suggestion_tag in with_metadata_suggestions_tags:
        if suggestion_tag in without_metadata_suggestions_tags:
            overlapping_suggestion_tags.append(suggestion_tag)

    # cleanup
    for suggestion_tag in overlapping_suggestion_tags:
        with_metadata_suggestions_tags.remove(suggestion_tag)
        without_metadata_suggestions_tags.remove(suggestion_tag)
                     
    # store data
    overlap_data[benchmark_name] = dict()
    overlap_data[benchmark_name]["with_metadata"] =  with_metadata_suggestions_tags
    overlap_data[benchmark_name]["without_metadata"] =  without_metadata_suggestions_tags
    overlap_data[benchmark_name]["overlap"] =  overlapping_suggestion_tags


with open("data/overlap/suggestion_overlap.json", "w+") as f:
    json.dump(overlap_data, f)



