from enum import IntEnum
import os
import logging
from typing import Dict, Optional, Tuple
from discopop_library.GlobalLogger.setup import setup_logger
from discopop_library.ArgumentClasses.GeneralArguments import GeneralArguments
from pathlib import Path
import json


def collect_data() -> None:
    setup_logger(GeneralArguments(log_level="INFO", write_log=False)) 
    logger = logging.getLogger("DataCollection").getChild("Speedup").getChild("Driver")

    logger.info("collecting speedup data")

    # prepare data folder strucutre
    if not os.path.exists("data"):
        os.mkdir("data")
    if not os.path.exists("data/speedup"):
        os.mkdir("data/speedup")

    # collect paths to runtime measurements
    benchmark_name_and_paths: Dict[str, Dict[bool, Path]] = dict()  # bool: metadata used?
    for path in Path('../').rglob('*/measurements.csv'):
        clean_path = str(path)[len("../auto_tuner_benchmarks") + 1 : str(path).index("/measurements.csv")]

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

    # print("BMNP: ", benchmark_name_and_paths, "\n")

    # collect runtime measurements 
    runtime_data: Dict[str, Dict[str, Dict[str, Optional[float]]]] = dict()
    for benchmark_name in benchmark_name_and_paths:
        if benchmark_name not in runtime_data:
            runtime_data[benchmark_name] = dict()
            runtime_data[benchmark_name]["with_metadata"] = dict()
            runtime_data[benchmark_name]["without_metadata"] = dict()

        # collect runtime results with metadata
        with open(benchmark_name_and_paths[benchmark_name][True], "r") as f:
            for line in f.readlines():
                line = line.replace("\n", "")
                if line.startswith("ID;"):
                    continue
                split_line = line.split(";")
                tag = split_line[0]
                runtime = float(split_line[1])
                return_code = split_line[2]
                
                if tag == "[]":
                    tag = "SEQ"

                if int(return_code) != 0:
                    runtime = None

                if tag not in ["SEQ", "REF"]:
                    tag = "BEST"

                if tag == "SEQ" and tag in runtime_data[benchmark_name]["with_metadata"]:
                    # value for SEQ already exists
                    runtime = None

                
                # initialize dict entry
                if tag not in runtime_data[benchmark_name]["with_metadata"]:
                    runtime_data[benchmark_name]["with_metadata"][tag] = runtime
                # update best value if necessary
                if runtime_data[benchmark_name]["with_metadata"][tag] is None:
                    runtime_data[benchmark_name]["with_metadata"][tag] = runtime
                elif runtime is not None: 
                    if runtime_data[benchmark_name]["with_metadata"][tag] > runtime:
                        runtime_data[benchmark_name]["with_metadata"][tag] = runtime    

        # collect runtime results without metadata
        with open(benchmark_name_and_paths[benchmark_name][False], "r") as f:
            for line in f.readlines():
                line = line.replace("\n", "")
                if line.startswith("ID;"):
                    continue
                split_line = line.split(";")
                tag = split_line[0]
                runtime = float(split_line[1])
                return_code = split_line[2]
                
                if tag == "[]":
                    tag = "SEQ"

                if int(return_code) != 0:
                    runtime = None

                if tag not in ["SEQ", "REF"]:
                    tag = "BEST"

                if tag == "SEQ" and tag in runtime_data[benchmark_name]["without_metadata"]:
                    # value for SEQ already exists
                    runtime = None

                
                # initialize dict entry
                if tag not in runtime_data[benchmark_name]["without_metadata"]:
                    runtime_data[benchmark_name]["without_metadata"][tag] = runtime
                # update best value if necessary
                if runtime_data[benchmark_name]["without_metadata"][tag] is None:
                    runtime_data[benchmark_name]["without_metadata"][tag] = runtime
                elif runtime is not None: 
                    if runtime_data[benchmark_name]["without_metadata"][tag] > runtime:
                        runtime_data[benchmark_name]["without_metadata"][tag] = runtime 

        # fill missing entries
        for tag in ["SEQ", "BEST", "REF"]:
            if tag not in runtime_data[benchmark_name]["with_metadata"]:
                runtime_data[benchmark_name]["with_metadata"][tag] = 0.0
            if tag not in runtime_data[benchmark_name]["without_metadata"]:
                runtime_data[benchmark_name]["without_metadata"][tag] = 0.0

    with open("data/speedup/runtime_data.json", "w+") as f:
        json.dump(runtime_data, f)

if __name__ == "__main__":
    collect_data()

