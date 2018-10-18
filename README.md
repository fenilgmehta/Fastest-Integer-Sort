# Fastest-Integer-Sort
These are generalised functions written in C++ which are highly optimised for the sorting of integer based array or vectors. It is 6 times faster than C++ STL std::sort for "int64_t array[10000000]".


Details
----------------------------------
1. "ir_sort" is the name of the function to sort the integer based arrays.
2. Five parameters: array, startIndex(integer), endIndex(integer), ascendingOrder(boolean)(optional), forceLinearSort(integer)(optional)
3. Alternative parameters: first two parameters same as std::sort, ascendingOrder(boolean)(optional), forceLinearSort(integer)(optional)
4. Sorts the input in ascending order and descending order.


Features
----------------------------------
1. faster than any other sorting algorithm for integer arrays
2. stable sorting
3. takes the advantage of binary operations over decimal operations
4. linear time complexity
5. very useful in competitions(refer "ir_sort_competitions.cpp" for usage in competitions)
6. added new function for sorting object arrays based on integer keys


Time complexity
----------------------------------
| Case         | Time complexity |
|:------------:|:---------------:|
| best case    | Ω(n)            |
| average case | θ(n)            |
| worst case   | O(n)            |


Graph
----------------------------------
![Project Summary](https://raw.githubusercontent.com/fenilgmehta/Fastest-Integer-Sort/master/x_graph_int64.png)
![Project Summary](https://raw.githubusercontent.com/fenilgmehta/Fastest-Integer-Sort/master/x_graph_int32.png)
![Project Summary](https://raw.githubusercontent.com/fenilgmehta/Fastest-Integer-Sort/master/x_graph_int16.png)
![Project Summary](https://raw.githubusercontent.com/fenilgmehta/Fastest-Integer-Sort/master/x_graph_int8_OR_char.png)


Usage
----------------------------------
For projects:
```
// copy the following three files to the project folder: "basic_sorts.h", "integer_sort.h" and "integer_sort.cpp"

// paste the following lines in the file start
include "integer_sort.cpp"
```

For competitions:
```
// copy the namespace "integer_sort" from "ir_sort_competitions.cpp" to the main ".cpp" program file

// to call the function, from the namespace "integer_sort_asc"
// the parameters are same as std::sort
// eg: integer_sort_asc::ir_sort(v.begin(), v.end()); where "v" is a vector of integers
integer_sort_asc::ir_sort(begin, end)
```


TODO
----------------------------------
1. write the function to sort the array of objects based on integer key in ascending(-ve numbers also) and descending order(both +ve and -ve numbers)
