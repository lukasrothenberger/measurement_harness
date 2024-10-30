from enum import IntEnum
import os
import logging
from typing import Dict, List, Set, Tuple
from discopop_library.GlobalLogger.setup import setup_logger
from discopop_library.ArgumentClasses.GeneralArguments import GeneralArguments
from pathlib import Path
import json

from enums import DPVersion

def get_benchmark_names() -> List[str]:
    benchmark_names: List[str] = []
    for path in Path('data').rglob('*/*.json'):
        with open(path, "r") as f:
            values = json.load(f)
            for key in values:
                benchmark_names.append(key)

    # remove "duplicates" due to path extensions (e.g. build, bin)
    to_be_removed: Set[str] = set()
    for elem1 in benchmark_names:
        for elem2 in benchmark_names:
            if elem1 != elem2 and elem1 in elem2:
                # elem1 is prefix of elem2
                # remove elem2
                to_be_removed.add(elem2)
    
    for elem in to_be_removed:
        if elem in benchmark_names:
            benchmark_names.remove(elem)


    return sorted(list(set(benchmark_names)))
    


if __name__ == "__main__":
    get_benchmark_names()

