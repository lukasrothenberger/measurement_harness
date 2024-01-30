
# Import the modules 
import matplotlib.pyplot as plt 
import sys
import os
  
# get path to measurement.csv
file_path = sys.argv[1]
if not os.path.exists(file_path):
    raise FileNotFoundError(file_path)
print("Parsing ", file_path)

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
for i, l in enumerate(data.keys()): 
    #plt.subplot(positions[i]) 
    plt.subplot()
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
  
# Show the plots 
plt.show() 
