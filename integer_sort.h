#ifndef PROJECT_SORTING_MYSORT_H
#define PROJECT_SORTING_MYSORT_H

#include <cstdint>
#include <algorithm>
#include <type_traits>

//*****************************************************************************************************************************************
int64_t INSERTION_SORT_THRESHOLD = 30; // 25
int64_t MERGE_SORT_THRESHOLD = 29;     // 135

// 0 to 1360
const int64_t THRESHOLD_INSERTION_FOR_MERGE[] = {0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 9, 10, 10, 12, 11, 14, 15, 13, 13, 16, 14, 15, 15, 19, 21, 19, 20, 25, 21, 22,
                                                 22, 23, 23, 23, 33, 23, 24, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27,
                                                 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 17, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20,
                                                 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24, 25, 25, 25, 25, 26, 26, 26, 26, 27, 27,
                                                 27, 27, 28, 28, 28, 28, 29, 29, 29, 29, 30, 30, 30, 30, 31, 31, 31, 31, 32, 32, 32, 32, 33, 33, 33, 33, 34, 34,
                                                 34, 34, 35, 35, 35, 35, 36, 36, 36, 36, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20,
                                                 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23,
                                                 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27,
                                                 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30,
                                                 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34,
                                                 34, 34, 34, 34, 34, 34, 35, 35, 35, 35, 35, 35, 35, 35, 36, 36, 36, 36, 36, 36, 36, 36, 37, 37, 37, 37, 18, 18,
                                                 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20,
                                                 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
                                                 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
                                                 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25,
                                                 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27,
                                                 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
                                                 28, 28, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                                                 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32,
                                                 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34,
                                                 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
                                                 35, 35, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
                                                 37, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19,
                                                 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20,
                                                 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
                                                 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
                                                 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
                                                 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
                                                 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
                                                 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
                                                 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26,
                                                 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27,
                                                 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
                                                 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
                                                 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
                                                 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                                                 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
                                                 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
                                                 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33,
                                                 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34,
                                                 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
                                                 34, 34, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
                                                 35, 35, 35, 35, 35, 35, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
                                                 36, 17, 17, 36, 36, 36, 36, 36, 36, 36, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
                                                 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
                                                 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
                                                 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
                                                 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20,
                                                 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
template<typename T>
inline int_fast16_t countBits_positive(T num) {
    int_fast16_t result = 0;

    while (num > 0) {
        num >>= 1;
        result += 1;
    }

    return result;
}

template<typename T>
inline int_fast16_t countBits_negative(const T &num) {
    auto number = static_cast<uint_fast64_t>(-num);
    int_fast16_t result = 0;

    while (number > 0) {
        number >>= 1;
        result += 1;
    }

    return result;
}

template<typename T>
inline int_fast16_t countBits(const T &num) {
    if (num < 0) return countBits_negative(num);
    return countBits_positive(num);
}

inline void set_BitShift_Iterations(const int_fast16_t &arrMax_bits, int_fast16_t &bitsShift, int_fast16_t &iterationsRequired) {
    if (arrMax_bits <= 8) {           //  8 bits = 1 iteration
        bitsShift = arrMax_bits;
        iterationsRequired = 1;
    } else if (arrMax_bits <= 10) {
        bitsShift = 5;
        iterationsRequired = 2;
    } else if (arrMax_bits <= 12) {
        bitsShift = 6;
        iterationsRequired = 2;
    } else if (arrMax_bits <= 14) {
        bitsShift = 7;
        iterationsRequired = 2;
    } else if (arrMax_bits <= 16) {   // 16 bits = 2 iterations
        bitsShift = 8;
        iterationsRequired = 2;
    } else if (arrMax_bits <= 18) {
        bitsShift = 6;
        iterationsRequired = 3;
    } else if (arrMax_bits <= 21) {
        bitsShift = 7;
        iterationsRequired = 3;
    } else if (arrMax_bits <= 24) {   // 24 bits = 3 iterations
        bitsShift = 8;
        iterationsRequired = 3;
    } else if (arrMax_bits <= 28) {
        bitsShift = 7;
        iterationsRequired = 4;
    } else if (arrMax_bits <= 32) {   // 32 bits = 4 iterations
        bitsShift = 8;
        iterationsRequired = 4;
    } else if (arrMax_bits <= 35) {
        bitsShift = 7;
        iterationsRequired = 5;
    } else if (arrMax_bits <= 40) {   // 40 bits = 5 iterations
        bitsShift = 8;
        iterationsRequired = 5;
    } else if (arrMax_bits <= 42) {
        bitsShift = 7;
        iterationsRequired = 6;
    } else if (arrMax_bits <= 48) {   // 48 bits = 6 iterations
        bitsShift = 8;
        iterationsRequired = 6;
    } else if (arrMax_bits == 49) {
        bitsShift = 7;
        iterationsRequired = 7;
    } else if (arrMax_bits <= 56) {   // 56 bits = 7 iterations
        bitsShift = 8;
        iterationsRequired = 7;
    } else {                                // 64 bits = 8 iterations
        bitsShift = 8;
        iterationsRequired = 8;
    }
}

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
// template<typename T>
// inline void
// ir_sort(T &arr, const int_fast64_t &low, const int_fast64_t &high, const bool &ascendingOrder = true, int_fast16_t forceLinearSort = 0);
//
// template<typename RandomAccessIterator>
// inline void
// ir_sort(RandomAccessIterator start, RandomAccessIterator end, const bool &ascendingOrder, int_fast16_t forceLinearSort);

//*****************************************************************************************************************************************
#endif //PROJECT_SORTING_MYSORT_H

