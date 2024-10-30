# Import libraries
import matplotlib.pyplot as plt
import numpy as np


# Creating dataset1
np.random.seed(10)
data1 = (0, 10, 5, 2, 7) # np.random.normal(100, 20, 200)

# Creating dataset2
np.random.seed(1240)
data2 = np.random.normal(100, 20, 200)

boxplots_dict = {"Label1": data1, "Label2": data2}

#fig = plt.figure(figsize =(10, 7))
fig, ax = plt.subplots()

ax.boxplot(boxplots_dict.values())
ax.set_xticklabels(boxplots_dict.keys())

# Creating plot 1
# plt.boxplot(data1)

plt.scatter(x=[1,2,3,4,5], y=[100, 30, 12, 92, 50])


# show plot
plt.show()
