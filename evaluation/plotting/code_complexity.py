# Import libraries
import matplotlib.pyplot as plt
import numpy as np


# Creating dataset1
np.random.seed(10)
data1 = (0, 10, 5, 2, 7) # np.random.normal(100, 20, 200)

# Creating dataset2
np.random.seed(1240)
data2 = np.random.normal(100, 20, 200)


# define LOC data
loc_npb_bt = 3270
loc_npb_cg = 961
loc_npb_lu = 3279
loc_npb_mg = 1268

loc_pb_2mm = 218
loc_pb_3mm = 985
loc_pb_atax = 174

loc_rodinia_hotspot = 360
loc_rodinia_kmeans = 3000
loc_rodinia_lud = 1269
loc_rodinia_nn = 305

# define cyclomatic complexity data
cc_npb_bt = (1, 65, 11, 1, 12)
cc_npb_cg = (1, 53, 8, 1, 12)
cc_npb_lu = (1, 50, 12, 2, 24)
cc_npb_mg = (1, 57, 8, 1, 9)

cc_pb_2mm = (1, 9, 3, 1, 6)
cc_pb_3mm = (1, 10, 3, 1, 6)
cc_pb_atax = (1, 7, 2, 1, 5)

cc_rodinia_hotspot = (1, 24, 6, 1, 10)
cc_rodinia_kmeans = (1,23,8,2,23)
cc_rodinia_lud = (1,18,6,2,13)
cc_rodinia_nn = (20,20,20,20,20)

boxplots_dicts = [
    { "label":"NPB.bt",
     "med":cc_npb_bt[2],
     "q1" : cc_npb_bt[3],
     "q3" : cc_npb_bt[4],
     "whislo" : cc_npb_bt[0],
     "whishi" : cc_npb_bt[1],
     "fliers": []
    },
    { "label":"NPB.cg",
     "med":cc_npb_cg[2],
     "q1" : cc_npb_cg[3],
     "q3" : cc_npb_cg[4],
     "whislo" : cc_npb_cg[0],
     "whishi" : cc_npb_cg[1],
     "fliers": []
    },
    { "label":"NPB.lu",
     "med":cc_npb_lu[2],
     "q1" : cc_npb_lu[3],
     "q3" : cc_npb_lu[4],
     "whislo" : cc_npb_lu[0],
     "whishi" : cc_npb_lu[1],
     "fliers": []
    },
    { "label":"NPB.mg",
     "med":cc_npb_mg[2],
     "q1" : cc_npb_mg[3],
     "q3" : cc_npb_mg[4],
     "whislo" : cc_npb_mg[0],
     "whishi" : cc_npb_mg[1],
     "fliers": []
    },

    { "label":"PB.2mm",
     "med":cc_pb_2mm[2],
     "q1" : cc_pb_2mm[3],
     "q3" : cc_pb_2mm[4],
     "whislo" : cc_pb_2mm[0],
     "whishi" : cc_pb_2mm[1],
     "fliers": []
    },
    { "label":"PB.3mm",
     "med":cc_pb_3mm[2],
     "q1" : cc_pb_3mm[3],
     "q3" : cc_pb_3mm[4],
     "whislo" : cc_pb_3mm[0],
     "whishi" : cc_pb_3mm[1],
     "fliers": []
    },
    { "label":"PB.atax",
     "med":cc_pb_atax[2],
     "q1" : cc_pb_atax[3],
     "q3" : cc_pb_atax[4],
     "whislo" : cc_pb_atax[0],
     "whishi" : cc_pb_atax[1],
     "fliers": []
    },

    { "label":"rdn.hotspot",
     "med":cc_rodinia_hotspot[2],
     "q1" : cc_rodinia_hotspot[3],
     "q3" : cc_rodinia_hotspot[4],
     "whislo" : cc_rodinia_hotspot[0],
     "whishi" : cc_rodinia_hotspot[1],
     "fliers": []
    },
    { "label":"rdn.kmeans",
     "med":cc_rodinia_kmeans[2],
     "q1" : cc_rodinia_kmeans[3],
     "q3" : cc_rodinia_kmeans[4],
     "whislo" : cc_rodinia_kmeans[0],
     "whishi" : cc_rodinia_kmeans[1],
     "fliers": []
    },
    { "label":"rdn.lud",
     "med":cc_rodinia_lud[2],
     "q1" : cc_rodinia_lud[3],
     "q3" : cc_rodinia_lud[4],
     "whislo" : cc_rodinia_lud[0],
     "whishi" : cc_rodinia_lud[1],
     "fliers": []
    },
    { "label":"rdn.nn",
     "med":cc_rodinia_nn[2],
     "q1" : cc_rodinia_nn[3],
     "q3" : cc_rodinia_nn[4],
     "whislo" : cc_rodinia_nn[0],
     "whishi" : cc_rodinia_nn[1],
     "fliers": []
    },

#    "rodinia.hotspot": cc_rodinia_hotspot,
#    "rodinia.kmeans": cc_rodinia_kmeans,
#    "rodinia.lud": cc_rodinia_lud,
#    "rodinia.nn": cc_rodinia_nn,
]

#fig = plt.figure(figsize =(10, 7))

fig, ax = plt.subplots()
ax1 = ax.twinx()

ax.bxp(boxplots_dicts)
# ax.set_xticklabels(boxplots_dict.keys())
ax.set_ylabel("Cyclomatic complexity")


# Plotting lines of code
# plt.boxplot(data1)
LOCs = (
    loc_npb_bt,
    loc_npb_cg,
    loc_npb_lu,
    loc_npb_mg,

    loc_pb_2mm,
    loc_pb_3mm,
    loc_pb_atax,

    loc_rodinia_hotspot,
    loc_rodinia_kmeans,
    loc_rodinia_lud,
    loc_rodinia_nn
)

#print("ASDF: ", [elem["label"] for elem in boxplots_dicts])
#ax1.scatter(x=[elem["label"] for elem in boxplots_dicts], y=LOCs)
ax1.scatter(x=np.arange(1, len(LOCs)+1), y=LOCs)
#ax1.spines["right"].set_color("blue")
ax1.tick_params(axis="y", colors="#1f77b4")
ax1.set_ylabel("Lines of code")


# show plot
plt.show()
