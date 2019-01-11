# from matplotlib.ticker import MaxNLocator
# from collections import namedtuple

# import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os, re, math
print("### import complete ###\n")

#################################################################################
# Generated data for graph generation
temp_files_list = os.listdir()
files_list = []

data_type_name = input("Enter the name of the datatype to be printed as heading (eg. int64_t): ")

for i in temp_files_list:
    if re.search(r"^rawData.*\.csv", i) : files_list.append(i)

files_list.sort()
sort_label = list(pd.read_csv(files_list[0]).mean().index)
xLabels = []
starting_value_of_ten = 100 # MODIFY as per files being scanned
for i in range(len(files_list)): xLabels.append(str(math.pow(10,i+math.log10(starting_value_of_ten)) // 1)[:-2]) # for i in range(n_groups): xLabels.append('10^' + str(i+1)) # this will show the arrayLength in power, i.e "10^6"
# final_result = pd.DataFrame(index=np.array(sort_label))
# final_result = pd.DataFrame()
final_result = []
final_df = []
for i in files_list:
    raw_data = pd.read_csv(i)
    raw_data_mean = raw_data.mean()
    final_result.append(list(raw_data_mean))
    final_df.append(raw_data_mean)
    # print(raw_data_mean)

final_result = pd.DataFrame(final_result)
final_df = pd.DataFrame(final_df)
print(final_df)

print("\nFILES_LIST :",files_list)
print("\nINDEX :",sort_label)
print("\nAVERAGE :\n", final_result)
print(final_result[0])

# exit(0)
#################################################################################
# Generate the graph

ratioTerm = np.array(final_result[0])
max_percentage = 1      # write 100 to get the result in terms of 100
# ir_sort_time = (a*max_percentage) / ratioTerm
# STL_sort_time= (b*max_percentage) / ratioTerm
# ir_sort_time = a
# STL_sort_time= b
final_time_ratio = []
for i in range(len(final_result)):
    final_time_ratio.append(np.sqrt((final_result[i] * max_percentage) / ratioTerm))

fig, ax = plt.subplots()

index = np.arange(len(final_result))
bar_width = 0.1
opacity = 1.0

# rects1 = ax.bar(index, ir_sort_time, bar_width, alpha=opacity, color='b', label='ir_sort')
# rects2 = ax.bar(index + bar_width, STL_sort_time, bar_width, alpha=opacity, color='r', label='std::sort')
# ax.bar(index, final_time_ratio[0], bar_width, alpha=opacity, color='b', label=sort_label[0])
for i in range(len(final_result)):
    ax.bar(index + i*bar_width, final_time_ratio[i], bar_width, alpha=opacity, label=sort_label[i])

ax.set_xlabel('Array length')
# ax.set_ylabel('Time (nano-seconds)')
# ax.set_ylabel('Ratio of time with std::sort')
ax.set_ylabel('Ratio of time with ir_sort')
ax.set_title('Compare ir_sort with other sorting algorithms, array of '+data_type_name)
ax.set_xticks(index + bar_width / 2)
ax.set_xticklabels(xLabels)
ax.legend()

fig.tight_layout()
plt.show()

#################################################################################
# Print final_time_ratio
# for i in final_time_ratio: print('\n'+str(i))