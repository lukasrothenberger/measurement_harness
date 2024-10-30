from enum import IntEnum
import os
import logging
from typing import Dict, Tuple
from discopop_library.GlobalLogger.setup import setup_logger
from discopop_library.ArgumentClasses.GeneralArguments import GeneralArguments
from pathlib import Path
import json

from enums import DPVersion

def collect_data() -> None:
    setup_logger(GeneralArguments(log_level="INFO", write_log=False)) 
    logger = logging.getLogger("DataCollection").getChild("Complexity").getChild("Driver")

    logger.info("collecting suggestion complexity data")

    # prepare data folder strucutre
    if not os.path.exists("data"):
        os.mkdir("data")
    if not os.path.exists("data/suggestion_complexity"):
        os.mkdir("data/suggestion_complexity")

    # collect paths to suggestion statistics
    benchmark_name_and_paths: Dict[str, Dict[bool, Path]] = dict()  # bool: metadata used?
    for path in Path('../auto_tuner_benchmarks').rglob('*/.discopop/explorer/statistics/suggestion_statistics_by_suggestionID.json'):
        clean_path = str(path)[len("../auto_tuner_benchmarks") + 1 : str(path).index("/.discopop/")]

        if "_with_metadata_" in clean_path or "_without_metadata_" in clean_path:
            # skip autotuner versions
            continue

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

    # collect complexity data 
    complexity_data: Dict[str, Dict[str, Dict[str, int]]] = dict()
    for benchmark_name in benchmark_name_and_paths:
        # collect data for suggestions generated with metadata
        with_metadata_suggestions_with_function_calls = 0
        with_metadata_suggestions_without_function_calls = 0

        with open(benchmark_name_and_paths[benchmark_name][True], "r") as f:
            suggestion_statistics = json.load(f)
            for suggestion_id in suggestion_statistics:
                if suggestion_statistics[suggestion_id]["suggestion_call_path_depth"] > 0:
                    with_metadata_suggestions_with_function_calls += 1
                else:
                    with_metadata_suggestions_without_function_calls += 1
        
        # collect data for suggestions generated without metadata
        without_metadata_suggestions_with_function_calls = 0
        without_metadata_suggestions_without_function_calls = 0

        with open(benchmark_name_and_paths[benchmark_name][False], "r") as f:
            suggestion_statistics = json.load(f)
            for suggestion_id in suggestion_statistics:
                if suggestion_statistics[suggestion_id]["suggestion_call_path_depth"] > 0:
                    without_metadata_suggestions_with_function_calls += 1
                else:
                    without_metadata_suggestions_without_function_calls += 1
        
        # store data
        complexity_data[benchmark_name] = dict()

        complexity_data[benchmark_name]["with_metadata"] = dict()
        complexity_data[benchmark_name]["with_metadata"]["with_calls"] = with_metadata_suggestions_with_function_calls
        complexity_data[benchmark_name]["with_metadata"]["without_calls"] = with_metadata_suggestions_without_function_calls

        complexity_data[benchmark_name]["without_metadata"] = dict()
        complexity_data[benchmark_name]["without_metadata"]["with_calls"] = without_metadata_suggestions_with_function_calls
        complexity_data[benchmark_name]["without_metadata"]["without_calls"] = without_metadata_suggestions_without_function_calls

    with open("data/suggestion_complexity/suggestion_complexity.json", "w+") as f:
        json.dump(complexity_data, f)


if __name__ == "__main__":
    collect_data()

