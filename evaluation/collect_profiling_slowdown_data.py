from enum import IntEnum
import os
import logging
from typing import Dict, List
from discopop_library.GlobalLogger.setup import setup_logger
from discopop_library.ArgumentClasses.GeneralArguments import GeneralArguments
from pathlib import Path
import json

from enums import DPVersion

setup_logger(GeneralArguments(log_level="INFO", write_log=False)) 
logger = logging.getLogger("DataCollection").getChild("Slowdown").getChild("Driver")

dp_version = None

# get DP version
while True:
    print("current discopop configuration?")
    print("\t0: vanilla")
    print("\t1: full callstack profiling")
    print("\t2: callstack profiling with cutoff")
    try:
        dp_version = DPVersion(int(input("Version? ")))
        break
    except ValueError:
        print("Invalid value.\n")
logger.info("DP version: " + dp_version.name)


# prepare data folder strucutre
if not os.path.exists("data"):
    os.mkdir("data")
if not os.path.exists("data/profiling_slowdown"):
    os.mkdir("data/profiling_slowdown")
if not os.path.exists("data/profiling_slowdown/"+dp_version.name):
    os.mkdir("data/profiling_slowdown/"+dp_version.name)


# load old profiling times
profiling_times: Dict[str, int] = dict()
if os.path.exists("data/profiling_slowdown/" + dp_version.name + "/profiling_times.json"):
    with open("data/profiling_slowdown/" + dp_version.name +  "/profiling_times.json", "r") as f:
        profiling_times = json.load(f)
print("OLD PROFILING TIMES")
print(profiling_times)

# collect new profiling times
logger.info("Profiling times:")
to_be_removed: List[str] = []
for path in Path('../profiler_only').rglob('*/.discopop/profiler/statistics/profiling_time.txt'):
    clean_path = str(path)[len('../profiler_only')+1 : str(path).index("/.discopop/")]
    with open(path, "r") as f:
        line = f.readline()
        time = line[0:line.index(" ")]
        profiling_times[clean_path] = time
        logger.info("\t" + clean_path + " -> " + str(time) + " ms")
    
    # remove profiling time to prevent overwriting issues when executing collection multiple times
    to_be_removed.append(path)

print("NEW PROFILING TIMES")
print(profiling_times)
with open("data/profiling_slowdown/" + dp_version.name +  "/profiling_times.json", "w+") as f:
    json.dump(profiling_times, f)

for path in to_be_removed:
    if os.path.exists(path):
        os.remove(path)
        logger.info("deleted saved profiling times: " + str(path))




