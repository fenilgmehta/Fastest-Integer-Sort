# Fastest-Integer-Sort
These are generalised functions written in C++ which are highly optimised for the sorting of integer based array or vectors. It is 4 times faster than C++ STL std::sort for "int64_t array[10000000]".


Details
----------------------------------
1. "ir_sort::integer_sort" is the name of the function to sort the integer based arrays.
2. Four parameters: first two parameters same as std::sort, ascendingOrder(boolean)(optional), forceLinearSort(integer)(optional)
3. Sort the input in ascending order and descending order.
4. Compiled using "g++ -std=c++14 -m64 -march=native" for testing
5. Can be called on all containers with RandomAccessIterator
6. Requirements:
    * [first, last) is a valid range.
    * RandomAccessIterator value_type is mutable.
    * RandomAccessIterator value_type is LessThanComparable.
    * RandomAccessIterator value_type supports the operator>>, which returns an integer-type right-shifted a specified number of bits.
    * RandomAccessIterator supports the operator[], which returns the i'th element of the container.


Features
----------------------------------
1. faster than any other sorting algorithm for integer arrays
2. stable sorting
3. takes the advantage of binary operations over decimal operations
4. linear time complexity
5. very useful in competitions(refer "ir_sort_competitions.cpp" for usage in competitions)
6. added new function for sorting object arrays based on integer keys
7. valid but not limited to the following C++ Standard Template Library(STL) containers for sorting:
    * std::array
    * std::valarray
    * std::vector
    * std::deque


Time complexity
----------------------------------
| Case         | Time complexity |
|:------------:|:---------------:|
| best case    | Ω(n)            |
| average case | θ(n)            |
| worst case   | O(n)            |


Graph
----------------------------------
![Speed Comparison, int64_t](https://raw.githubusercontent.com/fenilgmehta/Fastest-Integer-Sort/master/graphs_and_analysis/all_comparisons/rawData_int64_t/Figure_1_int64_t.png)
![Speed Comparison, int32_t](https://raw.githubusercontent.com/fenilgmehta/Fastest-Integer-Sort/master/graphs_and_analysis/all_comparisons/rawData_int32_t/Figure_1_int32_t.png)
![Speed Comparison, int16_t](https://raw.githubusercontent.com/fenilgmehta/Fastest-Integer-Sort/master/graphs_and_analysis/all_comparisons/rawData_int16_t/Figure_1_int16_t.png)
![Speed Comparison, int8_t](https://raw.githubusercontent.com/fenilgmehta/Fastest-Integer-Sort/master/graphs_and_analysis/all_comparisons/rawData_int8_t/Figure_1_int8_t.png)


Basic working
----------------------------------
It is a stable sorting algorithm.
This algorithm takes the advantage of bitwise operations provided by the C++ language on top of processors which run at a very high speed as compared to the other operations like addition, subtraction, multiplication, division, etc. Along with this, it uses radix sort to make the sorting faster.
I have combined the above mentioned things into one algorithm which would sort the input given to it. To improve the performance for small size of input arrays, I have used insertion sort and merge sort.
The graphs above shows how ir_sort::integer_sort starts performing better with higher array size.


Usage
----------------------------------
For projects:
```
// copy the following four files to the project folder: "basic_sorts.hpp", "ir_commons.hpp", "integer_sort.cpp" and "integer_sort_objects_small.cpp"

// paste the following lines in the file start
include "integer_sort.cpp"
```

For competitions:
```
// copy the namespace "ir_sort" from "ir_sort_competitions.cpp" to the main ".cpp" program file

// to call the function, from the namespace "ir_sort"
// the parameters are same as std::sort
// let v be define as 'vector<int> v{6,4,3,5,9,7,2,1,8,0}'
// eg: ir_sort::integer_sort(v.begin(), v.end()); where "v" is a vector of integers, result is in ascending order
// eg: ir_sort::integer_sort(v.rbegin(), v.rend()); where "v" is a vector of integers, result is in descending order
ir_sort::integer_sort(begin, end)
```


TODO
----------------------------------
1. Write the sorting function to sort the array of objects based on integer key without creating copy of the objects.
    * ascending order for -ve number only
    * descending order for +ve and -ve numbers
    * this will be useful where the objects are large in size.
