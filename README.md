# Fastest-Integer-Sort
Algorithm written in C++ which is highly optimised for the sorting of arrays and vectors of large objects.


Graph
----------------------------------
![Running time comparision of fm_sort optimization with other sorting algorithms](https://raw.githubusercontent.com/fenilgmehta/Fastest-Integer-Sort/develop/graphs_and_analysis/Graphs_v3/Figure_1.png)


Stats
----------------------------------
* Results for ```Array length = 781250```, ```Object size = 4 K bytes = vector<uint64_t>[512]```, ```Array size = 2.98 GB```:
    - Heavy comparison: The comparison is the sum of all the numbers of the array
    - Light comparison: The comparison is with the first element of the array, as a key

|  Sorting technique   | Heavy Comparison time | Light Comparison time |
|:--------------------:|:---------------------:|:---------------------:|
| fm_sort optimization |     x                 |      y                |
| C++ STL std::sort    | 6.32x                 |  2.98y                |
| std::stable_sort     | 5.54x                 | 15.03y                |
| pdqsort              | 3.80x                 |  1.99y                |
| spinsort             | 3.15x                 |  5.95y                |
| timsort              | 2.83x                 |  6.72y                |
| flat_stable_sort     | 2.55x                 |  5.03y                |
| spreadsort           | 2.23x                 |  1.73y                |
| skasort              | 1.53x                 |  2.77y                |


* Test conditions:
    1. Compiled using "g++ -std=c++17 -O2 -m64 -march=native" for testing
    2. timsort is used to sort the array/vector of objects based on index
    3. Arrays used to test the performance
        - random
        - sorted
        - sorted + 0.1% end
        - sorted +   1% end
        - sorted +  10% end
        - sorted + 0.1% mid
        - sorted +   1% mid
        - sorted +  10% mid
        - rev sorted
        - rev sorted + 0.1% end
        - rev sorted +   1% end
        - rev sorted +  10% end
        - rev sorted + 0.1% mid
        - rev sorted +   1% mid
        - rev sorted +  10% mid

Details
----------------------------------
1. "fm_sort::fm_sort_objects" is the name of the function to sort an array based on index
2. Three parameters:
    - first : iterator pointing to the first element of the object array
    - last  : iterator pointing the element one beyond the last element
    - p_arr_index: Auxiliary array/vector with the correct index of the elements of the range [first, last)
3. Sort the input in ascending order and descending order.
4. Can be called on all containers with RandomAccessIterator
5. Time complexity depends on the sorting algorithm used to sort the index array
6. Auxiliary space complexity is O(n) - a vector of type uint16_t/uint32_t/uint64_t of size **n** is required which can be negligible as compared to the size of the array/vector to be sorted and the performance improvement.

Features
----------------------------------
1. faster than any other sorting algorithm for large object arrays
2. stable/unstable sorting based on the sorting technique used to sort the index array
3. takes the advantage of large size of the objects
4. very useful in places where in-memory sorting is to be performed on large objects
5. valid but not limited to the following C++ Standard Template Library(STL) containers for sorting:
    * std::vector
    * std::array
    * std::valarray


Basic working
----------------------------------
* Working of swapping optimization
    - Once fm_sort get the correct positions of the objects in the index array, the original array is sorted based on the index array in just **n** swaps
* Working of the ir_sort
    - This algorithm takes the advantage of bitwise operations provided by the C++ language on top of processors which run at a very high speed as compared to the other operations like addition, subtraction, multiplication, division, etc. Along with this, it uses radix sort to make the sorting faster.
    - I have combined the above mentioned things into one algorithm which would sort the input given to it.


Usage
----------------------------------
* To use this sorting optimization in your project, follow the below mentioned steps - 
```c++
// copy the following four files to the project folder: "ir_commons.hpp", "integer_sort.hpp" and "integer_sort_objects_small.hpp"

// paste the following lines in the file start
#include "integer_sort.hpp"
#include "integer_sort_objects_small.hpp"

// assume "arr1" is the vector of objects(type T) to be sorted
// assume "compare_function" is a function which is used to compare elements(type T) of the vector "arr1"

using IndexType = uint64_t;
vector<IndexType> p_arr_index(arr1.size());
iota(p_arr_index.begin(), p_arr_index.end(), 0);

auto arr_first = arr1.begin();
std::sort(p_arr_index.begin(), p_arr_index.end(), [arr_first](const IndexType &a, const IndexType &b) { return compare_function(arr_first[a], arr_first[b]); });

fm_sort::fm_sort_objects(arr1.begin(), arr1.end(), begin(p_arr_index));

```


* To use the radix sort in competitions, follow the below mentioned steps:
```c++
// copy the namespace "ir_sort" from "ir_sort_competitions.cpp" to the main ".cpp" program file

// to call the function, from the namespace "ir_sort"
// the parameters are same as std::sort
// let v be define as 'vector<int> v{6,4,3,5,9,7,2,1,8,0}'
// eg: ir_sort::integer_sort(v.begin(), v.end()); where "v" is a vector of integers, result is in ascending order
// eg: ir_sort::integer_sort(v.rbegin(), v.rend()); where "v" is a vector of integers, result is in descending order
ir_sort::integer_sort(begin, end)
```


