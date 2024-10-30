from enum import IntEnum
import os
import logging
from typing import Dict, Tuple
from discopop_library.GlobalLogger.setup import setup_logger
from discopop_library.ArgumentClasses.GeneralArguments import GeneralArguments
from pathlib import Path
import json

from enums import DPVersion

setup_logger(GeneralArguments(log_level="INFO", write_log=False)) 
logger = logging.getLogger("DataCollection").getChild("DataRaces").getChild("Driver")

logger.info("collecting suggestion data races")

# prepare data folder strucutre
if not os.path.exists("data"):
    os.mkdir("data")
if not os.path.exists("data/data_races"):
    os.mkdir("data/data_races")

import sys

# collect paths to suggestion statistics
benchmark_name_and_paths: Dict[str, Dict[bool, Path]] = dict()  # bool: metadata used?
for path in Path('../auto_tuner_benchmarks').rglob('*/.discopop/sanity_checker/results.json'):
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

print("BMNP: ", benchmark_name_and_paths)

# collect data race data
data_race_data: Dict[str, Dict[str, Dict[str, int]]] = dict()
for benchmark_name in benchmark_name_and_paths:
    # collect data for suggestions generated with metadata
    with_metadata_suggestions_with_data_race = 0
    with_metadata_suggestions_without_data_race = 0

    with open(benchmark_name_and_paths[benchmark_name][True], "r") as f:
        suggestion_results = json.load(f)
        for suggestion_dict in suggestion_results:
            if suggestion_dict["TSAN_CODE"]:
                with_metadata_suggestions_without_data_race += 1
            else:
                with_metadata_suggestions_with_data_race += 1


    # collect data for suggestions generated without metadata
    without_metadata_suggestions_with_data_race = 0
    without_metadata_suggestions_without_data_race = 0

    with open(benchmark_name_and_paths[benchmark_name][False], "r") as f:
        suggestion_results = json.load(f)
        for suggestion_dict in suggestion_results:
            if suggestion_dict["TSAN_CODE"]:
                without_metadata_suggestions_without_data_race += 1
            else:
                without_metadata_suggestions_with_data_race += 1
                     
    # store data
    data_race_data[benchmark_name] = dict()

    data_race_data[benchmark_name]["with_metadata"] = dict()
    data_race_data[benchmark_name]["with_metadata"]["with_race"] = with_metadata_suggestions_with_data_race
    data_race_data[benchmark_name]["with_metadata"]["without_race"] = with_metadata_suggestions_without_data_race

    data_race_data[benchmark_name]["without_metadata"] = dict()
    data_race_data[benchmark_name]["without_metadata"]["with_race"] = without_metadata_suggestions_with_data_race
    data_race_data[benchmark_name]["without_metadata"]["without_race"] = without_metadata_suggestions_without_data_race


with open("data/data_races/data_races.json", "w+") as f:
    json.dump(data_race_data, f)


