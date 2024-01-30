
# Import the modules 
from typing import List
import warnings
import matplotlib.pyplot as plt 
import sys
import os
  
# get paths to measurement.csv
file_paths = sys.argv[1:]
current_idx = 1
valid_file_paths: List[str] = []
for file_path in file_paths:
    if not os.path.exists(file_path):
        warnings.warn("FileNotFound: " + file_path + " --> Skipping")
        continue
    valid_file_paths.append(file_path)


for file_path in file_paths:
    print("Parsing:", file_path)
    # create subplot for each measurement file

    # Initialize a dictionary for months 
    data = dict() 
    
    # Read the data 
    optimized_solution_id = -1
    with open(file_path, 'r') as f: 
        for line in f.readlines(): 
            
            # Store each line in the dictionary 
            id, time, return_code, trail = line.split(';') 
            
            if id == "ID":
                # skip header
                continue
            
            # get the id of the optimized solution
            try:
                if int(id) > optimized_solution_id:
                    optimized_solution_id = int(id)
            except ValueError:
                pass

            # cleanup the print
            id = id.replace("sequential", "seq")

            if id not in data: 
                data[id] = [] 
            data[id].append((id, float(time))) 
    
    # Position of each subplot where 221 means 2 row, 
    # 2 columns, 1st index 
    positions = [221, 222, 223, 224] 
    
    # Colors to distinguish the plot 
    colors = ['r', 'g', 'b', 'y'] 
    
    # Plot the subgraphs 
    rows = int((len(valid_file_paths) + (len(valid_file_paths) % 2)) / 2)
    cols = int(2)
    idx = current_idx
    
    for i, l in enumerate(data.keys()): 
        #plt.subplot(positions[i]) 
        plt.subplot(rows, cols, current_idx)
        data_i = dict(data[l]) 
        # determine bar color
        if "seq" in data_i.keys():
            bar_color = "r"
        elif str(optimized_solution_id) in data_i.keys():
            bar_color = "g"
        else:
            bar_color = "b"

        plt.bar(data_i.keys(), data_i.values(), color=bar_color) 
        plt.xlabel("Code version") 
        plt.ylabel("Execution time [s]")
    
    # increase the index 
    current_idx = current_idx + 1
    
# Show the plots 
plt.show() 
