#ifndef PROJECT_SORTING_IR_SORT_CPP
#define PROJECT_SORTING_IR_SORT_CPP

#include "integer_sort.h"
#include "basic_sorts.h"

//*****************************************************************************************************************************************
template<typename ArrayElementType, typename T>
inline void
countingSort_asc(T &arr, const int_fast64_t &low, const int_fast64_t &high,
                 const ArrayElementType &arrMaxElement, const ArrayElementType &arrMinElement) {
    // NOTE: This is the modified version created by me so that it works for negative numbers too.
    //       This sorting algorithm is not valid for negative numbers, however I have modified it
    //       so that it works for -ve numbers also.

    // arrMaxElement: largest element in the array to be sorted. Can be +ve or -ve.
    // arrMinElement: smallest element in the array to be sorted. Can be +ve or -ve.

    // i: used for iterations
    // j: used for the index which is to be used to store the next sorted element
    int_fast64_t i, j;

    int_fast64_t boxLength = arrMaxElement - arrMinElement + 1;

    // the "()" is used create new array and make all the elements of the array to 0
    auto box = new uint32_t[boxLength]();

    // This for loop is used to increment the value of the i positioned element of the
    // counting array(+ve and -ve) to say that the element is present in that quantity
    for (i = low; i <= high; i++) {
        box[arr[i] - arrMinElement]++;
    }

    for (i = 0, j = 0; i < boxLength; i++) {
        while (box[i]-- > 0) arr[j++] = i + arrMinElement;
    }

    delete[] box;
}

template<typename ArrayElementType, typename T>
inline void
countingSort_desc(T &arr, const int_fast64_t &low, const int_fast64_t &high,
                  const ArrayElementType &arrMaxElement, const ArrayElementType &arrMinElement) {
    // NOTE: This is the modified version created by me so that it works for negative numbers too.
    //       This sorting algorithm is not valid for negative numbers, however I have modified it
    //       so that it works for -ve numbers also.

    // arrMaxElement: largest element in the array to be sorted. Can be +ve or -ve.
    // arrMinElement: smallest element in the array to be sorted. Can be +ve or -ve.

    // i: used for iterations
    // j: used for the index which is to be used to store the next sorted element
    int_fast64_t i, j;

    int_fast64_t boxLength = arrMaxElement - arrMinElement + 1;

    // the "()" is used create new array and make all the elements of the array to 0
    auto box = new uint32_t[boxLength]();

    // This for loop is used to increment the value of the i positioned element of the
    // counting array(+ve and -ve) to say that the element is present in that quantity
    for (i = low; i <= high; i++) {
        box[arr[i] - arrMinElement]++;
    }

    // MODIFIED
    for (i = boxLength - 1, j = 0; i >= 0; i--) {
        while (box[i]-- > 0) arr[j++] = i + arrMinElement;
    }

    delete[] box;
}

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
// Sorts the +ve numbers in ascending order
template<typename ArrayElementType, typename T>
inline void
radixSort_Positive_asc(T &arr, const int_fast64_t &low, int_fast64_t high,
                       const int_fast64_t &sortArrayLength, const int_fast16_t &bits_arrMaxElement) {
    if (low >= high) return;

    // SUBORDINATE VARIABLES
    // ArrayElementType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
    // std::vector<ArrayElementType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
    auto *copyArray = new ArrayElementType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
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

        // this for loop is used to insert the elements of "p_copyArray" into "p_arr" in sorted
        // order of the column being scanned of the original array
        for (i = high; i >= 0; i--) {
            p_arr[--box[((p_copyArray[i] >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
        }

        // this operation is done so that we move to the next column of array elements
        bitsShiftFactor += bitsShift;
    }

    // the sorted arrays is copied into the original input array if the final sorted output is in "copyArray"
    if ((&arr[low]) != p_arr) {
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
radixSort_asc(T &arr, const int_fast64_t &low, int_fast64_t high,
              const int_fast64_t &sortArrayLength, const int_fast16_t &maxBits) {
    if (low >= high) return;

    // SUBORDINATE VARIABLES
    // ArrayElementType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
    // std::vector<ArrayElementType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
    auto *copyArray = new ArrayElementType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
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

        // this for loop is used to insert the elements of "p_copyArray" into "p_arr" in sorted
        // order of the column being scanned of the original array
        for (i = high; i >= 0; i--) {
            p_arr[--box[((p_copyArray[i] >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
        }

        // this operation is done so that we move to the next column of array elements
        bitsShiftFactor += bitsShift;
    }

    // Order of sorting till here: 0, 1, 2, 3, ..., -3, -2, -1

    // ##### NEW START #####
    int_fast64_t binarySearch_low = 0, binarySearch_high = high, binarySearch_mid;
    while (binarySearch_low <= binarySearch_high) {
        binarySearch_mid = (binarySearch_low + binarySearch_high) >> 1;
        if (p_arr[binarySearch_mid] < 0) binarySearch_high = binarySearch_mid - 1;
        else binarySearch_low = binarySearch_mid + 1;
    }

    // binarySearch_low points to the first -ve number
    if (binarySearch_low + 1 <= high) while (p_arr[binarySearch_low + 1] >= 0) binarySearch_low++;
    if (binarySearch_low - 1 >= 0) while (p_arr[binarySearch_low - 1] < 0) binarySearch_low--;

    std::swap<ArrayElementType *>(p_arr, p_copyArray);
    for (i = 0, j = binarySearch_low; j <= high;) p_arr[i++] = p_copyArray[j++];
    for (j = 0; j < binarySearch_low;) p_arr[i++] = p_copyArray[j++];
    // ##### NEW END #####

    // the sorted arrays is copied into the original input array if the final sorted output is in "copyArray"
    if ((&arr[low]) != p_arr) {
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
// Sorts the +ve numbers in descending order
template<typename ArrayElementType, typename T>
inline void
radixSort_Positive_desc(T &arr, const int_fast64_t &low, int_fast64_t high,
                        const int_fast64_t &sortArrayLength, const int_fast16_t &bits_arrMaxElement) {
    if (low >= high) return;

    // SUBORDINATE VARIABLES
    // ArrayElementType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
    // std::vector<ArrayElementType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
    auto *copyArray = new ArrayElementType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
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

        // MODIFIED: from radixSort_positive_asc
        // this for loop is used to add the values of the previous element to the next element of the array "box"
        for (i = boxArraySize - 2; i >= 0; i--) {
            box[i] += box[i + 1];
        }

        // swapping is equivalent to copying all the elements of "arr" to "copyArray"
        // BEFORE swapping,
        //      "p_arr" would be storing the reference to the array sorted in the last pass
        //      "p_copyArray" would point to the old and un-updated array
        // AFTER swapping,
        //      "p_arr" would point to the old and un-updated array
        //      "p_copyArray" would be storing the reference to the array sorted in the last pass
        std::swap<ArrayElementType *>(p_arr, p_copyArray);

        // this for loop is used to insert the elements of "p_copyArray" into "p_arr" in sorted
        // order of the column being scanned of the original array
        for (i = high; i >= 0; i--) {
            p_arr[--box[((p_copyArray[i] >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
        }

        // this operation is done so that we move to the next column of array elements
        bitsShiftFactor += bitsShift;
    }

    // the sorted arrays is copied into the original input array if the final sorted output is in "copyArray"
    if ((&arr[low]) != p_arr) {
        // copy(p_arr, p_arr + sortArrayLength, arr + low);
        // copy(p_arr, p_arr + sortArrayLength, p_copyArray);
        for (i = 0; i < sortArrayLength; i++) {
            p_copyArray[i] = p_arr[i];
        }
    }

    delete[] copyArray;
}

// Sorts the +ve and -ve numbers in descending order
template<typename ArrayElementType, typename T>
inline void
radixSort_desc(T &arr, const int_fast64_t &low, int_fast64_t high,
               const int_fast64_t &sortArrayLength, const int_fast16_t &maxBits) {
    if (low >= high) return;

    // SUBORDINATE VARIABLES
    // ArrayElementType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
    // std::vector<ArrayElementType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
    auto *copyArray = new ArrayElementType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
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

        // MODIFIED: from radixSort_asc
        // this for loop is used to add the values of the previous element to the next element of the array "box"
        for (i = boxArraySize - 2; i >= 0; i--) {
            box[i] += box[i + 1];
        }

        // swapping is equivalent to copying all the elements of "arr" to "copyArray"
        // BEFORE swapping,
        //      "p_arr" would be storing the reference to the array sorted in the last pass
        //      "p_copyArray" would point to the old and un-updated array
        // AFTER swapping,
        //      "p_arr" would point to the old and un-updated array
        //      "p_copyArray" would be storing the reference to the array sorted in the last pass
        std::swap<ArrayElementType *>(p_arr, p_copyArray);

        // this for loop is used to insert the elements of "p_copyArray" into "p_arr" in sorted
        // order of the column being scanned of the original array
        for (i = high; i >= 0; i--) {
            p_arr[--box[((p_copyArray[i] >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
        }

        // this operation is done so that we move to the next column of array elements
        bitsShiftFactor += bitsShift;
    }

    // MODIFIED: from radixSort_asc
    // Order of sorting till here: -1, -2, -3, ..., 3, 2, 1, 0

    // ##### NEW START #####
    // MODIFIED: from radixSort_asc
    int_fast64_t binarySearch_low = 0, binarySearch_high = high, binarySearch_mid;
    while (binarySearch_low <= binarySearch_high) {
        binarySearch_mid = (binarySearch_low + binarySearch_high) >> 1;
        if (p_arr[binarySearch_mid] < 0) binarySearch_low = binarySearch_mid + 1;
        else binarySearch_high = binarySearch_mid - 1;
    }

    // MODIFIED: from radixSort_asc
    // binarySearch_low points to the first +ve number
    if (p_arr[high] < 0) binarySearch_low = high + 1;
    else if (p_arr[0] > 0) binarySearch_low = 0;

    if (binarySearch_low + 1 <= high) while (p_arr[binarySearch_low + 1] < 0) binarySearch_low++;
    if (binarySearch_low - 1 >= 0) while (p_arr[binarySearch_low - 1] >= 0) binarySearch_low--;

    std::swap<ArrayElementType *>(p_arr, p_copyArray);
    for (i = 0, j = binarySearch_low; j <= high;) p_arr[i++] = p_copyArray[j++];
    for (j = 0; j < binarySearch_low;) p_arr[i++] = p_copyArray[j++];
    // ##### NEW END #####

    // the sorted arrays is copied into the original input array if the final sorted output is in "copyArray"
    if ((&arr[low]) != p_arr) {
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
// all the elements from arr[low] to arr[high] are sorted, both low and high inclusive
// template<typename ArrayElementType, typename T>
template<typename T>
inline void
ir_sort(T &arr, const int_fast64_t &low, const int_fast64_t &high, const bool &ascendingOrder = true, int_fast16_t forceLinearSort = 0) {
    /* ### PARAMETERS
     * arr: the array to be sorted(values from [low, high] will be sorted)
     * low: the lower index for sorting
     * high: the higher index for sorting(the element at index high will also be considered for sorting)
     * ascendingOrder: this is used to decide whether the user wants the array to be sorted in ascending order or descending order
     * forceLinearSort: this has 4 possible values(-1, 0, 1, 2)
     *                : -2 is for insertion_sort (only this)
     *                : -1 is for merge_sort(dynamic with insertion_sort)
     *                :  0 is for dynamic selection of the sorting technique
     *                :  1 is for radix_sort (dynamic with counting_sort)
     *                :  2 is for counting_sort (only this)
     * */

    if (low >= high) return;

    using ArrayElementType = std::remove_reference_t<std::remove_const_t<decltype(arr[0])>>; // this should be used if the template parameter is only "T"
    // using ArrayElementType = typename std::remove_all_extents<decltype(arr)>::type;       // has NOT been tested

    const int_fast64_t sortArrayLength = high - low + 1;


    // ==========================================================
    if (forceLinearSort > 0) {
        // FORCE radix_sort or counting_sort
        // forceLinearSort == 1: uses counting_sort only
        // forceLinearSort == 2: uses radix_sort or counting_sort, whichever is better
        MERGE_SORT_THRESHOLD = 0;
    } else if (forceLinearSort == -1) {
        // FORCE merge_sort
        if (ascendingOrder) merge_sort_basic_asc<ArrayElementType, T>(arr, low, high);
        else merge_sort_basic_desc<ArrayElementType, T>(arr, low, high);
        return;
    } else if (forceLinearSort == -2) {
        // FORCE insertion_sort
        if (ascendingOrder) insertion_sort_basic_asc<ArrayElementType, T>(arr, low, high);
        else insertion_sort_basic_desc<ArrayElementType, T>(arr, low, high);
        return;
    } else {
        // dynamically choose the sort
        // set merge_sort threshold based on size of ArrayElementType
        std::size_t sizeOfArrayElementType = sizeof(ArrayElementType);
        if (sizeOfArrayElementType == 1) MERGE_SORT_THRESHOLD = 49;
        else if (sizeOfArrayElementType == 2) MERGE_SORT_THRESHOLD = 189;
        else if (sizeOfArrayElementType == 4) MERGE_SORT_THRESHOLD = 194;
        else if (sizeOfArrayElementType == 8) MERGE_SORT_THRESHOLD = 193;
        forceLinearSort = 0;    // if by any chance, forceLinearSort has any value less than -2 then it will be changed to 0
    }


    // ==========================================================
    // merge_sort
    if (sortArrayLength < MERGE_SORT_THRESHOLD) {
        if (ascendingOrder) merge_sort_basic_asc<ArrayElementType, T>(arr, low, high);
        else merge_sort_basic_desc<ArrayElementType, T>(arr, low, high);
        return;
    }


    // ==========================================================
    // arrMaxElement: largest element in "arr"
    // arrMinElement: smallest element in "arr"
    ArrayElementType arrMaxElement = arr[high], arrMinElement = arr[high];

    // this for loop is used to FIND THE MAXIMUM and MINIMUM of all the elements in the given array/RandomAccessIterator
    for (int_fast64_t i = low; i < high; i++) {
        if (arr[i] > arrMaxElement) arrMaxElement = arr[i];
        else if (arr[i] < arrMinElement) arrMinElement = arr[i];
    }


    // counting sort
    if ((1.0 * sortArrayLength) / (arrMaxElement - arrMinElement) > 0.71 || forceLinearSort == 2) {
        if (ascendingOrder) countingSort_asc<ArrayElementType>(arr, low, high, arrMaxElement, arrMinElement);
        else countingSort_desc<ArrayElementType>(arr, low, high, arrMaxElement, arrMinElement);
        return;
    }


    // ==========================================================
    int_fast16_t onlyPositiveNumbers = 0;                       //  0 means +ve and -ve numbers
    if (arrMinElement > -1) onlyPositiveNumbers = 1;            //  1 means +ve numbers ONLY
    else if (arrMaxElement < 0) onlyPositiveNumbers = -1;       // -1 means -ve numbers ONLY

    int_fast16_t bits_arrMaxElement = countBits(arrMaxElement), bits_arrMinElement = countBits(arrMinElement);

    // IMPORTANT
    // this is done so that the extra bit can be used to segregate -ve numbers from the +ve numbers
    if (onlyPositiveNumbers == 0) {
        bits_arrMinElement++;
        if (bits_arrMinElement <= bits_arrMaxElement) bits_arrMaxElement++;
        else bits_arrMaxElement = bits_arrMinElement;
    } else if (bits_arrMinElement > bits_arrMaxElement) bits_arrMaxElement = bits_arrMinElement;


    if (forceLinearSort == 0) {
        if (onlyPositiveNumbers != 0) {
            // +ve and -ve numbers
            if (bits_arrMaxElement <= 8) MERGE_SORT_THRESHOLD = 49;
            else if (bits_arrMaxElement <= 16) MERGE_SORT_THRESHOLD = 77;
            else if (bits_arrMaxElement <= 24) MERGE_SORT_THRESHOLD = 134;
            else if (bits_arrMaxElement <= 32) MERGE_SORT_THRESHOLD = 165;
            else if (bits_arrMaxElement <= 40) MERGE_SORT_THRESHOLD = 245;
            else if (bits_arrMaxElement <= 48) MERGE_SORT_THRESHOLD = 302;
            else if (bits_arrMaxElement <= 56) MERGE_SORT_THRESHOLD = 407;
            else MERGE_SORT_THRESHOLD = 455;
        } else {
            // +ve numbers ONLY
            // TODO: optimise the thresholds for +ve numbers only
            if (bits_arrMaxElement <= 8) MERGE_SORT_THRESHOLD = 49;
            else if (bits_arrMaxElement <= 16) MERGE_SORT_THRESHOLD = 77;
            else if (bits_arrMaxElement <= 24) MERGE_SORT_THRESHOLD = 134;
            else if (bits_arrMaxElement <= 32) MERGE_SORT_THRESHOLD = 165;
            else if (bits_arrMaxElement <= 40) MERGE_SORT_THRESHOLD = 245;
            else if (bits_arrMaxElement <= 48) MERGE_SORT_THRESHOLD = 302;
            else if (bits_arrMaxElement <= 56) MERGE_SORT_THRESHOLD = 407;
            else MERGE_SORT_THRESHOLD = 455;
        }
    }


    // ==========================================================
    switch (onlyPositiveNumbers) {
        case 0:
            // +ve and -ve number to be sorted
            if (ascendingOrder) {
                if (sortArrayLength <= MERGE_SORT_THRESHOLD) {
                    merge_sort_basic_asc<ArrayElementType, T>(arr, low, high);
                    return;
                } else {
                    radixSort_asc<ArrayElementType, T>(arr, low, high, sortArrayLength, bits_arrMaxElement);
                    return;
                }
            } else {
                if (sortArrayLength <= MERGE_SORT_THRESHOLD) {
                    merge_sort_basic_desc<ArrayElementType, T>(arr, low, high);
                    return;
                } else {
                    radixSort_desc<ArrayElementType, T>(arr, low, high, sortArrayLength, bits_arrMaxElement);
                    return;
                }
            }
        default:
            // +ve numbers ONLY
            // -ve numbers ONLY
            if (ascendingOrder) {
                if (sortArrayLength <= MERGE_SORT_THRESHOLD) {
                    merge_sort_basic_asc<ArrayElementType, T>(arr, low, high);
                    return;
                } else {
                    radixSort_Positive_asc<ArrayElementType, T>(arr, low, high, sortArrayLength, bits_arrMaxElement);
                    return;
                }
            } else {
                if (sortArrayLength <= MERGE_SORT_THRESHOLD) {
                    merge_sort_basic_desc<ArrayElementType, T>(arr, low, high);
                    return;
                } else {
                    radixSort_Positive_desc<ArrayElementType, T>(arr, low, high, sortArrayLength, bits_arrMaxElement);
                    return;
                }
            }
    }
}

// this function call has parameters similar to std::sort
template<typename RandomAccessIterator>
inline void
ir_sort(RandomAccessIterator start, RandomAccessIterator end, const bool &ascendingOrder = true, int_fast16_t forceLinearSort = 0) {
    ir_sort(start, 0, end - start - 1, ascendingOrder, forceLinearSort);
}

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
template<typename ArrayElementType, typename T, typename myGetIndex>
inline void
radixSort_Positive_asc(T &arr, const int_fast64_t &low, int_fast64_t high,
                       const int_fast64_t &sortArrayLength, myGetIndex getIndex) {
    if (low >= high) return;

    using IndexType = std::remove_reference_t<std::remove_const_t<decltype(getIndex(arr[0]))>>;

    // SUBORDINATE VARIABLES
    int_fast64_t arrMaxElement = getIndex(arr[high]);       // "arrMaxElement" is used to store the largest element in "arr"
    auto *arr_index = new IndexType[sortArrayLength];       // this array is used to store the index of partially sorted array
    auto *copyArray_index = new IndexType[sortArrayLength]; // this array is used to store the index of partially sorted array
    auto *p_arr_index = &arr_index[0];
    auto *p_copyArray_index = &copyArray_index[0];
    ArrayElementType *p_arr = &arr[low];
    high -= low;

    auto *indexTracker = new IndexType[sortArrayLength];

    // REQUIRED VARIABLES
    int_fast64_t i;                         // IMPORTANT: used for iterations
    int_fast16_t bitsShift = 8;             // IMPORTANT: bits to be considered for sorting
    int_fast16_t iterationsRequired = 0;    // IMPORTANT: number of passes for the given array

    // this for loop is used to FIND THE MAXIMUM of all the elements in the array and store its value in the variable "arrMaxElement"
    for (i = 0; i < high; i++) {
        if (getIndex(p_arr[i]) > arrMaxElement) arrMaxElement = getIndex(p_arr[i]);
    }

    set_BitShift_Iterations(countBits_positive(arrMaxElement), bitsShift, iterationsRequired);

    // this is used to access the n'th column of the element
    int_fast16_t bitsShiftFactor = 0;
    // this variable store the size of the array "box"
    const int_fast32_t boxArraySize = static_cast<int_fast32_t>(1) << (bitsShift);
    // modNum = arr[i] % boxArraySize, both are same
    const int_fast32_t modNum = boxArraySize - 1;
    // this is used to store the radix for the array elements
    // the size of the array "box" is "boxArraySize" so that the numbers from 0 to "boxArraySize - 1" can be tracked in the column being scanned
    uint_fast64_t box[boxArraySize];

    // this will fill the arr_index with the values from 0, 1, 2, 3, ..., (high - 1)
    // for (i = 0; i <= high; i++) arr_index[i] = i;
    std::iota(&arr_index[0], &arr_index[high] + 1, 0);

    // START ACTUAL SORTING
    // This condition is used so that all the digits of each element are checked and "arr" is sorted according to the digit considered
    while (iterationsRequired-- > 0) {
        // default values of all the elements of the array "box" = 0
        for (i = 0; i < boxArraySize;) {
            box[i++] = 0;
        }

        // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column
        for (i = 0; i <= high; i++) {
            box[(getIndex(p_arr[i]) >> bitsShiftFactor) & modNum]++;
        }

        // this for loop is used to add the values of the previous element to the next element of the array "box"
        for (i = 1; i < boxArraySize; i++) {
            box[i] += box[i - 1];
        }

        // this statement will copy all the elements of "arr" to "arr_index"
        // BEFORE this step,
        //      "arr_index" would be storing the reference to the array sorted in the last pass
        //      "copyArray_index" would point to the old and un-updated array
        // AFTER this step,
        //      "arr_index" would point to the old and un-updated array
        //      "copyArray_index" would be storing the reference to the array sorted in the last pass
        std::swap(p_arr_index, p_copyArray_index);

        // this for loop is used to insert the elements of array "arr_index" into "copyArray_index" in sorted order of the column being scanned of the array "arr"
        /*for (i = high; i >= 0; i--) {
            p_arr_index[--box[((getIndex(p_arr[p_copyArray_index[i]]) >> bitsShiftFactor) & modNum)]] = p_copyArray_index[i];
        }*/
        if (iterationsRequired == 0) {
            // this section will run for the last iteration over the input array
            // this block will fill the array "indexTracker"
            uint_fast64_t positionFor_p_index_arr;
            for (i = high; i >= 0; i--) {
                positionFor_p_index_arr = --box[((getIndex(p_arr[p_copyArray_index[i]]) >> bitsShiftFactor) & modNum)];
                p_arr_index[positionFor_p_index_arr] = p_copyArray_index[i];
                indexTracker[p_copyArray_index[i]] = positionFor_p_index_arr;
            }
        } else {
            for (i = high; i >= 0; i--) {
                p_arr_index[--box[((getIndex(p_arr[p_copyArray_index[i]]) >> bitsShiftFactor) & modNum)]] = p_copyArray_index[i];
            }
        }

        // this operation is done so that we move to the next column of array elements
        bitsShiftFactor += bitsShift;
    }

    // now output of "p_arr_index" = numpy.argsort(arr)
    // DONE: write loop to sort the array using "p_arr_index"
    // DONE: refer the following link https://stackoverflow.com/questions/13102277/sort-an-array-by-an-index-array-in-c
    for (i = 0; i <= high; i++) {
        std::swap(p_arr[i], p_arr[p_arr_index[i]]);
        p_arr_index[indexTracker[i]] = p_arr_index[i];
        indexTracker[p_arr_index[i]] = indexTracker[i];
    }

    delete[] indexTracker;
    delete[] arr_index;
    delete[] copyArray_index;
}

//*****************************************************************************************************************************************
#endif //PROJECT_SORTING_IR_SORT_CPP

