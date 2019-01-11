#ifndef IR_SORT_INTEGER_SORT_OBJECTS_SMALL_CPP
#define IR_SORT_INTEGER_SORT_OBJECTS_SMALL_CPP

#include <cstdint>
#include <iterator>

#include "basic_sorts.cpp"
#include "integer_sort.hpp"

namespace ir_sort {
    namespace object_integer_sort_functions {

//#########################################################################################
//                                                                                       ##
//                    C O U N T I N G   S O R T   -   O B J E C T S                      ##
//                                                                                       ##
//#########################################################################################

        template<typename ArrayIndexType, typename ArrayValueType, typename ArrayPointerType, typename myGetIndex>
        inline void
        countingSort_asc_object(ArrayPointerType arr, const int_fast64_t low, int_fast64_t high, const int_fast64_t sortArrayLength,
                                const ArrayIndexType &arrMaxElement, const ArrayIndexType &arrMinElement, myGetIndex getIndex) {
            // NOTE: This is the modified version created by me so that it works for negative numbers too.
            //       This sorting algorithm is not valid for negative numbers, however I have modified it
            //       so that it works for -ve numbers also.

            // arrMaxElement: largest element in the array to be sorted. Can be +ve or -ve.
            // arrMinElement: smallest element in the array to be sorted. Can be +ve or -ve.

            // i: used for iterations
            // j: used for the index which is to be used to store the next sorted element
            int_fast64_t i, j;

            int_fast64_t boxArraySize = arrMaxElement - arrMinElement + 1;

            // the "()" is used create new array and make all the elements of the array to 0
            auto box = new uint32_t[boxArraySize]();
            auto *p_copyArray = new ArrayValueType[high - low + 1];     // this dynamic array is used to store the partially sorted array // FAST and reliable
            ArrayValueType *p_arr = &arr[low];
            high -= low;

            // MODIFIED
            // This for loop is used to increment the value of the i positioned element of the
            // counting array(+ve and -ve) to say that the element is present in that quantity
            for (i = 0; i <= high; i++) {
                box[getIndex(arr[i]) - arrMinElement]++;
            }

            // NEW
            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (i = 1; i < boxArraySize; i++) {
                box[i] += box[i - 1];
            }

            // NEW
            // sort the array
            for (i = high; i >= 0; i--) {
                p_copyArray[--box[getIndex(p_arr[i])] - arrMinElement] = p_arr[i];
            }

            // NEW
            // copy the contents back to the original array
            for (i = 0; i < sortArrayLength; i++) {
                p_arr[i] = p_copyArray[i];
            }

            delete[] p_copyArray;
            delete[] box;
        }

        template<typename ArrayIndexType, typename ArrayValueType, typename ArrayPointerType, typename myGetIndex>
        inline void
        countingSort_desc_object(ArrayPointerType arr, const int_fast64_t low, int_fast64_t high, const int_fast64_t sortArrayLength,
                                 const ArrayIndexType &arrMaxElement, const ArrayIndexType &arrMinElement, myGetIndex getIndex) {
            // NOTE: This is the modified version created by me so that it works for negative numbers too.
            //       This sorting algorithm is not valid for negative numbers, however I have modified it
            //       so that it works for -ve numbers also.

            // arrMaxElement: largest element in the array to be sorted. Can be +ve or -ve.
            // arrMinElement: smallest element in the array to be sorted. Can be +ve or -ve.

            // i: used for iterations
            // j: used for the index which is to be used to store the next sorted element
            int_fast64_t i, j;

            int_fast64_t boxArraySize = arrMaxElement - arrMinElement + 1;

            // the "()" is used create new array and make all the elements of the array to 0
            auto box = new uint32_t[boxArraySize]();
            auto *p_copyArray = new ArrayValueType[high - low + 1];     // this dynamic array is used to store the partially sorted array // FAST and reliable
            ArrayValueType *p_arr = &arr[low];
            high -= low;

            // This for loop is used to increment the value of the i positioned element of the
            // counting array(+ve and -ve) to say that the element is present in that quantity
            for (i = 0; i <= high; i++) {
                box[getIndex(arr[i]) - arrMinElement]++;
            }

            // MODIFIED from above
            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (i = boxArraySize - 2; i >= 0; i--) {
                box[i] += box[i + 1];
            }

            // sort the array
            for (i = high; i >= 0; i--) {
                p_copyArray[--box[getIndex(p_arr[i])] - arrMinElement] = p_arr[i];
            }

            // copy the contents back to the original array
            for (i = 0; i < sortArrayLength; i++) {
                p_arr[i] = p_copyArray[i];
            }

            delete[] p_copyArray;
            delete[] box;
        }

//#########################################################################################
//                                                                                       ##
//      R A D I X   S O R T   -   A S C E N D I N G   O R D E R   -   O B J E C T        ##
//                                                                                       ##
//#########################################################################################


        // Sorts the +ve numbers in ascending order
        template<typename ArrayValueType, typename ArrayPointerType, typename myGetIndex>
        inline void
        radixSort_Positive_asc(ArrayPointerType arr, const int_fast64_t low, int_fast64_t high,
                               const int_fast64_t sortArrayLength, const int_fast16_t &bits_arrMaxElement, myGetIndex getIndex) {
            if (low >= high) return;

            // SUBORDINATE VARIABLES
            // ArrayValueType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
            // std::vector<ArrayValueType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
            auto *copyArray = new ArrayValueType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
            ArrayValueType *p_arr = &arr[low];
            ArrayValueType *p_copyArray = &copyArray[0];
            high -= low;

            // REQUIRED VARIABLES
            int_fast64_t i;                     // IMPORTANT: used for all iterations
            int_fast16_t bitsShift;             // IMPORTANT: bits to be considered for sorting
            int_fast16_t iterationsRequired;    // IMPORTANT: number of passes for the given array

            ir_sort::integer_sort_common::set_BitShift_Iterations(bits_arrMaxElement, bitsShift, iterationsRequired);

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
                    box[(getIndex(p_arr[i]) >> bitsShiftFactor) & modNum]++;
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
                std::swap<ArrayValueType *>(p_arr, p_copyArray);

                // this for loop is used to insert the elements of "p_copyArray" into "p_arr" in sorted
                // order of the column being scanned of the original array
                for (i = high; i >= 0; i--) {
                    p_arr[--box[((getIndex(p_copyArray[i]) >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
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
        template<typename ArrayValueType, typename ArrayPointerType, typename myGetIndex>
        inline void
        radixSort_asc(ArrayPointerType arr, const int_fast64_t low, int_fast64_t high,
                      const int_fast64_t sortArrayLength, const int_fast16_t &maxBits, myGetIndex getIndex) {
            if (low >= high) return;

            // SUBORDINATE VARIABLES
            // ArrayValueType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
            // std::vector<ArrayValueType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
            auto *copyArray = new ArrayValueType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
            ArrayValueType *p_arr = &arr[low];
            ArrayValueType *p_copyArray = &copyArray[0];
            high -= low;

            // REQUIRED VARIABLES
            int_fast64_t i, j;                  // IMPORTANT: used for all iterations
            int_fast16_t bitsShift;             // IMPORTANT: bits to be considered for sorting
            int_fast16_t iterationsRequired;    // IMPORTANT: number of passes for the given array

            ir_sort::integer_sort_common::set_BitShift_Iterations(maxBits, bitsShift, iterationsRequired);

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
                    box[(getIndex(p_arr[i]) >> bitsShiftFactor) & modNum]++;
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
                std::swap<ArrayValueType *>(p_arr, p_copyArray);

                // this for loop is used to insert the elements of "p_copyArray" into "p_arr" in sorted
                // order of the column being scanned of the original array
                for (i = high; i >= 0; i--) {
                    p_arr[--box[((getIndex(p_copyArray[i]) >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
                }

                // this operation is done so that we move to the next column of array elements
                bitsShiftFactor += bitsShift;
            }

            // Order of sorting till here: 0, 1, 2, 3, ..., -3, -2, -1

            // ##### NEW START #####
            int_fast64_t binarySearch_low = 0, binarySearch_high = high, binarySearch_mid;
            while (binarySearch_low <= binarySearch_high) {
                binarySearch_mid = (binarySearch_low + binarySearch_high) >> 1;
                if (getIndex(p_arr[binarySearch_mid]) < 0) binarySearch_high = binarySearch_mid - 1;
                else binarySearch_low = binarySearch_mid + 1;
            }

            // binarySearch_low points to the first -ve number
            if (binarySearch_low + 1 <= high) while (getIndex(p_arr[binarySearch_low + 1]) >= 0) binarySearch_low++;
            if (binarySearch_low - 1 >= 0) while (getIndex(p_arr[binarySearch_low - 1]) < 0) binarySearch_low--;

            std::swap<ArrayValueType *>(p_arr, p_copyArray);
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

//#########################################################################################
//                                                                                       ##
//      R A D I X   S O R T   -   D E S C E N D I N G   O R D E R   -   O B J E C T      ##
//                                                                                       ##
//#########################################################################################

        // Sorts the +ve numbers in descending order
        template<typename ArrayValueType, typename ArrayPointerType, typename myGetIndex>
        inline void
        radixSort_Positive_desc(ArrayPointerType arr, const int_fast64_t low, int_fast64_t high,
                                const int_fast64_t sortArrayLength, const int_fast16_t &bits_arrMaxElement, myGetIndex getIndex) {
            if (low >= high) return;

            // SUBORDINATE VARIABLES
            // ArrayValueType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
            // std::vector<ArrayValueType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
            auto *copyArray = new ArrayValueType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
            ArrayValueType *p_arr = &arr[low];
            ArrayValueType *p_copyArray = &copyArray[0];
            high -= low;

            // REQUIRED VARIABLES
            int_fast64_t i;                     // IMPORTANT: used for all iterations
            int_fast16_t bitsShift;             // IMPORTANT: bits to be considered for sorting
            int_fast16_t iterationsRequired;    // IMPORTANT: number of passes for the given array

            ir_sort::integer_sort_common::set_BitShift_Iterations(bits_arrMaxElement, bitsShift, iterationsRequired);

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
                    box[(getIndex(p_arr[i]) >> bitsShiftFactor) & modNum]++;
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
                std::swap<ArrayValueType *>(p_arr, p_copyArray);

                // this for loop is used to insert the elements of "p_copyArray" into "p_arr" in sorted
                // order of the column being scanned of the original array
                for (i = high; i >= 0; i--) {
                    p_arr[--box[((getIndex(p_copyArray[i]) >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
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
        template<typename ArrayValueType, typename ArrayPointerType, typename myGetIndex>
        inline void
        radixSort_desc(ArrayPointerType arr, const int_fast64_t low, int_fast64_t high,
                       const int_fast64_t sortArrayLength, const int_fast16_t &maxBits, myGetIndex getIndex) {
            if (low >= high) return;

            // SUBORDINATE VARIABLES
            // ArrayValueType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
            // std::vector<ArrayValueType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
            auto *copyArray = new ArrayValueType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
            ArrayValueType *p_arr = &arr[low];
            ArrayValueType *p_copyArray = &copyArray[0];
            high -= low;

            // REQUIRED VARIABLES
            int_fast64_t i, j;                  // IMPORTANT: used for all iterations
            int_fast16_t bitsShift;             // IMPORTANT: bits to be considered for sorting
            int_fast16_t iterationsRequired;    // IMPORTANT: number of passes for the given array

            ir_sort::integer_sort_common::set_BitShift_Iterations(maxBits, bitsShift, iterationsRequired);

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
                    box[(getIndex(p_arr[i]) >> bitsShiftFactor) & modNum]++;
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
                std::swap<ArrayValueType *>(p_arr, p_copyArray);

                // this for loop is used to insert the elements of "p_copyArray" into "p_arr" in sorted
                // order of the column being scanned of the original array
                for (i = high; i >= 0; i--) {
                    p_arr[--box[((getIndex(p_copyArray[i]) >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
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
                if (getIndex(p_arr[binarySearch_mid]) < 0) binarySearch_low = binarySearch_mid + 1;
                else binarySearch_high = binarySearch_mid - 1;
            }

            // MODIFIED: from radixSort_asc
            // binarySearch_low points to the first +ve number
            if (getIndex(p_arr[high]) < 0) binarySearch_low = high + 1;
            else if (getIndex(p_arr[0]) > 0) binarySearch_low = 0;

            if (binarySearch_low + 1 <= high) while (getIndex(p_arr[binarySearch_low + 1]) < 0) binarySearch_low++;
            if (binarySearch_low - 1 >= 0) while (getIndex(p_arr[binarySearch_low - 1]) >= 0) binarySearch_low--;

            std::swap<ArrayValueType *>(p_arr, p_copyArray);
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

//#########################################################################################
//                                                                                       ##
//                     I N T E G E R   S O R T   -   O B J E C T S                       ##
//                                                                                       ##
//#########################################################################################

        // integer_radix_sort
        // all the elements from arr[low] to arr[high] are sorted, both low and high inclusive
        // template<typename ArrayElementType, typename T>
        template<typename RandomAccessIterator, typename myGetIndex>
        inline void
        integer_sort_small_obj(RandomAccessIterator &arr, const int_fast64_t low, const int_fast64_t high, myGetIndex getIndex, bool &ascendingOrder,
                               int_fast16_t &forceLinearSort) {
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

            using ArrayValueType = typename std::iterator_traits<RandomAccessIterator>::value_type;
            using ArrayPointerType = ArrayValueType *; // this is pointer to an element of the array for which RandomAccessIterator is passed
            using ArrayIndexType = decltype(getIndex(arr[0]));
            // using ArrayIndexType = std::remove_reference_t<std::remove_const_t<decltype(getIndex(arr[0]))>>;

            ArrayPointerType p_arr = &arr[0];
            if ((&arr[1]) != ((&arr[0]) + 1)) {
                // the given parameter "arr" is a reverse iterator
                p_arr = &arr[high];
                ascendingOrder = (!ascendingOrder);
            }

            const int_fast64_t sortArrayLength = high - low + 1;


            // ==========================================================
            // TODO, change the THRESHOLDS for objects
            if (forceLinearSort > 0) {
                // FORCE radix_sort or counting_sort
                // forceLinearSort == 1: uses counting_sort only
                // forceLinearSort == 2: uses radix_sort or counting_sort, whichever is better
                ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 0;
            } else if (forceLinearSort == -1) {
                // FORCE merge_sort
                if (ascendingOrder) ir_sort::basic_sorts::merge_sort_asc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, getIndex);
                else ir_sort::basic_sorts::merge_sort_desc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, getIndex);
                return;
            } else if (forceLinearSort == -2) {
                // FORCE insertion_sort
                if (ascendingOrder) ir_sort::basic_sorts::insertion_sort_asc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, getIndex);
                else ir_sort::basic_sorts::insertion_sort_desc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, getIndex);
                return;
            } else {
                // dynamically choose the sort
                // set merge_sort threshold based on size of ArrayValueType
                std::size_t sizeOfArrayElementType = sizeof(ArrayValueType);
                if (sizeOfArrayElementType == 1) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 49;
                else if (sizeOfArrayElementType == 2) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 189;
                else if (sizeOfArrayElementType == 4) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 194;
                else if (sizeOfArrayElementType == 8) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 193;
                forceLinearSort = 0;    // if by any chance, forceLinearSort has any value less than -2 then it will be changed to 0
            }


            // ==========================================================
            // merge_sort
            if (sortArrayLength < ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                if (ascendingOrder) ir_sort::basic_sorts::merge_sort_asc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, getIndex);
                else ir_sort::basic_sorts::merge_sort_desc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, getIndex);
                return;
            }


            // ==========================================================
            // arrMaxElement: largest element in "arr"
            // arrMinElement: smallest element in "arr"
            ArrayIndexType arrMaxElement = getIndex(p_arr[high]), arrMinElement = getIndex(p_arr[high]);

            // this for loop is used to FIND THE MAXIMUM and MINIMUM of all the elements in the given array/RandomAccessIterator
            for (int_fast64_t i = low; i < high; i++) {
                if (getIndex(p_arr[i]) > arrMaxElement) arrMaxElement = getIndex(p_arr[i]);
                else if (getIndex(p_arr[i]) < arrMinElement) arrMinElement = getIndex(p_arr[i]);
            }


            // counting sort
            if ((1.0 * sortArrayLength) / (arrMaxElement - arrMinElement) > 0.71 || forceLinearSort == 2) {
                if (ascendingOrder)
                    countingSort_asc_object<ArrayIndexType, ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, sortArrayLength, arrMaxElement, arrMinElement,
                                                                                             getIndex);
                else
                    countingSort_desc_object<ArrayIndexType, ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, sortArrayLength, arrMaxElement, arrMinElement,
                                                                                              getIndex);
                return;
            }


            // ==========================================================
            int_fast16_t onlyPositiveNumbers = 0;                       //  0 means +ve and -ve numbers
            if (arrMinElement > -1) onlyPositiveNumbers = 1;            //  1 means +ve numbers ONLY
            else if (arrMaxElement < 0) onlyPositiveNumbers = -1;       // -1 means -ve numbers ONLY

            int_fast16_t bits_arrMaxElement = ir_sort::integer_sort_common::countBits(arrMaxElement),
                         bits_arrMinElement = ir_sort::integer_sort_common::countBits(arrMinElement);

            // IMPORTANT
            // this is done so that the extra bit can be used to segregate -ve numbers from the +ve numbers
            if (onlyPositiveNumbers == 0) {
                bits_arrMinElement++;
                if (bits_arrMinElement <= bits_arrMaxElement) bits_arrMaxElement++;
                else bits_arrMaxElement = bits_arrMinElement;
            } else if (bits_arrMinElement > bits_arrMaxElement) bits_arrMaxElement = bits_arrMinElement;


            if (forceLinearSort == 0 && sortArrayLength < 408) {
                if (onlyPositiveNumbers != 0) {
                    // +ve and -ve numbers
                    if (bits_arrMaxElement <= 8) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 49;
                    else if (bits_arrMaxElement <= 16) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 77;
                    else if (bits_arrMaxElement <= 24) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 134;
                    else if (bits_arrMaxElement <= 32) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 165;
                    else if (bits_arrMaxElement <= 40) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 245;
                    else if (bits_arrMaxElement <= 48) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 302;
                    else if (bits_arrMaxElement <= 56) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 407;
                    else ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 455;
                } else {
                    // +ve numbers ONLY
                    if (bits_arrMaxElement <= 8) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 0;
                    else if (bits_arrMaxElement <= 16) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 72;
                    else if (bits_arrMaxElement <= 24) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 129;
                    else if (bits_arrMaxElement <= 32) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 164;
                    else if (bits_arrMaxElement <= 40) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 235;
                    else if (bits_arrMaxElement <= 48) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 291;
                    else if (bits_arrMaxElement <= 56) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 401;
                    else ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 457;
                }
            }


            // ==========================================================
            switch (onlyPositiveNumbers) {
                case 0:
                    // +ve and -ve number to be sorted
                    if (ascendingOrder) {
                        if (sortArrayLength <= ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                            ir_sort::basic_sorts::merge_sort_asc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, getIndex);
                            return;
                        } else {
                            radixSort_asc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, sortArrayLength, bits_arrMaxElement, getIndex);
                            return;
                        }
                    } else {
                        if (sortArrayLength <= ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                            ir_sort::basic_sorts::merge_sort_desc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, getIndex);
                            return;
                        } else {
                            radixSort_desc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, sortArrayLength, bits_arrMaxElement, getIndex);
                            return;
                        }
                    }
                default:
                    // +ve numbers ONLY
                    // -ve numbers ONLY
                    if (ascendingOrder) {
                        if (sortArrayLength <= ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                            ir_sort::basic_sorts::merge_sort_asc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, getIndex);
                            return;
                        } else {
                            radixSort_Positive_asc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, sortArrayLength, bits_arrMaxElement, getIndex);
                            return;
                        }
                    } else {
                        if (sortArrayLength <= ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                            ir_sort::basic_sorts::merge_sort_desc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, getIndex);
                            return;
                        } else {
                            radixSort_Positive_desc<ArrayValueType, ArrayPointerType, myGetIndex>(p_arr, low, high, sortArrayLength, bits_arrMaxElement, getIndex);
                            return;
                        }
                    }
            }
        }

    } // namespace object_integer_sort_functions

    // this function call has parameters similar to std::sort
    template<typename RandomAccessIterator, typename myGetIndex>
    inline void
    integer_sort_small_obj(RandomAccessIterator start, RandomAccessIterator end, myGetIndex getIndex, bool ascendingOrder = true, int_fast16_t forceLinearSort = 0) {
        // return if RandomAccessIterator is not a random_access_iterator
        if (typeid(typename std::iterator_traits<RandomAccessIterator>::iterator_category) != typeid(std::random_access_iterator_tag))
            return;

        // return if getIndex does not return integer for the given RandomAccessIterator
        if (! std::is_integral<decltype(getIndex(start[0]))>::value)
            return;

        object_integer_sort_functions::integer_sort_small_obj<RandomAccessIterator, myGetIndex>(start, 0, end - start - 1, getIndex, ascendingOrder, forceLinearSort);
    }

} // namespace ir_sort

#endif //IR_SORT_INTEGER_SORT_OBJECTS_SMALL_CPP
