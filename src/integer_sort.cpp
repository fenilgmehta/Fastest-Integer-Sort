#ifndef IR_SORT_CPP
#define IR_SORT_CPP

#include <cstdint>
#include <numeric>
#include <iterator>
#include <cmath>
#include <algorithm>

#include "integer_sort.hpp"
#include "basic_sorts.cpp"
#include "integer_sort_objects_small.cpp"

namespace ir_sort {

    namespace parameter {
        // ascending_order     : parameter passed by user
        //                       (updation: refer "ir_sort::stable_integer_sort_new")
        //                       (only set once)
        // signed_sorting      : this is true if both +ve and -ve numbers are present in the array to be sorted
        //                       (updation: refer function "ir_sort::stable_integer_sort_functions::integer_sort(RandomAccessIterator start_arr)")
        // force_linear_sort   : this will decide the type of sorting to be used
        //                       (updation: refer "ir_sort::stable_integer_sort_new")
        bool ascending_order, signed_sorting;
        int_fast32_t force_linear_sort;

        // length              : length of the array to be sorted
        //                       (updation: refer "ir_sort::stable_integer_sort_new")
        // iterations_required : number of columns of size min_bits_shift to be sorted
        //                       (updation: refer "ir_sort::parameter::set_BitShift_Iterations")
        int_fast32_t length;
        int_fast32_t iterations_required;

        // box        : used to store the count of occurrences of a number in the column being scanned for sorting
        //              (updation: refer "ir_sort::parameter::reset_box")
        int_fast32_t box[256];

        // bit_mask           : it is power(2, min_bits_shift) - 1
        //                      (updation: refer "ir_sort::parameter::set_BitShift_Iterations")
        // min_bits_shift     : this will denote the number of bits to be considered per iteration, i.e the size of each column is min_bits_shift
        //                      (updation: refer "ir_sort::parameter::set_BitShift_Iterations")
        // initial_bits_shift : the initial bits to be skipped or shifted and then use next "min_bits_shift" for sorting
        //                      (updation: refer "ir_sort::parameter::set_BitShift_Iterations")
        //                      (updation: refer "ir_sort::parameter::update_initial_bits_shift")
        //                      (NOTE: updated regularly)
        // sign_bit_mask      : this number can be XORed with the signed column to sort
        //                      (updation: refer "ir_sort::parameter::set_BitShift_Iterations")
        //REMOVE // int_fast32_t bit_mask;
        //REMOVE // int_fast32_t sign_bit_mask;
        int_fast32_t min_bits_shift, initial_bits_shift;

        /*
         * This will fill "box" with 0
         * Default value of all the elements of the array "box" = 0
         *
         * */
        inline void reset_box() {
            for (auto &i: box) i = 0;
        }

        /*
         * Updates the following variables:
         * 1. initial_bits_shift
         *
         * */
        inline void update_initial_bits_shift() {
            initial_bits_shift += min_bits_shift;
        }

        /*
         * Updates the following variables:
         * 1. bits_mask
         * 2. min_bits_shift
         * 3. initial_bits_shift
         * 4. iterations_required
         *
         * */
        inline void set_BitShift_Iterations(const int_fast32_t arr_max_bits) {
            // box_length = 256;
            // if (arr_max_bits <= 8) {           //  8 bits = 1 iteration
            //     min_bits_shift = arr_max_bits;
            //     iterations_required = 1;
            //     box_length = 1 << arr_max_bits;
            // } else if (arr_max_bits <= 10) {
            //     min_bits_shift = 5;
            //     iterations_required = 2;
            // } else if (arr_max_bits <= 12) {
            //     min_bits_shift = 6;
            //     iterations_required = 2;
            // } else if (arr_max_bits <= 14) {
            //     min_bits_shift = 7;
            //     iterations_required = 2;
            // } else if (arr_max_bits <= 16) {   // 16 bits = 2 iterations
            //     min_bits_shift = 8;
            //     iterations_required = 2;
            // } else if (arr_max_bits <= 18) {
            //     min_bits_shift = 6;
            //     iterations_required = 3;
            // } else if (arr_max_bits <= 21) {
            //     min_bits_shift = 7;
            //     iterations_required = 3;
            // } else if (arr_max_bits <= 24) {   // 24 bits = 3 iterations
            //     min_bits_shift = 8;
            //     iterations_required = 3;
            // } else if (arr_max_bits <= 28) {
            //     min_bits_shift = 7;
            //     iterations_required = 4;
            // } else if (arr_max_bits <= 32) {   // 32 bits = 4 iterations
            //     min_bits_shift = 8;
            //     iterations_required = 4;
            // } else if (arr_max_bits <= 35) {
            //     min_bits_shift = 7;
            //     iterations_required = 5;
            // } else if (arr_max_bits <= 40) {   // 40 bits = 5 iterations
            //     min_bits_shift = 8;
            //     iterations_required = 5;
            // } else if (arr_max_bits <= 42) {
            //     min_bits_shift = 7;
            //     iterations_required = 6;
            // } else if (arr_max_bits <= 48) {   // 48 bits = 6 iterations
            //     min_bits_shift = 8;
            //     iterations_required = 6;
            // } else if (arr_max_bits == 49) {
            //     min_bits_shift = 7;
            //     iterations_required = 7;
            // } else if (arr_max_bits <= 56) {   // 56 bits = 7 iterations
            //     min_bits_shift = 8;
            //     iterations_required = 7;
            // } else if (arr_max_bits <= 64) {   // 64 bits = 8 iterations
            //     min_bits_shift = 8;
            //     iterations_required = 8;
            // } else {                          // Handle the situation where int128_t of int256_t or int512_t or int1024_t or anything else is used to create the array / array index
            //     min_bits_shift = 8;
            //     iterations_required = arr_max_bits >> 3;
            //     if ((arr_max_bits & 7) != 0) iterations_required++;
            // }
            // bit_mask = (1 << min_bits_shift) - 1;
            // sign_bit_mask = bit_mask ^ (bit_mask >> 1);

            iterations_required = arr_max_bits >> 3;
            if((arr_max_bits & 7) != 0) ++iterations_required;
            initial_bits_shift = 0;
        }
    };

    namespace stable_integer_sort_functions {

//############################################################################
//                                                                          ##
//                        C O U N T I N G   S O R T                         ##
//                                                                          ##
//############################################################################

        template<typename ArrayValueType, typename ArrayPointerType>
        inline void
        countingSort_asc(ArrayPointerType arr, const int_fast64_t low, const int_fast64_t high,
                         const ArrayValueType &arrMaxElement, const ArrayValueType &arrMinElement) {
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

        template<typename ArrayValueType, typename ArrayPointerType>
        inline void
        countingSort_desc(ArrayPointerType arr, const int_fast64_t low, const int_fast64_t high,
                          const ArrayValueType &arrMaxElement, const ArrayValueType &arrMinElement) {
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

//############################################################################
//                                                                          ##
//                     R A D I X   S O R T   -   N E W                      ##
//                                                                          ##
//############################################################################

        template<typename RandomAccessIterator, typename Buffer>
        inline void
        partial_radix_sort_asc(RandomAccessIterator start_arr, Buffer start_buffer) {
            parameter::reset_box();

            // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
            for (int_fast32_t i = 0; i < parameter::length; i++) {
                parameter::box[(start_arr[i] >> parameter::initial_bits_shift) & 255]++;
            }

            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (int_fast32_t i = 1; i < 256; i++) {
                parameter::box[i] += parameter::box[i - 1];
            }

            // this for loop is used to insert the elements of "start_arr" into "start_buffer" in sorted
            // order of the column being scanned of the original array
            for (int_fast32_t i = parameter::length - 1; i >= 0; i--) {
                start_buffer[--parameter::box[(start_arr[i] >> parameter::initial_bits_shift) & 255]] = start_arr[i];
            }

            parameter::update_initial_bits_shift();
        }

        template<typename RandomAccessIterator, typename Buffer>
        inline void
        partial_radix_sort_desc(RandomAccessIterator start_arr, Buffer start_buffer) {
            parameter::reset_box();

            // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
            for (int_fast32_t i = 0; i < parameter::length; i++) {
                parameter::box[(start_arr[i] >> parameter::initial_bits_shift) & 255]++;
            }

            // MODIFIED: from "partial_radix_sort_asc"
            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (int_fast32_t i = 254; i >= 0; i--) {
                parameter::box[i] += parameter::box[i + 1];
            }

            // this for loop is used to insert the elements of "start_arr" into "start_buffer" in sorted
            // order of the column being scanned of the original array
            for (int_fast32_t i = parameter::length - 1; i >= 0; i--) {
                start_buffer[--parameter::box[(start_arr[i] >> parameter::initial_bits_shift) & 255]] = start_arr[i];
            }

            parameter::update_initial_bits_shift();
        }

        template<typename RandomAccessIterator, typename Buffer>
        inline void
        partial_radix_sort_asc_flip_bit(RandomAccessIterator start_arr, Buffer start_buffer) {
            parameter::reset_box();

            // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
            for (int_fast32_t i = 0; i < parameter::length; i++) {
                parameter::box[((start_arr[i] >> parameter::initial_bits_shift) ^ 128) & 255]++;
            }

            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (int_fast32_t i = 1; i < 256; i++) {
                parameter::box[i] += parameter::box[i - 1];
            }

            // this for loop is used to insert the elements of "start_arr" into "start_buffer" in sorted
            // order of the column being scanned of the original array
            for (int_fast32_t i = parameter::length - 1; i >= 0; i--) {
                start_buffer[--parameter::box[((start_arr[i] >> parameter::initial_bits_shift) ^ 128) &
                                              255]] = start_arr[i];
            }

            parameter::update_initial_bits_shift();
        }

        // MODIFIED: only added the XOR for the sign bit
        template<typename RandomAccessIterator, typename Buffer>
        inline void
        partial_radix_sort_desc_flip_bit(RandomAccessIterator start_arr, Buffer start_buffer) {
            parameter::reset_box();

            // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
            for (int_fast32_t i = 0; i < parameter::length; i++) {
                parameter::box[((start_arr[i] >> parameter::initial_bits_shift) ^ 128) & 255]++;
            }

            // MODIFIED: from "partial_radix_sort_asc"
            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (int_fast32_t i = 254; i >= 0; i--) {
                parameter::box[i] += parameter::box[i + 1];
            }

            // this for loop is used to insert the elements of "start_arr" into "start_buffer" in sorted
            // order of the column being scanned of the original array
            for (int_fast32_t i = parameter::length - 1; i >= 0; i--) {
                start_buffer[--parameter::box[((start_arr[i] >> parameter::initial_bits_shift) ^ 128) &
                                              255]] = start_arr[i];
            }

            parameter::update_initial_bits_shift();
        }

        template<typename RandomAccessIterator, typename ArrayValueType>
        inline void
        radix_sort_asc(RandomAccessIterator start_arr, int_fast32_t arr_max_bits) {
            // this dynamic array is used to store the partially sorted array
            // FAST as compared to vector<ArrayValueType>
            // RELIABLE as compared the array on stack created using ArrayValueType buffer[parameter::length]
            auto buffer = new ArrayValueType[parameter::length];

            // parameter::initial_bits_shift = 0;
            parameter::set_BitShift_Iterations(arr_max_bits);

            int_fast32_t i = 1;
            while (i < parameter::iterations_required) {
                partial_radix_sort_asc(start_arr, buffer);
                i++;

                if (i == parameter::iterations_required) break;
                partial_radix_sort_asc(buffer, start_arr);
                i++;
            }

            // the array is a mixture of +ve and -ve numbers, then use descending order
            // for last column which contains the bit which decides the sign of a number

            switch (parameter::iterations_required % 2) {
                case 0: // even length
                    if (parameter::signed_sorting)
                        partial_radix_sort_asc_flip_bit(buffer, start_arr);
                    else
                        partial_radix_sort_asc(buffer, start_arr);
                    break;
                case 1: // odd length
                    if (parameter::signed_sorting)
                        partial_radix_sort_asc_flip_bit(start_arr, buffer);
                    else
                        partial_radix_sort_asc(start_arr, buffer);
            }

            // the sorted arrays is copied into the original input array if parameter::length is odd
            if ((parameter::iterations_required % 2) != 0) {
                // for (i = 0; i < parameter::length; i++) {
                //     start_arr[i] = buffer[i];
                // }
                std::copy(buffer, std::next(buffer, parameter::length), start_arr);
            }

            delete[] buffer;
        }

        template<typename RandomAccessIterator, typename ArrayValueType>
        inline void
        radix_sort_desc(RandomAccessIterator start_arr, int_fast32_t arr_max_bits) {
            // this dynamic array is used to store the partially sorted array
            // FAST as compared to vector<ArrayValueType>
            // RELIABLE as compared the array on stack created using ArrayValueType buffer[parameter::length]
            auto buffer = new ArrayValueType[parameter::length];

            // parameter::initial_bits_shift=0;
            parameter::set_BitShift_Iterations(arr_max_bits);

            int_fast32_t i = 1;
            while (i < parameter::iterations_required) {
                partial_radix_sort_desc(start_arr, buffer);
                i++;

                if (i == parameter::iterations_required) break;
                partial_radix_sort_desc(buffer, start_arr);
                i++;
            }

            // the array is a mixture of +ve and -ve numbers, then use descending order
            // for last column which contains the bit which decides the sign of a number

            switch (parameter::iterations_required % 2) {
                case 0: // even length
                    if (parameter::signed_sorting)
                        partial_radix_sort_desc_flip_bit(buffer, start_arr);
                    else
                        partial_radix_sort_desc(buffer, start_arr);
                    break;
                case 1: // odd length
                    if (parameter::signed_sorting)
                        partial_radix_sort_desc_flip_bit(start_arr, buffer);
                    else
                        partial_radix_sort_desc(start_arr, buffer);
            }

            // the sorted arrays is copied into the original input array if parameter::length is odd
            if ((parameter::iterations_required % 2) != 0) {
                // for (i = 0; i < parameter::length; i++) {
                //     start_arr[i] = buffer[i];
                // }
                std::copy(buffer, std::next(buffer, parameter::length), start_arr);
            }

            delete[] buffer;
        }

//############################################################################
//                                                                          ##
//         R A D I X   S O R T   -   A S C E N D I N G   O R D E R          ##
//                                                                          ##
//############################################################################

        // Sorts the +ve numbers in ascending order
        template<typename ArrayValueType, typename ArrayPointerType>
        inline void
        radixSort_Positive_asc(ArrayPointerType arr, const int_fast64_t low, const int_fast64_t high,
                               const int_fast64_t sortArrayLength, const int_fast16_t &bits_arrMaxElement) {
            if (low >= high) return;

            // SUBORDINATE VARIABLES
            // ArrayValueType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
            // std::vector<ArrayValueType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
            ArrayPointerType copyArray = new ArrayValueType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
            ArrayPointerType p_arr = &arr[low];
            ArrayPointerType p_copyArray = &copyArray[0];
            // high -= low; // this is of no use since the call to integer_sort will set low=0 always

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
                std::swap<ArrayValueType *>(p_arr, p_copyArray);

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
        template<typename ArrayValueType, typename ArrayPointerType>
        inline void
        radixSort_asc(ArrayPointerType arr, const int_fast64_t low, const int_fast64_t high,
                      const int_fast64_t sortArrayLength, const int_fast16_t &maxBits) {
            if (low >= high) return;

            // SUBORDINATE VARIABLES
            // ArrayValueType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
            // std::vector<ArrayValueType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
            auto *copyArray = new ArrayValueType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
            ArrayValueType *p_arr = &arr[low];
            ArrayValueType *p_copyArray = &copyArray[0];
            // high -= low; // this is of no use since the call to integer_sort will set low=0 always

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
                std::swap<ArrayValueType *>(p_arr, p_copyArray);

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

            // NOTE: though the below program me seem to be wrong for situations where all numbers are +ve or -ve
            // However, this function will only be called if there is atleast one -ve in all +ve OR one +ve in all -ve
            // binarySearch_low points to the first -ve number
            if (binarySearch_low + 1 <= high) while (p_arr[binarySearch_low + 1] >= 0) binarySearch_low++;
            if (binarySearch_low - 1 >= 0) while (p_arr[binarySearch_low - 1] < 0) binarySearch_low--;

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

//############################################################################
//                                                                          ##
//        R A D I X   S O R T   -   D E S C E N D I N G   O R D E R         ##
//                                                                          ##
//############################################################################

        // Sorts the +ve numbers in descending order
        template<typename ArrayValueType, typename ArrayPointerType>
        inline void
        radixSort_Positive_desc(ArrayPointerType arr, const int_fast64_t low, const int_fast64_t high,
                                const int_fast64_t sortArrayLength, const int_fast16_t &bits_arrMaxElement) {
            if (low >= high) return;

            // SUBORDINATE VARIABLES
            // ArrayValueType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
            // std::vector<ArrayValueType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
            auto *copyArray = new ArrayValueType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
            ArrayValueType *p_arr = &arr[low];
            ArrayValueType *p_copyArray = &copyArray[0];
            // high -= low; // this is of no use since the call to integer_sort will set low=0 always

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
                std::swap<ArrayValueType *>(p_arr, p_copyArray);

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
        template<typename ArrayValueType, typename ArrayPointerType>
        inline void
        radixSort_desc(ArrayPointerType arr, const int_fast64_t low, const int_fast64_t high,
                       const int_fast64_t sortArrayLength, const int_fast16_t &maxBits) {
            if (low >= high) return;

            // SUBORDINATE VARIABLES
            // ArrayValueType copyArray[sortArrayLength];              // this "array" is used to store the partially sorted array       // stack memory overflow
            // std::vector<ArrayValueType> copyArray(sortArrayLength); // this "vector" is used to store the partially sorted array      // SLOW
            auto *copyArray = new ArrayValueType[sortArrayLength];     // this dynamic array is used to store the partially sorted array // FAST and reliable
            ArrayValueType *p_arr = &arr[low];
            ArrayValueType *p_copyArray = &copyArray[0];
            // high -= low; // this is of no use since the call to integer_sort will set low=0 always

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
                std::swap<ArrayValueType *>(p_arr, p_copyArray);

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

//############################################################################
//                                                                          ##
//                         I N T E G E R   S O R T                          ##
//                                                                          ##
//############################################################################

        // integer_radix_sort
        // all the elements from arr[low] to arr[high] are sorted, both low and high inclusive
        // template<typename ArrayElementType, typename T>
        template<typename RandomAccessIterator>
        inline void
        integer_sort(RandomAccessIterator arr, const int_fast64_t low, const int_fast64_t high, bool &ascendingOrder, int_fast16_t &forceLinearSort) {
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

            // NOTE: determine the type of elements stored in the array to be sorted, and it pointer type
            // NOTE: avoid using the things in comment
            // They are unverified ways to get the ArrayValueType
            // using ArrayValueType = std::remove_reference_t<std::remove_const_t<decltype(arr[0])>>; // this should be used if the template parameter is only "T"
            // using ArrayValueType = typename std::remove_all_extents<decltype(arr)>::type;       // has NOT been tested
            using ArrayValueType = typename std::iterator_traits<RandomAccessIterator>::value_type;
            using ArrayPointerType = ArrayValueType *; // this is pointer to an element of the array for which RandomAccessIterator is passed

            // NOTE: check for reverse iterator and perform appropriate actions on "p_arr"
            ArrayPointerType p_arr = (&(arr[0]));
            if ((&(arr[1])) != ((&(arr[0])) + 1)) {
                // the given parameter "arr" is a reverse iterator
                p_arr = &(arr[high]);
                ascendingOrder = (!ascendingOrder);
            }

            const int_fast64_t sortArrayLength = high - low + 1;
            if (sortArrayLength <= 1360) ir_sort::basic_sorts::INSERTION_SORT_THRESHOLD = ir_sort::basic_sorts::THRESHOLD_INSERTION_FOR_MERGE[high - low + 1];

            // ==========================================================
            if (forceLinearSort > 0) {
                // FORCE radix_sort or counting_sort
                // forceLinearSort == 1: uses radix_sort or counting_sort, whichever is better
                // forceLinearSort == 2: uses counting_sort only
                ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 0;
            } else if (forceLinearSort == -1) {
                // FORCE merge_sort and insertion dynamically
                if (ascendingOrder) ir_sort::basic_sorts::merge_sort_basic_asc<ArrayPointerType, ArrayValueType>(p_arr, low, high);
                else ir_sort::basic_sorts::merge_sort_basic_desc<ArrayPointerType, ArrayValueType>(p_arr, low, high);
                return;
            } else if (forceLinearSort == -2) {
                // FORCE insertion_sort
                if (ascendingOrder) ir_sort::basic_sorts::insertion_sort_basic_asc<ArrayPointerType, ArrayValueType>(p_arr, low, high);
                else ir_sort::basic_sorts::insertion_sort_basic_desc<ArrayPointerType, ArrayValueType>(p_arr, low, high);
                return;
            } else {
                // dynamically choose the sort
                // set merge_sort threshold based on size of ArrayValueType
                std::size_t sizeOfArrayValueType = sizeof(ArrayValueType);
                if (sizeOfArrayValueType == 1) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 49;
                else if (sizeOfArrayValueType == 2) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 189;
                else if (sizeOfArrayValueType == 4) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 194;
                else if (sizeOfArrayValueType == 8) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 193;
                forceLinearSort = 0;    // if by any chance, forceLinearSort has any value less than -2 then it will be changed to 0
            }


            // ==========================================================
            // merge_sort
            if (sortArrayLength < ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                if (ascendingOrder) ir_sort::basic_sorts::merge_sort_basic_asc<ArrayPointerType, ArrayValueType>(p_arr, low, high);
                else ir_sort::basic_sorts::merge_sort_basic_desc<ArrayPointerType, ArrayValueType>(p_arr, low, high);
                return;
            }


            // ==========================================================
            // arrMinElement: smallest element in "arr"
            // arrMaxElement: largest element in "arr"
            ArrayValueType arrMinElement = p_arr[high], arrMaxElement = p_arr[high];

            // this for loop is used to FIND THE MAXIMUM and MINIMUM of all the elements in the given array with the help of RandomAccessIterator
            for (int_fast64_t i = low; i < high; i++) {
                if (p_arr[i] > arrMaxElement) arrMaxElement = p_arr[i];
                else if (p_arr[i] < arrMinElement) arrMinElement = p_arr[i];
            }


            // ==========================================================
            // counting sort
            if (forceLinearSort == 2 || (1.0 * sortArrayLength) / (arrMaxElement - arrMinElement) > 0.71) {
                if (ascendingOrder) countingSort_asc<ArrayValueType>(p_arr, low, high, arrMaxElement, arrMinElement);
                else countingSort_desc<ArrayValueType>(p_arr, low, high, arrMaxElement, arrMinElement);
                return;
            }


            // ==========================================================
            int_fast16_t onlyPositiveNumbers = 0;                       //  0 means +ve and -ve numbers
            if (arrMinElement > -1) onlyPositiveNumbers = 1;            //  1 means +ve numbers ONLY
            else if (arrMaxElement < 0) onlyPositiveNumbers = -1;       // -1 means -ve numbers ONLY

            int_fast16_t bits_arrMaxElement = ir_sort::integer_sort_common::countBits(
                    arrMaxElement), bits_arrMinElement = ir_sort::integer_sort_common::countBits(arrMinElement);

            // IMPORTANT
            // We use bits_arrMaxElement while calling the function, hence it is updated here
            // This is done so that the extra bit can be used to segregate -ve numbers from the +ve numbers
            // In the else if part we do not check if all the number are -ve of +ve because,
            //      if all numbers are +ve, then bits_arrMinElement <= bits_arrMaxElement
            //      if all numbers are -ve, then bits_arrMinElement >= bits_arrMaxElement

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
                            ir_sort::basic_sorts::merge_sort_basic_asc<ArrayPointerType, ArrayValueType>(p_arr, low, high);
                            return;
                        } else {
                            radixSort_asc<ArrayValueType, ArrayPointerType>(p_arr, low, high, sortArrayLength, bits_arrMaxElement);
                            return;
                        }
                    } else {
                        if (sortArrayLength <= ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                            ir_sort::basic_sorts::merge_sort_basic_desc<ArrayPointerType, ArrayValueType>(p_arr, low, high);
                            return;
                        } else {
                            radixSort_desc<ArrayValueType, ArrayPointerType>(p_arr, low, high, sortArrayLength, bits_arrMaxElement);
                            return;
                        }
                    }
                default:
                    // +ve numbers ONLY
                    // -ve numbers ONLY
                    if (ascendingOrder) {
                        if (sortArrayLength <= ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                            ir_sort::basic_sorts::merge_sort_basic_asc<ArrayPointerType, ArrayValueType>(p_arr, low, high);
                            return;
                        } else {
                            radixSort_Positive_asc<ArrayValueType, ArrayPointerType>(p_arr, low, high, sortArrayLength, bits_arrMaxElement);
                            return;
                        }
                    } else {
                        if (sortArrayLength <= ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                            ir_sort::basic_sorts::merge_sort_basic_desc<ArrayPointerType, ArrayValueType>(p_arr, low, high);
                            return;
                        } else {
                            radixSort_Positive_desc<ArrayValueType, ArrayPointerType>(p_arr, low, high, sortArrayLength, bits_arrMaxElement);
                            return;
                        }
                    }
            }
        }


        // integer_radix_sort
        // all the elements from arr[low] to arr[high] are sorted, both low and high inclusive
        // template<typename ArrayElementType, typename T>
        template<typename RandomAccessIterator>
        inline void
        integer_sort(RandomAccessIterator start_arr, std::random_access_iterator_tag) {
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
             *                :  3 is for radix_sort (without scanning for largest and smallest members)
             * */

            // NOTE: determine the type of elements stored in the array to be sorted, and it pointer type
            using ArrayValueType = typename std::iterator_traits<RandomAccessIterator>::value_type;
            using ArrayPointerType = ArrayValueType *; // this is pointer to an element of the array for which RandomAccessIterator is passed

            if (parameter::length <= 1360)
                ir_sort::basic_sorts::INSERTION_SORT_THRESHOLD = ir_sort::basic_sorts::THRESHOLD_INSERTION_FOR_MERGE[parameter::length];

            // ==========================================================
            if (parameter::force_linear_sort > 0) {
                // FORCE radix_sort or counting_sort
                // forceLinearSort == 1: uses radix_sort or counting_sort, whichever is better
                // forceLinearSort == 2: uses counting_sort only
                ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 0;
            } else if (parameter::force_linear_sort == -1) {
                // FORCE merge_sort and insertion dynamically
                if (parameter::ascending_order)
                    ir_sort::basic_sorts::merge_sort_basic_asc<RandomAccessIterator, ArrayValueType>(start_arr, 0, parameter::length - 1);
                else ir_sort::basic_sorts::merge_sort_basic_desc<RandomAccessIterator, ArrayValueType>(start_arr, 0, parameter::length - 1);
                return;
            } else if (parameter::force_linear_sort == -2) {
                // FORCE insertion_sort
                if (parameter::ascending_order)
                    ir_sort::basic_sorts::insertion_sort_basic_asc<RandomAccessIterator, ArrayValueType>(start_arr, 0, parameter::length - 1);
                else ir_sort::basic_sorts::insertion_sort_basic_desc<RandomAccessIterator, ArrayValueType>(start_arr, 0, parameter::length - 1);
                return;
            } else {
                // dynamically choose the sort
                // set merge_sort threshold based on size of ArrayValueType
                auto sizeOfArrayValueType = sizeof(ArrayValueType);
                if (sizeOfArrayValueType == 1) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 49;
                else if (sizeOfArrayValueType == 2) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 189;
                else if (sizeOfArrayValueType == 4) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 194;
                else if (sizeOfArrayValueType == 8) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 193;
                parameter::force_linear_sort = 0;    // if by any chance, forceLinearSort has any value less than -2 then it will be changed to 0
            }


            // ==========================================================
            // merge_sort
            if (parameter::length < ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                if (parameter::ascending_order)
                    ir_sort::basic_sorts::merge_sort_basic_asc<RandomAccessIterator, ArrayValueType>(start_arr, 0, parameter::length - 1);
                else ir_sort::basic_sorts::merge_sort_basic_desc<RandomAccessIterator, ArrayValueType>(start_arr, 0, parameter::length - 1);
                return;
            }


            // ==========================================================
            // arrMinElement: smallest element in "arr"
            // arrMaxElement: largest element in "arr"
            ArrayValueType arrMinElement, arrMaxElement;
            arrMinElement = arrMaxElement = start_arr[0];

            // this for loop is used to FIND THE MAXIMUM and MINIMUM of all the elements in the given array with the help of RandomAccessIterator
            if(parameter::force_linear_sort != 3){
                for (int_fast32_t i = 1; i < parameter::length; i++) {
                    if (start_arr[i] > arrMaxElement) arrMaxElement = start_arr[i];
                    else if (start_arr[i] < arrMinElement) arrMinElement = start_arr[i];
                }
            } else{
                if(ArrayValueType(-1) > ArrayValueType(0)){
                    // unsigned
                    arrMinElement = 0;
                }else{
                    // signed
                    arrMinElement = -1;
                }
                arrMaxElement = 1;
            }

            // ==========================================================
            // counting sort
            if (parameter::force_linear_sort == 2 || ((parameter::force_linear_sort != 3) && (1.0 * parameter::length) / (arrMaxElement - arrMinElement) > 0.71)) {
                if (parameter::ascending_order) countingSort_asc<ArrayValueType>(start_arr, 0, parameter::length - 1, arrMaxElement, arrMinElement);
                else countingSort_desc<ArrayValueType>(start_arr, 0, parameter::length - 1, arrMaxElement, arrMinElement);
                return;
            }


            // ==========================================================
            int_fast16_t onlyPositiveNumbers = 0;                       //  0 means +ve and -ve numbers
            if (arrMinElement > -1) onlyPositiveNumbers = 1;            //  1 means +ve numbers ONLY
            else if (arrMaxElement < 0) onlyPositiveNumbers = -1;       // -1 means -ve numbers ONLY
            parameter::signed_sorting = (onlyPositiveNumbers == 0);

            int_fast16_t bits_arrMaxElement = ir_sort::integer_sort_common::countBits(arrMaxElement),
                         bits_arrMinElement = ir_sort::integer_sort_common::countBits(arrMinElement);

            // IMPORTANT
            // We use bits_arrMaxElement while calling the function, hence it is updated here
            // This is done so that the extra bit can be used to segregate -ve numbers from the +ve numbers
            // In the else if part we do not check if all the number are -ve of +ve because,
            //      if all numbers are +ve, then bits_arrMinElement <= bits_arrMaxElement
            //      if all numbers are -ve, then bits_arrMinElement >= bits_arrMaxElement

            if (onlyPositiveNumbers == 0) {
                bits_arrMinElement++;
                if (bits_arrMinElement <= bits_arrMaxElement) bits_arrMaxElement++;
                else bits_arrMaxElement = bits_arrMinElement;
            } else if (bits_arrMinElement > bits_arrMaxElement) bits_arrMaxElement = bits_arrMinElement;

            if(parameter::force_linear_sort==3){
                bits_arrMaxElement = sizeof(ArrayValueType) << 3;
            }

            // if (parameter::force_linear_sort == 0 && parameter::length < 408) {
            //     if (onlyPositiveNumbers != 0) {
            //         // +ve and -ve numbers
            //         if (bits_arrMaxElement <= 8) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 49;
            //         else if (bits_arrMaxElement <= 16) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 77;
            //         else if (bits_arrMaxElement <= 24) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 134;
            //         else if (bits_arrMaxElement <= 32) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 165;
            //         else if (bits_arrMaxElement <= 40) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 245;
            //         else if (bits_arrMaxElement <= 48) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 302;
            //         else if (bits_arrMaxElement <= 56) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 407;
            //         else ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 455;
            //     } else {
            //         // +ve numbers ONLY
            //         if (bits_arrMaxElement <= 8) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 0;
            //         else if (bits_arrMaxElement <= 16) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 72;
            //         else if (bits_arrMaxElement <= 24) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 129;
            //         else if (bits_arrMaxElement <= 32) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 164;
            //         else if (bits_arrMaxElement <= 40) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 235;
            //         else if (bits_arrMaxElement <= 48) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 291;
            //         else if (bits_arrMaxElement <= 56) ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 401;
            //         else ir_sort::basic_sorts::MERGE_SORT_THRESHOLD = 457;
            //     }
            // }


            // ==========================================================
            if (parameter::ascending_order) {
                if (parameter::length <= ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                    ir_sort::basic_sorts::merge_sort_basic_asc<RandomAccessIterator, ArrayValueType>(start_arr, 0, parameter::length - 1);
                    return;
                } else {
                    radix_sort_asc<RandomAccessIterator, ArrayValueType>(start_arr, bits_arrMaxElement);
                    return;
                }
            } else {
                if (parameter::length <= ir_sort::basic_sorts::MERGE_SORT_THRESHOLD) {
                    ir_sort::basic_sorts::merge_sort_basic_desc<RandomAccessIterator, ArrayValueType>(start_arr, 0, parameter::length - 1);
                    return;
                } else {
                    radix_sort_desc<RandomAccessIterator, ArrayValueType>(start_arr, bits_arrMaxElement);
                    return;
                }
            }
        }
    }

    // this function call has parameters similar to std::sort
    template<typename RandomAccessIterator>
    inline void
    integer_sort(RandomAccessIterator first, RandomAccessIterator last, bool ascending_order = true, int_fast16_t force_linear_sort = 0) {
        // return if RandomAccessIterator is not a random_access_iterator
        if (typeid(typename std::iterator_traits<RandomAccessIterator>::iterator_category) != typeid(std::random_access_iterator_tag))
            return;

        // return if the given RandomAccessIterator is not for an integer
        if (!std::is_integral<typename std::iterator_traits<RandomAccessIterator>::value_type>::value)
            return;

        if (first >= last) return;

        stable_integer_sort_functions::integer_sort(first, 0, last - first - 1, ascending_order, force_linear_sort);
    }

    // this function call has parameters similar to std::sort
/**
 *  @brief Sort the elements of a sequence.
 *  @ingroup sorting_algorithms
 *  @param  first                   An iterator.
 *  @param  last                     Another iterator.
 *  @param  ascending_order         boolean.
 *  @param  force_linear_sort       int.
 *  @return  Nothing.
 *
 *  Sorts the elements in the range @p [start,end) in ascending order,
 *  such that for each iterator @e i in the range @p [start,end-1),
 *  *(i+1) < *i is false.
 *
 *  The relative ordering of equivalent elements is preserved.
 *  Valid only for Random Access Iterators.
 *
 *  Time complexity             : O(nk) .
 *  Auxilary space complexity   : O(n)  .
*/
    template<typename RandomAccessIterator>
    inline void
    stable_integer_sort_new(RandomAccessIterator first, RandomAccessIterator last, bool ascending_order = true, int_fast32_t force_linear_sort = 0) {
        // NOTE: this is not a good idea since user can still pass a iterator which is not a
        // RandomAccessIterator. To avoid this, we have passed the iterator category as a parameter
        // so that only those iterators can make a call
        // WORK: return if RandomAccessIterator is not a random_access_iterator
        // if (typeid(typename std::iterator_traits<RandomAccessIterator>::iterator_category) != typeid(std::random_access_iterator_tag))
        //     return;

        // WORK: return if the given RandomAccessIterator is not for an integer
        if (!std::is_integral<typename std::iterator_traits<RandomAccessIterator>::value_type>::value)
            return;

        if (first >= last) return;

        parameter::ascending_order = ascending_order;
        parameter::force_linear_sort = force_linear_sort;
        parameter::length = std::distance(first, last);
        stable_integer_sort_functions::integer_sort<RandomAccessIterator>(
                first,
                typename std::iterator_traits<RandomAccessIterator>::iterator_category()
        );
    }


//############################################################################
//                                                                          ##
//                       O B J E C T   S O R T I N G                        ##
//                                                                          ##
//############################################################################

//#######################################################################################
//                                                                                     ##
//      R A D I X   S O R T   -   A S C E N D I N G   O R D E R   -   O B J E C T      ##
//                                                                                     ##
//#######################################################################################

    // REFER the following StackOverflow question
    // https://stackoverflow.com/questions/13102277/sort-an-array-by-an-index-array-in-c

    template<typename ArrayElementType, typename T, typename myGetIndex>
    inline void
    radixSort_Positive_asc(T &arr, const int_fast64_t low, int_fast64_t high,
                           const int_fast64_t sortArrayLength, myGetIndex getIndex) {
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

        ir_sort::integer_sort_common::set_BitShift_Iterations(
                ir_sort::integer_sort_common::countBits_positive(arrMaxElement),
                bitsShift,
                iterationsRequired);

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


    template<typename ArrayElementType, typename T, typename myGetIndex>
    inline void
    radixSort_asc(T &arr, const int_fast64_t low, int_fast64_t high,
                  const int_fast64_t sortArrayLength, myGetIndex getIndex) {
        if (low >= high) return;

        // bool allPositive = true;
        // for(int_fast64_t i = low; i<= high; i++) {
        //     if(getIndex(arr[i]) < 0) {allPositive=false; break;}
        // }
        // if(allPositive) radixSort_Positive_asc<ArrayElementType, T, myGetIndex>(arr, low, high, sortArrayLength, getIndex);

        // using IndexType = std::remove_reference_t<std::remove_const_t<decltype(getIndex(arr[0]))>>;
        using IndexType = int32_t;
        if(high - low > (1ll << 32)) using IndexType = int64_t;

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

        ir_sort::integer_sort_common::set_BitShift_Iterations(
                ir_sort::integer_sort_common::countBits_positive(arrMaxElement),
                bitsShift,
                iterationsRequired);

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

        // ##### NEW START #####
        // Order of sorting till here: 0, 1, 2, 3, ..., -3, -2, -1
        int_fast64_t binarySearch_low = 0, binarySearch_high = high, binarySearch_mid;
        while (binarySearch_low <= binarySearch_high) {
            binarySearch_mid = (binarySearch_low + binarySearch_high) >> 1;
            if (getIndex(p_arr[p_arr_index[binarySearch_mid]]) < 0) binarySearch_high = binarySearch_mid - 1;
            else binarySearch_low = binarySearch_mid + 1;
        }

        // NOTE: though the below program me seem to be wrong for situations where all numbers are +ve or -ve
        // However, this function will only be called if there is atleast one -ve in all +ve OR one +ve in all -ve
        // binarySearch_low points to the first -ve number
        if (binarySearch_low <= high) while (binarySearch_low >= 0 && getIndex(p_arr[p_arr_index[binarySearch_low]]) < 0) binarySearch_low--;
        if (binarySearch_low >= 0) while (binarySearch_low <= high && getIndex(p_arr[p_arr_index[binarySearch_low]]) >= 0) binarySearch_low++;

        if (!(binarySearch_low == (-1) || binarySearch_low == (high + 1))) {
            int64_t j;
            std::swap(p_arr_index, p_copyArray_index);
            for (i = 0, j = binarySearch_low; j <= high;) p_arr_index[i++] = p_copyArray_index[j++];
            for (j = 0; j < binarySearch_low;) p_arr_index[i++] = p_copyArray_index[j++];
        }

        // ###### NEW END ######

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

} // namespace ir_sort

#endif //IR_SORT_CPP

