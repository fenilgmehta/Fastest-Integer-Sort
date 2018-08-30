#include "integer_sort.h"
#include "basic_sorts.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
// Sorts the +ve numbers in ascending order
template<typename ArrayElementType, typename T>
inline void
radixSort_Positive_asc(T &arr, const int_fast64_t &low, int_fast64_t high, const int_fast64_t &sortArrayLength, const int_fast16_t &bits_arrMaxElement) {
    if (low >= high) return;

    // using ArrayElementType = std::remove_reference_t<std::remove_const_t<decltype(arr[0])>>;     // this should be used if the template parameter is only "T"
    // using ArrayElementType = typename std::remove_all_extents<decltype(arr)>::type;              // has NOT been tested

    // SUBORDINATE VARIABLES
    // ArrayElementType copyArray[sortArrayLength];                         // this "vector" is used to store the partially sorted array        // stack memory overflow
    // std::vector<ArrayElementType> copyArray(sortArrayLength);            // this "vector" is used to store the partially sorted array        // SLOW
    auto *copyArray = new ArrayElementType[sortArrayLength];    // this dynamic array is used to store the partially sorted array   // FAST and reliable
    ArrayElementType *p_arr = &arr[low];
    ArrayElementType *p_copyArray = &copyArray[0];
    high -= low;

    // REQUIRED VARIABLES
    int_fast64_t i;                     // IMPORTANT: used for all iterations
    int_fast16_t bitsShift;             // IMPORTANT: bits to be considered for sorting
    int_fast16_t iterationsRequired;    // IMPORTANT: number of passes for the given array

    set_BitShift_Iterations(bits_arrMaxElement, bitsShift, iterationsRequired);

    // this is used to access the n'th column of the element
    int_fast16_t bitsShiftFactor = 0;
    // this variable store the size of the array "box"
    const int_fast32_t boxArraySize = static_cast<int_fast32_t>(1) << (bitsShift);
    // (arr[i] & modNum) = (arr[i] % boxArraySize), both are same
    const int_fast32_t modNum = boxArraySize - 1;
    // this is used to store the radix for the array elements
    // the size of the array "box" is "boxArraySize" so that the numbers from 0 to "boxArraySize - 1" can be tracked in the column being scanned
    uint_fast64_t box[boxArraySize];

    // START ACTUAL SORTING
    // This condition is used so that all the digits of each element are checked and "arr" is sorted according to the digit considered
    while (iterationsRequired-- > 0) {
        // default values of all the elements of the array "box" = 0
        for (i = 0; i < boxArraySize;) {
            box[i++] = 0;
        }

        // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
        for (i = 0; i <= high; i++) {
            box[(p_arr[i] >> bitsShiftFactor) & modNum]++;
        }

        // this for loop is used to add the values of the previous element to the next element of the array "box"
        for (i = 1; i < boxArraySize; i++) {
            box[i] += box[i - 1];
        }

        // swapping is equivalent to copying all the elements of "arr" to "copyArray"
        // BEFORE swapping,
        //      "p_arr" would be storing the reference to the array sorted in the last pass
        //      "p_copyArray" would point to the old and un-updated array
        // AFTER swapping,
        //      "p_arr" would point to the old and un-updated array
        //      "p_copyArray" would be storing the reference to the array sorted in the last pass
        std::swap<ArrayElementType *>(p_arr, p_copyArray);

        // this for loop is used to insert the elements of "p_copyArray" into "p_arr" in sorted order of the column being scanned of the original array
        for (i = high; i >= 0; i--) {
            p_arr[--box[((p_copyArray[i] >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
        }

        // this operation is done so that we move to the next column of array elements
        bitsShiftFactor += bitsShift;
    }

    // the sorted arrays is copied into the original input array if the final sorted output is in "copyArray"
    if ((&arr[0] + low) != p_arr) {
        // copy(p_arr, p_arr + sortArrayLength, arr + low);
        // copy(p_arr, p_arr + sortArrayLength, p_copyArray);
        for (i = 0; i < sortArrayLength; i++) {
            p_copyArray[i] = p_arr[i];
        }
    }

    delete[] copyArray;
}

// Sorts the +ve and -ve numbers in ascending order
template<typename ArrayElementType, typename T>
inline void
radixSort_asc(T &arr, const int_fast64_t &low, int_fast64_t high, const int_fast64_t &sortArrayLength, const int_fast16_t &maxBits) {
    if (low >= high) return;

    // using ArrayElementType = std::remove_reference_t<std::remove_const_t<decltype(arr[0])>>;     // this should be used if the template parameter is only "T"
    // using ArrayElementType = typename std::remove_all_extents<decltype(arr)>::type;              // has NOT been tested

    // SUBORDINATE VARIABLES
    // ArrayElementType copyArray[sortArrayLength];                         // this "vector" is used to store the partially sorted array        // stack memory overflow
    // std::vector<ArrayElementType> copyArray(sortArrayLength);            // this "vector" is used to store the partially sorted array        // SLOW
    auto *copyArray = new ArrayElementType[sortArrayLength];    // this dynamic array is used to store the partially sorted array   // FAST and reliable
    ArrayElementType *p_arr = &arr[low];
    ArrayElementType *p_copyArray = &copyArray[0];
    high -= low;

    // REQUIRED VARIABLES
    int_fast64_t i, j;                  // IMPORTANT: used for all iterations
    int_fast16_t bitsShift;             // IMPORTANT: bits to be considered for sorting
    int_fast16_t iterationsRequired;    // IMPORTANT: number of passes for the given array

    set_BitShift_Iterations(maxBits, bitsShift, iterationsRequired);

    // this is used to access the n'th column of the element
    int_fast16_t bitsShiftFactor = 0;
    // this variable store the size of the array "box"
    const int_fast32_t boxArraySize = static_cast<int_fast32_t>(1) << (bitsShift);
    // (arr[i] & modNum) = (arr[i] % boxArraySize), both are same
    const int_fast32_t modNum = boxArraySize - 1;
    // this is used to store the radix for the array elements
    // the size of the array "box" is "boxArraySize" so that the numbers from 0 to "boxArraySize - 1" can be tracked in the column being scanned
    uint_fast64_t box[boxArraySize];

    // START ACTUAL SORTING
    // This condition is used so that all the digits of each element are checked and "arr" is sorted according to the digit considered
    while (iterationsRequired-- > 0) {
        // default values of all the elements of the array "box" = 0
        for (i = 0; i < boxArraySize;) {
            box[i++] = 0;
        }

        // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
        for (i = 0; i <= high; i++) {
            box[(p_arr[i] >> bitsShiftFactor) & modNum]++;
        }

        // this for loop is used to add the values of the previous element to the next element of the array "box"
        for (i = 1; i < boxArraySize; i++) {
            box[i] += box[i - 1];
        }

        // swapping is equivalent to copying all the elements of "arr" to "copyArray"
        // BEFORE swapping,
        //      "p_arr" would be storing the reference to the array sorted in the last pass
        //      "p_copyArray" would point to the old and un-updated array
        // AFTER swapping,
        //      "p_arr" would point to the old and un-updated array
        //      "p_copyArray" would be storing the reference to the array sorted in the last pass
        std::swap<ArrayElementType *>(p_arr, p_copyArray);

        // this for loop is used to insert the elements of "p_copyArray" into "p_arr" in sorted order of the column being scanned of the original array
        for (i = high; i >= 0; i--) {
            p_arr[--box[((p_copyArray[i] >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
        }

        // this operation is done so that we move to the next column of array elements
        bitsShiftFactor += bitsShift;
    }

    // ##### NEW START #####
    int_fast64_t binarySearch_low = 0, binarySearch_high = high, binarySearch_mid;
    while (binarySearch_low <= binarySearch_high) {
        binarySearch_mid = (binarySearch_low + binarySearch_high) >> 1;
        if (p_arr[binarySearch_mid] < 0) binarySearch_high = binarySearch_mid - 1;
        else binarySearch_low = binarySearch_mid + 1;
    }
    if (binarySearch_low + 1 <= high) while (p_arr[binarySearch_low + 1] >= 0) binarySearch_low++;
    if (binarySearch_low - 1 >= 0) while (p_arr[binarySearch_low - 1] < 0) binarySearch_low--;

    std::swap<ArrayElementType *>(p_arr, p_copyArray);
    for (i = 0, j = binarySearch_low; j <= high;) p_arr[i++] = p_copyArray[j++];
    for (j = 0; j < binarySearch_low;) p_arr[i++] = p_copyArray[j++];
    // ##### NEW END #####

/*    box[0] = sortArrayLength;
    box[1] = 0;
    for (i = 0; i <= high; i++) if (p_arr[i] < 0) box[1]++;
    for (i = high; i >= 0; i--) p_arr[--box[(p_copyArray[i] < 0)]] = p_copyArray[i];*/

    // the sorted arrays is copied into the original input array if the final sorted output is in "copyArray"
    if ((&arr[0] + low) != p_arr) {
        // copy(p_arr, p_arr + sortArrayLength, arr + low);
        // copy(p_arr, p_arr + sortArrayLength, p_copyArray);
        for (i = 0; i < sortArrayLength; i++) {
            p_copyArray[i] = p_arr[i];
        }
    }

    delete[] copyArray;
}


// Sorts the +ve numbers in ascending order
template<typename ArrayElementType, typename T>
inline void
radixSort_Positive_asc(T &arr, const int_fast64_t &low, int_fast64_t high, const int_fast64_t &sortArrayLength) {
    if (low >= high) return;

    // using ArrayElementType = std::remove_reference_t<std::remove_const_t<decltype(arr[0])>>;     // this should be used if the template paramater is only "T"
    // using ArrayElementType = typename std::remove_all_extents<decltype(arr)>::type;              // has NOT been tested

    // SUBORDINATE VARIABLES
    ArrayElementType arrMaxElement = arr[high];     // "arrMaxElement" is used to store the largest element in "arr"
    // ArrayElementType copyArray[sortArrayLength];                         // this "vector" is used to store the partially sorted array        // stack memory overflow
    // std::vector<ArrayElementType> copyArray(sortArrayLength);            // this "vector" is used to store the partially sorted array        // SLOW
    auto *copyArray = new ArrayElementType[sortArrayLength];    // this dynamic array is used to store the partially sorted array   // FAST and reliable
    ArrayElementType *p_arr = &arr[low];
    ArrayElementType *p_copyArray = &copyArray[0];
    high -= low;

    // REQUIRED VARIABLES
    int_fast64_t i;                     // IMPORTANT: used for all iterations
    int_fast16_t bitsShift;             // IMPORTANT: bits to be considered for sorting
    int_fast16_t iterationsRequired;    // IMPORTANT: number of passes for the given array

    // this for loop is used to FIND THE MAXIMUM of all the elements in the given array/RandomAccessIterator and store its value in the variable "arrMaxElement"
    for (i = 0; i < high; i++) {
        if (p_arr[i] > arrMaxElement) arrMaxElement = p_arr[i];
    }

    set_BitShift_Iterations(countBits_positive(arrMaxElement), bitsShift, iterationsRequired);

    // this is used to access the n'th column of the element
    int_fast16_t bitsShiftFactor = 0;
    // this variable store the size of the array "box"
    const int_fast32_t boxArraySize = static_cast<int_fast32_t>(1) << (bitsShift);
    // (arr[i] & modNum) = (arr[i] % boxArraySize), both are same
    const int_fast32_t modNum = boxArraySize - 1;
    // this is used to store the radix for the array elements
    // the size of the array "box" is "boxArraySize" so that the numbers from 0 to "boxArraySize - 1" can be tracked in the column being scanned
    uint_fast64_t box[boxArraySize];

    // START ACTUAL SORTING
    // This condition is used so that all the digits of each element are checked and "arr" is sorted according to the digit considered
    while (iterationsRequired-- > 0) {
        // default values of all the elements of the array "box" = 0
        for (i = 0; i < boxArraySize;) {
            box[i++] = 0;
        }

        // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
        for (i = 0; i <= high; i++) {
            box[(p_arr[i] >> bitsShiftFactor) & modNum]++;
        }

        // this for loop is used to add the values of the previous element to the next element of the array "box"
        for (i = 1; i < boxArraySize; i++) {
            box[i] += box[i - 1];
        }

        // swapping is equivalent to copying all the elements of "arr" to "copyArray"
        // BEFORE swapping,
        //      "p_arr" would be storing the reference to the array sorted in the last pass
        //      "p_copyArray" would point to the old and un-updated array
        // AFTER swapping,
        //      "p_arr" would point to the old and un-updated array
        //      "p_copyArray" would be storing the reference to the array sorted in the last pass
        std::swap<ArrayElementType *>(p_arr, p_copyArray);

        // this for loop is used to insert the elements of "p_copyArray" into "p_arr" in sorted order of the column being scanned of the original array
        for (i = high; i >= 0; i--) {
            p_arr[--box[((p_copyArray[i] >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
        }

        // this operation is done so that we move to the next column of array elements
        bitsShiftFactor += bitsShift;
    }

    // the sorted arrays is copied into the original input array if the final sorted output is in "copyArray"
    if ((&arr[0] + low) != p_arr) {
        // copy(p_arr, p_arr + sortArrayLength, arr + low);
        // copy(p_arr, p_arr + sortArrayLength, p_copyArray);
        for (i = 0; i < sortArrayLength; i++) {
            p_copyArray[i] = p_arr[i];
        }
    }

    delete[] copyArray;
}

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
// integer_radix_sort
template<typename T>
inline void ir_sort(T &arr, const int_fast64_t &low, const int_fast64_t &high, const bool &ascendingOrder = true) {
    // for loop is better than STL function fill(.....)
    // copy(....) of STL is better than for loop

    if (low >= high) return;

    using ArrayElementType = std::remove_reference_t<std::remove_const_t<decltype(arr[0])>>;
    const int_fast64_t sortArrayLength = high - low + 1;

    // "arrMaxElement" is used to store the largest element in "arr"
    // "arrMinElement" is used to store the smallest element in "arr"
    ArrayElementType arrMaxElement = arr[high], arrMinElement = arr[high];

    // this for loop is used to FIND THE MAXIMUM and MINIMUM of all the elements in the given array/RandomAccessIterator
    for (int_fast64_t i = low; i < high; i++) {
        if (arr[i] > arrMaxElement) arrMaxElement = arr[i];
        else if (arr[i] < arrMinElement) arrMinElement = arr[i];
    }

    int_fast16_t onlyPositiveNumbers = 0;
    if (arrMinElement > -1) onlyPositiveNumbers = 1;
    else if (arrMaxElement < 0) onlyPositiveNumbers = -1;

    int_fast16_t bits_arrMaxElement = countBits(arrMaxElement), bits_arrMinElement = countBits(arrMinElement);

    if (onlyPositiveNumbers != 0) {
        if (bits_arrMaxElement <= 8) MERGE_SORT_THRESHOLD = 49;
        else if (bits_arrMaxElement <= 16) MERGE_SORT_THRESHOLD = 77;
        else if (bits_arrMaxElement <= 24) MERGE_SORT_THRESHOLD = 134;
        else if (bits_arrMaxElement <= 32) MERGE_SORT_THRESHOLD = 165;
        else if (bits_arrMaxElement <= 40) MERGE_SORT_THRESHOLD = 245;
        else if (bits_arrMaxElement <= 48) MERGE_SORT_THRESHOLD = 302;
        else if (bits_arrMaxElement <= 56) MERGE_SORT_THRESHOLD = 407;
        else MERGE_SORT_THRESHOLD = 455;
    } else {
        // TODO: change the thresholds
        // if (bits_arrMaxElement <= 8) MERGE_SORT_THRESHOLD = 49;
        // else if (bits_arrMaxElement <= 16) MERGE_SORT_THRESHOLD = 77;
        // else if (bits_arrMaxElement <= 24) MERGE_SORT_THRESHOLD = 134;
        // else if (bits_arrMaxElement <= 32) MERGE_SORT_THRESHOLD = 165;
        // else if (bits_arrMaxElement <= 40) MERGE_SORT_THRESHOLD = 245;
        // else if (bits_arrMaxElement <= 48) MERGE_SORT_THRESHOLD = 302;
        // else if (bits_arrMaxElement <= 56) MERGE_SORT_THRESHOLD = 407;
        // else MERGE_SORT_THRESHOLD = 455;
    }


    if (onlyPositiveNumbers != 0) {
        // +ve numbers ONLY
        if (ascendingOrder) {
            if (sortArrayLength <= MERGE_SORT_THRESHOLD) {
                merge_sort_basic_asc<ArrayElementType, T>(arr, low, high);
                return;
            } else {
                radixSort_Positive_asc<ArrayElementType, T>(arr, low, high, sortArrayLength, bits_arrMaxElement);
                return;
            }
        } else {
            // TODO: implement the functions for descending Order Sorting
            // if (sortArrayLength <= MERGE_SORT_THRESHOLD) {
            //     merge_sort_basic_desc<ArrayElementType, T>(arr, low, high);
            //     return;
            // } else {
            //     // radixSort_Positive_desc<ArrayElementType, T>(arr, low, high, sortArrayLength, bits_arrMaxElement);
            //     return;
            // }
        }
    } else {
        // +ve and -ve number
        if (ascendingOrder) {
            if (sortArrayLength <= MERGE_SORT_THRESHOLD) {
                merge_sort_basic_asc<ArrayElementType, T>(arr, low, high);
                return;
            } else {
                if (bits_arrMaxElement > bits_arrMinElement) radixSort_asc<ArrayElementType, T>(arr, low, high, sortArrayLength, bits_arrMaxElement);
                else radixSort_asc<ArrayElementType, T>(arr, low, high, sortArrayLength, bits_arrMinElement);
                return;
            }
        } else {
            // TODO: implement the functions for descending Order Sorting
            // if (sortArrayLength <= MERGE_SORT_THRESHOLD) {
            //     merge_sort_basic_desc<ArrayElementType, T>(arr, low, high);
            //     return;
            // } else {
            // if (bits_arrMaxElement > bits_arrMinElement) radixSort_desc<ArrayElementType, T>(arr, low, high, sortArrayLength, bits_arrMaxElement);
            // else radixSort_desc<ArrayElementType, T>(arr, low, high, sortArrayLength, bits_arrMinElement);
            // return;
            // }
        }
    }
}

//#########################################################################################################################################
//-----------------------------------------------------------------------------------------------------------------------------------------
//#########################################################################################################################################
//-----------------------------------------------------------------------------------------------------------------------------------------
//#########################################################################################################################################

/*// Quick sort
template<typename T, typename ArrayElement, typename myComparator>
inline void quick_sort(T &arr, const int64_t &low, const int64_t &high, myComparator Comp) {
    if (low >= high) return;

    if ((high - low) < INSERTION_SORT_THRESHOLD) {
        insertion_sort<T, ArrayElement, myComparator>(arr, low, high, Comp);
        return;
    }

    ArrayElement mid = (low + high) >> 1;
    if (Comp(arr[low], arr[mid])) std::swap(arr[mid], arr[low]);
    if (Comp(arr[high], arr[mid])) std::swap(arr[mid], arr[high]);
    if (Comp(arr[high], arr[low])) std::swap(arr[low], arr[high]);
    ArrayElement pivot = arr[low];    // pivot

    int64_t i = low;
    int64_t j = high + 1;

    while (i < j) {

        // find item on lo to swap
        // while (arr[++i] < pivot) {
        while (Comp(arr[++i], pivot)) {
            if (i == high) break;
        }

        // find item on hi to swap
        // while (pivot < arr[--j]) {
        while (Comp(pivot, arr[--j])) {
            // the following condition is - redundant since arr[low] acts as sentinel
            // if (j == low) break;
        }

        // check if pointers cross
        if (i < j) std::swap(arr[i], arr[j]);
    }

    // put partitioning item pivot at arr[j]
    std::swap(arr[low], arr[j]);

    if ((j - 1 - low) < INSERTION_SORT_THRESHOLD) insertion_sort<T, ArrayElement, myComparator>(arr, low, j - 1, Comp);
    else quick_sort<T, ArrayElement, myComparator>(arr, low, j - 1, Comp);

    if ((high - j - 1) < INSERTION_SORT_THRESHOLD) insertion_sort<T, ArrayElement, myComparator>(arr, j + 1, high, Comp);
    else quick_sort<T, ArrayElement, myComparator>(arr, j + 1, high, Comp);
}

template<typename T>
inline void quick_sort(T &arr, const int64_t &low, const int64_t &high, bool ascendingOrder) {
    using ArrayElement = std::remove_reference_t<std::remove_const_t<decltype(arr[0])>>;

    if (ascendingOrder) quick_sort<T, ArrayElement>(arr, low, high, [](ArrayElement &a, ArrayElement &b) { return a < b; });
    else quick_sort<T, ArrayElement>(arr, low, high, [](ArrayElement &a, ArrayElement &b) { return a > b; });
}

template<typename T, typename myGetIndex>
inline void quick_sort(T &arr, const int64_t &low, const int64_t &high, bool ascendingOrder, myGetIndex getIndex) {
    using ArrayElement = std::remove_reference_t<std::remove_const_t<decltype(arr[0])>>;

    if (ascendingOrder) quick_sort<T, ArrayElement>(arr, low, high, [getIndex](ArrayElement &a, ArrayElement &b) { return getIndex(a) < getIndex(b); });
    else quick_sort<T, ArrayElement>(arr, low, high, [getIndex](ArrayElement &a, ArrayElement &b) { return getIndex(a) > getIndex(b); });
}*/

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
#pragma clang diagnostic pop