
# Import the modules 
from pathlib import Path
from typing import List
import warnings
import matplotlib.pyplot as plt 
from matplotlib.backends.backend_pdf import PdfPages
import sys
import os
  

valid_file_paths: List[str] = []

# get paths to measurement.csv from command line arguments, if supplied
if len(sys.argv) > 1:
    file_paths = sys.argv[1:]
    for file_path in file_paths:
        if not os.path.exists(file_path):
            warnings.warn("FileNotFound: " + file_path + " --> Skipping")
            continue
        valid_file_paths.append(file_path)

else:
    # get paths to measurements from searching
    for file_path in Path(os.getcwd()).rglob('measurements.csv'):
        print(file_path.name , "->", file_path)
        if not os.path.exists(file_path):
            warnings.warn("FileNotFound: " + file_path + " --> Skipping")
            continue
        valid_file_paths.append(file_path)

# configure plot
cols = int(1)
rows = int((len(valid_file_paths) + (len(valid_file_paths) % 2)) / cols)
fig, axes = plt.subplots(nrows=rows, ncols=cols, figsize=(10*cols, 5*rows))
fig.tight_layout(pad=8)

current_idx = 1
for file_path in valid_file_paths:
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

            # cleanup the print
            id = id.replace("sequential", "seq")
            if id.startswith("./code_"):
                id = id.replace("./code_", "")

            # get the id of the optimized solution
            try:
                if int(id) > optimized_solution_id:
                    optimized_solution_id = int(id)
            except ValueError:
                pass

            if id not in data: 
                data[id] = [] 
            data[id].append((id, float(time))) 

    for i, l in enumerate(data.keys()): 
        #plt.subplot(positions[i]) 
        plt.subplot(rows, cols, current_idx)
        data_i = dict(data[l]) 
        # determine bar color
        if "seq" in data_i.keys():
            bar_color = "y"
        elif str(optimized_solution_id) in data_i.keys():
            bar_color = "g"
        else:
            bar_color = "b"

        plt.bar(data_i.keys(), data_i.values(), color=bar_color) 
        plt.xlabel("Code version") 
        plt.ylabel("Execution time [s]")
        
    plt.title(file_path)
    
    # increase the index 
    current_idx = current_idx + 1

# create PDF report
pdf = PdfPages("measurement_report.pdf")
pdf.savefig()
pdf.close()

