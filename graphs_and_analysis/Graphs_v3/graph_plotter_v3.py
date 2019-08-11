import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# INPUT
names = np.array(["std::sort", "pdqsort", "std::stable_sort", "spinsort", "flat_stable_sort", "spreadsort", "timsort", "skasort", "fm_sort\noptimization"])[::-1]
Averages = [174.19, 30.06, 104.68, 20.04, 152.71, 151.56, 86.94, 59.96, 70.32, 50.75, 61.38, 17.42, 77.88, 67.69, 42.41, 27.85, 27.55, 10.08][::-1]
range_limit = 1000
graph_title = 'Array size = 2.98 GB\nArray length = 781250\nObject size = 4 K bytes = vector<uint64_t>[512]'
y_label_name = f'Running time - normalized to range[0, {range_limit}]'

names2 = []
for i in names:
    names2.append(i)
    names2.append(i)

v1 = np.array(Averages[::2])
v2 = np.array(Averages[1::2])
v1 = v1 * range_limit / v1.max()
v2 = v2 * range_limit / v2.max()

p1 = pd.DataFrame([names, v1]).T
p2 = pd.DataFrame([names, v2]).T
p3 = pd.DataFrame([names2, Averages, len(names)*["Light comparison","Heavy comparison"]], index=["Sorting technique", "Average execution time" ,"Data comparision type"]).T
p3["Average execution time"] = p3["Average execution time"] * range_limit / p3["Average execution time"].max()

# GRAPH type 1
x_pos = range(len(names))
plt.bar(x_pos, v1)
plt.xticks(x_pos, names)
plt.yticks(range(0, range_limit+1, range_limit//10), [str(i) for i in range(0, range_limit+1, range_limit//10)])
plt.ylabel(y_label_name)
plt.title(graph_title)
plt.show()

# GRAPH type 2
sns.set(style="whitegrid")
ax = sns.barplot(x="Sorting technique", y="Average execution time", hue="Data comparision type", data=p3, palette=sns.color_palette("coolwarm", 17)[0::16] )
# ax.axhline(0, color="k", clip_on=False)

for p in ax.patches:
    ax.annotate(f"{p.get_height():.0f}", (p.get_x() * 1.005, p.get_height() * 1.005))

# for ticks in ax.xaxis.get_major_ticks():
#     if ticks.label1.get_text() == names[-1]:
#         # ticks.label1.set_facecolor("red")
#         # ax.patches[p3.index.get_indexer([ticks.label1.get_text()])[0]].set_facecolor('red')
#         ax.patches[p3.index.get_indexer([ticks.label1.get_text()])[0]].set_facecolor('red')

plt.yticks(range(0, range_limit+1, range_limit//10), [str(i) for i in range(0, range_limit + 1, range_limit//10)])
plt.ylabel(y_label_name)
plt.title(graph_title)
plt.tight_layout(h_pad=2)
plt.show()


# REFER: https://seaborn.pydata.org/tutorial/color_palettes.html
# REFER: https://seaborn.pydata.org/examples/color_palettes.html
# default_color_palette = sns.color_palette()
# cool_warm_tuple = sns.color_palette("coolwarm", 17)
# sns.palplot(cool_warm_tuple)
# plt.show()
