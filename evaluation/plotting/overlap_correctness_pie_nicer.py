# Import libraries
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as mpatches

fig, ax = plt.subplots()

#size = 0.25
size = 0.25


#vals = np.array([[60., 32.], [37., 40.], [29., 10.]])
#vals = np.array([[66., 12.], [33., 41.]])


# sample correctness and overlap values
valid = 60  # level 0

valid_vanilla = 5  # level 1
valid_both = 40
valid_extended = 15

# level 2: copy of level 1 for valid suggestions, since no issue might be detected

invalid = 40  # level 0

invalid_error = 10  # level 1
invalid_dr = 30

invalid_error_vanilla = 6 # level 2
invalid_dr_vanilla = 18

invalid_error_both = 2
invalid_dr_both = 3

invalid_error_extended = 2
invalid_dr_extended = 9

# sample correctness and overlap chart
vals = np.array([[valid], [invalid]])

level_1_vals = [valid, invalid_error, invalid_dr]

level_2_vals = [valid_vanilla, valid_both, valid_extended, invalid_error_vanilla, invalid_error_both, invalid_error_extended, invalid_dr_vanilla, invalid_dr_both, invalid_dr_extended]

cmap = plt.get_cmap("Pastel1")
#level_0_colors = ["lightgrey", "lightgrey"]
#level_1_colors = ["lightgrey", "grey", "grey"]
#level_2_colors = ["black", "grey",  "lightgrey", "black", "grey", "lightgrey"]


level_1_colors = cmap([2,0,0])
level_2_colors = ["black", "grey",  "lightgrey", "black", "grey", "lightgrey"]


# inverted

level_1_patches, level_1_texts = ax.pie(level_1_vals, radius=1-size, colors=level_1_colors,
       wedgeprops=dict(width=3*size, edgecolor='k'), hatch=["||", "..", "--"])

level_2_patches, level_2_texts = ax.pie(level_2_vals, radius=1, colors=level_2_colors,
       wedgeprops=dict(width=size, edgecolor='k'))
# end inverted

# orig
#level_0_patches, level_0_texts = ax.pie(level_0_vals, radius=1, colors=level_0_colors,
#       wedgeprops=dict(width=size, edgecolor='k'), hatch=["++", "\\\\"])
#level_1_patches, level_1_texts = ax.pie(level_1_vals, radius=1-size, colors=level_1_colors,
#       wedgeprops=dict(width=size, edgecolor='k'), hatch=["++", "OO", "///"])
#level_2_patches, level_2_texts = ax.pie(level_2_vals, radius=1-(2*size), colors=level_2_colors,
#       wedgeprops=dict(width=size, edgecolor='k'))
# end orig


# remove lines between patches of different levels if requested
#level_1_patches[0].set_linewidth(0)

#ax.pie(vals.flatten(), radius=1-(2*size), colors=inner_colors,
#       wedgeprops=dict(width=size, edgecolor='w'))

# define level_0_legend
valid_patch = mpatches.Patch(color=level_1_colors[0], label='valid')
invalid_patch = mpatches.Patch(color=level_1_colors[1], label='invalid')
level_0_legend = plt.legend(handles=[valid_patch, invalid_patch], loc=1)

# define level 1 legend
error_patch = mpatches.Patch(color=level_1_colors[1], label='error')
error_patch.set_hatch("..")
error_patch.set_edgecolor("k")
dr_patch = mpatches.Patch(color=level_1_colors[2], label='data race')
dr_patch.set_hatch("--")
dr_patch.set_edgecolor("k")
level_1_legend = plt.legend(handles=[error_patch, dr_patch], loc=4)

# define level 2 legend
vanilla_patch = mpatches.Patch(color="lightgrey", label='vanilla')
both_patch = mpatches.Patch(color="grey", label='both versions')
extended_patch = mpatches.Patch(color="black", label='extended')
level_2_legend = plt.legend(handles=[vanilla_patch, both_patch, extended_patch], loc=2)

# register legends
plt.gca().add_artist(level_0_legend)
plt.gca().add_artist(level_1_legend)

ax.set(aspect="equal", title='Suggestions')
plt.show()