import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


def add_bar_size(ax):
    for p in ax.patches:
        ax.annotate(f"{p.get_height():.0f}", (p.get_x() * 1.005, p.get_height() * 1.005))


# INPUT
comparison_type = ["Light comparison", "Heavy comparison"]
names_init = np.array(["std::sort", "pdqsort", "std::stable_sort", "spinsort", "flat_stable_sort", "spreadsort", "timsort", "skasort", "fm_sort\noptimization"])
names_rev = names_init[::-1]
averages_init = [174.19, 30.06, 104.68, 20.04, 152.71, 151.56, 86.94, 59.96, 70.32, 50.75, 61.38, 17.42, 77.88, 67.69, 42.41,
                 27.85, 27.55, 10.08]
averages_rev = averages_init[::-1]
range_limit = 1000
graph_title = 'Array size = 2.98 GB\nArray length = 781250\nObject size = 4 K bytes = vector<uint64_t>[512]'
y_label_name = f'Running time - normalized to range[0, {range_limit}]'

names2 = []
for i in names_rev:
    names2.append(i)
    names2.append(i)

v1_light_init = np.array(averages_rev[::2])
v2_heavy_init = np.array(averages_rev[1::2])
v1_light_norm = v1_light_init * range_limit / v1_light_init.max()
v2_heavy_norm = v2_heavy_init * range_limit / v2_heavy_init.max()

p1 = pd.DataFrame([names_rev, v1_light_norm]).T
p2 = pd.DataFrame([names_rev, v2_heavy_norm]).T
p3 = pd.DataFrame([names2, averages_rev, len(names_rev) * ["Light comparison", "Heavy comparison"]],
                  index=["Sorting technique", "Average execution time", "Data comparison type"]).T
p3["Average execution time"] = p3["Average execution time"] * range_limit / p3["Average execution time"].max()

##########################################################################
# GRAPH type 1
x_pos = range(len(names_rev))
plt.bar(x_pos, v1_light_norm)
plt.xticks(x_pos, names_rev)
plt.yticks(range(0, range_limit + 1, range_limit // 10), [str(i) for i in range(0, range_limit + 1, range_limit // 10)])
plt.ylabel(y_label_name)
plt.title(graph_title)
plt.show()


##########################################################################
# GRAPH type 2
def change_width(ax, new_value):
    for patch in ax.patches:
        current_width = patch.get_width()
        diff = current_width - new_value

        # we change the bar width
        patch.set_width(new_value)

        # we recenter the bar
        patch.set_x(patch.get_x() + diff * .5)


# https://seaborn.pydata.org/generated/seaborn.barplot.html
sns.set(style="whitegrid")
ax = sns.barplot(x="Data comparison type", y="Average execution time", hue="Sorting technique", data=p3,
                 palette=sns.color_palette())
# ax.axhline(0, color="k", clip_on=False)

add_bar_size(ax)
change_width(ax, 0.15)

plt.yticks(range(0, range_limit + 1, range_limit // 10), [str(i) for i in range(0, range_limit + 1, range_limit // 10)])
plt.ylabel(y_label_name)
plt.xlabel("")
plt.title(graph_title)
# plt.tight_layout(h_pad=2)
plt.show()
# plt.savefig("Figure_1.png", bbox_inches='tight', dpi=600)

##########################################################################
# GRAPH type 3 - BEST
# import matplotlib.pyplot as plt
# import matplotlib.cm as mplcm
# import matplotlib.colors as colors
# NUM_COLORS = len(names_rev)
# cm = plt.get_cmap('gist_rainbow')
# cNorm  = colors.Normalize(vmin=0, vmax=NUM_COLORS-1)
# scalarMap = mplcm.ScalarMappable(norm=cNorm, cmap=cm)
# # ax.set_color_cycle([scalarMap.to_rgba(i) for i in range(NUM_COLORS)])
# plt.colorbar(scalarMap)

df3 = pd.DataFrame([v1_light_norm, v2_heavy_norm], index=comparison_type, columns=names_rev)
index = np.arange(len(df3))
bar_width = 0.095
opacity = 1.0

fig, ax = plt.subplots()

for i in range(len(df3.columns)):
    ax.bar(index + i * bar_width, df3.values[:, i], bar_width, alpha=opacity, label=df3.columns[i])

add_bar_size(ax)

ax.set_xticks(index + len(df3.columns)*bar_width/2 - bar_width/2)
ax.set_xticklabels(comparison_type)
ax.set_xlabel("")
ax.set_ylabel(y_label_name)
ax.set_title(graph_title)
ax.legend()

current_palette = sns.color_palette()
# sns.palplot(current_palette)
plt.legend(current_palette)

fig.tight_layout()
plt.show()

##########################################################################

# REFER: https://seaborn.pydata.org/tutorial/color_palettes.html
# REFER: https://seaborn.pydata.org/examples/color_palettes.html
# default_color_palette = sns.color_palette()
# cool_warm_tuple = sns.color_palette("coolwarm", 17)
# sns.palplot(cool_warm_tuple)
# plt.show()
