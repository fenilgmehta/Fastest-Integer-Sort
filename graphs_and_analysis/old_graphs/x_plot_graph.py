import math
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
from collections import namedtuple

# int8_t
# a = np.array([275 ,4649    ,14572   ,98227   ,1055457 ,11270786, 96196725])           # only +ve numbers
# b = np.array([541 , 11042   ,154503  ,1708525 ,21876474    ,279309306, 2844986238])   # only +ve numbers
a = np.array([269 ,3836    ,13945   ,90872   ,933410  ,14385985])
b = np.array([517 ,11898   ,148601  ,1587447 ,19650477    ,310950246])
str_title = 'Compare ir_sort and std::sort\nfor int8_t / char'


# int16_t
# a = np.array([393 ,7613    ,33050   ,276984  ,2112954 ,11793120])         # only +ve numbers
# b = np.array([815 ,13249   ,163954  ,2031294 ,25689232    ,277444139])    # only +ve numbers
# a = np.array([279 ,6343    ,30626   ,271267  ,2292243 ,18349168])
# b = np.array([547 ,10904   ,156395  ,1997349 ,27713607    ,368732364])
# str_title = 'Compare ir_sort and std::sort\nfor int16_t'

# int32_t
# a = np.array([348 ,7475    ,51502   ,407814  ,4367830 ,43633473])
# b = np.array([733 ,13102   ,159645  ,1947570 ,24938572    ,294587143])
# str_title = 'Compare ir_sort and std::sort\nfor int32_t'

# int64_t
# a = np.array([306,6250,   44412,  383460, 4942495,48869952    ,459929464])
# b = np.array([561 ,10350   ,147551  ,1910381 ,28341456    ,323027561   ,3375677528])
# str_title = 'Compare ir_sort and std::sort\nfor int64_t'

n_groups = len(a)
xLabels = [];
for i in range(n_groups): xLabels.append(str(math.pow(10,i+1)// 1))
for i in range(n_groups): xLabels[i] = xLabels[i][:-2]
# for i in range(n_groups): xLabels.append('10^' + str(i+1))        # this will show the arrayLength in power, i.e "10^6"
xLabels = tuple(xLabels)

ratioTerm = a;
max_percentage = 1      # write 100 to get the result in terms of 100
ir_sort_time = (a*max_percentage) / ratioTerm
STL_sort_time= (b*max_percentage) / ratioTerm
# ir_sort_time = a
# STL_sort_time= b

fig, ax = plt.subplots()

index = np.arange(n_groups)
bar_width = 0.1
opacity = 1.0

rects1 = ax.bar(index, ir_sort_time, bar_width,
                alpha=opacity, color='b',
                label='ir_sort')

rects2 = ax.bar(index + bar_width, STL_sort_time, bar_width,
                alpha=opacity, color='r',
                label='std::sort')

ax.set_xlabel('Array length')
# ax.set_ylabel('Time (nano-seconds)')
# ax.set_ylabel('Ratio of time with std::sort')
ax.set_ylabel('Ratio of time with ir_sort')
ax.set_title(str_title)
ax.set_xticks(index + bar_width / 2)
ax.set_xticklabels(xLabels)
ax.legend()

fig.tight_layout()
plt.show()
