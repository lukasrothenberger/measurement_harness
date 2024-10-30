# Import libraries
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as mpatches
from matplotlib.patches import Rectangle

fig, ax = plt.subplots()

#size = 0.25
size = 1/3

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

level_0_vals = [valid, invalid]

level_1_vals = [valid, invalid_error, invalid_dr]

level_2_vals = [valid_vanilla, valid_both, valid_extended, invalid_error_vanilla, invalid_error_both, invalid_error_extended, invalid_dr_vanilla, invalid_dr_both, invalid_dr_extended]

cmap = plt.get_cmap("Pastel1")
#level_0_colors = ["lightgrey", "lightgrey"]
#level_1_colors = ["lightgrey", "grey", "grey"]
#level_2_colors = ["black", "grey",  "lightgrey", "black", "grey", "lightgrey"]


level_0_colors = cmap([2,0])
level_1_colors = cmap([2,0,0])
level_2_colors = ["black", "grey",  "lightgrey", "black", "grey", "lightgrey"]


# inverted
#level_0_patches, level_0_texts = ax.pie(level_0_vals, radius=1-(2*size), colors=level_0_colors,
#       wedgeprops=dict(width=size, edgecolor='k'), hatch=["++", "\\\\"])
#
#level_1_patches, level_1_texts = ax.pie(level_1_vals, radius=1-size, colors=level_1_colors,
#       wedgeprops=dict(width=size, edgecolor='k'), hatch=["++", "OO", "///"])
#
#level_2_patches, level_2_texts = ax.pie(level_2_vals, radius=1, colors=level_2_colors,
#       wedgeprops=dict(width=size, edgecolor='k'))
# end inverted

# create plot
total = valid + invalid
# level 0
valid_offset = 0.
valid_portion = float(valid) / float(total)
invalid_offset = float(valid) / float(total)
invalid_portion = float(invalid) / float(total)
patch = plt.gca().add_patch(Rectangle((valid_offset, 0.8), valid_portion, 0.2,
                      alpha=1, facecolor=level_0_colors[0]))
patch.set_edgecolor("k")
patch.set_hatch("++")
patch.set_linewidth(0)

patch = plt.gca().add_patch(Rectangle((invalid_offset, 0.8), invalid_portion, 0.2,
                      alpha=1, facecolor=level_0_colors[1]))
patch.set_edgecolor("k")
patch.set_hatch("\\\\")

# level 1
patch = plt.gca().add_patch(Rectangle((valid_offset, 0.6), valid_portion, 0.2,
                      alpha=1, facecolor=level_1_colors[0]))
patch.set_edgecolor("k")
patch.set_hatch("++")
patch.set_linewidth(0)

invalid_error_offset = invalid_offset
invalid_error_portion = float(invalid_error) / float(total)
patch = plt.gca().add_patch(Rectangle((invalid_error_offset, 0.6), invalid_error_portion, 0.2,
                      alpha=1, facecolor=level_1_colors[1]))
patch.set_edgecolor("k")
patch.set_hatch("OO")
patch.set_linewidth(0)

invalid_dr_offset = invalid_offset + invalid_error_portion
invalid_dr_portion = float(invalid_dr) / float(total)
patch = plt.gca().add_patch(Rectangle((invalid_dr_offset, 0.6), invalid_dr_portion, 0.2,
                      alpha=1, facecolor=level_1_colors[2]))
patch.set_edgecolor("k")
patch.set_hatch("///")
patch.set_linewidth(0)

# level 2
valid_vanilla_offset = 0
valid_vanilla_portion = float(valid_vanilla) / float(total)
patch = plt.gca().add_patch(Rectangle((valid_vanilla_offset, 0.4), valid_vanilla_portion, 0.2,
                      alpha=1, facecolor=level_2_colors[0]))

valid_both_offset = valid_vanilla_offset + valid_vanilla_portion
valid_both_portion = float(valid_both) / float(total)
patch = plt.gca().add_patch(Rectangle((valid_both_offset, 0.4), valid_both_portion, 0.2,
                      alpha=1, facecolor=level_2_colors[1]))

valid_extended_offset = valid_both_offset + valid_both_portion
valid_extended_portion = float(valid_extended) / float(total)
patch = plt.gca().add_patch(Rectangle((valid_extended_offset, 0.4), valid_extended_portion, 0.2,
                      alpha=1, facecolor=level_2_colors[2]))


invalid_error_vanilla_offset = valid_extended_offset + valid_extended_portion
invalid_error_vanilla_portion = float(invalid_error_vanilla) / float(total)
patch = plt.gca().add_patch(Rectangle((invalid_error_vanilla_offset, 0.4), invalid_error_vanilla_portion, 0.2,
                      alpha=1, facecolor=level_2_colors[0]))

invalid_error_both_offset = invalid_error_vanilla_offset + invalid_error_vanilla_portion
invalid_error_both_portion = float(invalid_error_both) / float(total)
patch = plt.gca().add_patch(Rectangle((invalid_error_both_offset, 0.4), invalid_error_both_portion, 0.2,
                      alpha=1, facecolor=level_2_colors[1]))

invalid_error_extended_offset = invalid_error_both_offset + invalid_error_both_portion
invalid_error_extended_portion = float(invalid_error_extended) / float(total)
patch = plt.gca().add_patch(Rectangle((invalid_error_extended_offset, 0.4), invalid_error_extended_portion, 0.2,
                      alpha=1, facecolor=level_2_colors[2]))


invalid_dr_vanilla_offset = invalid_error_extended_offset + invalid_error_extended_portion
invalid_dr_vanilla_portion = float(invalid_dr_vanilla) / float(total)
patch = plt.gca().add_patch(Rectangle((invalid_dr_vanilla_offset, 0.4), invalid_dr_vanilla_portion, 0.2,
                      alpha=1, facecolor=level_2_colors[0]))

invalid_dr_both_offset = invalid_dr_vanilla_offset + invalid_dr_vanilla_portion
invalid_dr_both_portion = float(invalid_dr_both) / float(total)
patch = plt.gca().add_patch(Rectangle((invalid_dr_both_offset, 0.4), invalid_dr_both_portion, 0.2,
                      alpha=1, facecolor=level_2_colors[1]))

invalid_dr_extended_offset = invalid_dr_both_offset + invalid_dr_both_portion
invalid_dr_extended_portion = float(invalid_dr_extended) / float(total)
patch = plt.gca().add_patch(Rectangle((invalid_dr_extended_offset, 0.4), invalid_dr_extended_portion, 0.2,
                      alpha=1, facecolor=level_2_colors[2]))






# remove lines between patches of different levels if requested
#level_0_patches[0].set_linewidth(0)
#level_0_patches[1].set_linewidth(0)
#level_1_patches[0].set_linewidth(0)

#ax.pie(vals.flatten(), radius=1-(2*size), colors=inner_colors,
#       wedgeprops=dict(width=size, edgecolor='w'))

# define level_0_legend
#valid_patch = mpatches.Patch(color=level_0_colors[0], label='valid')
#valid_patch.set_hatch("++")
#valid_patch.set_edgecolor("k")
#invalid_patch = mpatches.Patch(color=level_0_colors[1], label='invalid')
#invalid_patch.set_hatch("\\\\\\")
#invalid_patch.set_edgecolor("k")
#level_0_legend = plt.legend(handles=[valid_patch, invalid_patch], loc=1)

# define level 1 legend
#error_patch = mpatches.Patch(color=level_1_colors[1], label='error')
#error_patch.set_hatch("OO")
#error_patch.set_edgecolor("k")
#dr_patch = mpatches.Patch(color=level_1_colors[2], label='data race')
#dr_patch.set_hatch("////")
#dr_patch.set_edgecolor("k")
#level_1_legend = plt.legend(handles=[error_patch, dr_patch], loc=4)

# define level 2 legend
#vanilla_patch = mpatches.Patch(color="lightgrey", label='vanilla')
#both_patch = mpatches.Patch(color="grey", label='both versions')
#extended_patch = mpatches.Patch(color="black", label='extended')
#level_2_legend = plt.legend(handles=[vanilla_patch, both_patch, extended_patch], loc=2)

# register legends
#plt.gca().add_artist(level_0_legend)
#plt.gca().add_artist(level_1_legend)

ax.set(title='Suggestions')
plt.show()