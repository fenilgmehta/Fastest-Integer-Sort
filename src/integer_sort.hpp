#ifndef IR_SORT_CPP
#define IR_SORT_CPP

// TESTING HEADERS START
// #include <iostream>
// #include <boost/sort/spreadsort/integer_sort.hpp>
// TESTING HEADERS END

#include <cstdint>
#include <numeric>
#include <vector>
// #include <iterator>

#include <boost/sort/pdqsort/pdqsort.hpp>

#include "ir_commons.hpp"

namespace ir_sort {
    namespace parameter {
        // signed_sorting      : this is true if both +ve and -ve numbers are present in the array to be sorted
        //                       (updation: refer function "ir_sort::stable_integer_sort_functions::integer_sort(RandomAccessIterator start_arr)")
        // force_linear_sort   : this will decide the type of sorting to be used
        //                       (updation: refer "ir_sort::integer_sort_stable")
        // length              : length of the array to be sorted
        //                       (updation: refer "ir_sort::integer_sort_stable")
        bool signed_sorting;
        int_fast32_t force_linear_sort = 0;
        int_fast32_t length;

        // box        : used to store the count of occurrences of a number in the column being scanned for sorting
        //              (updation: refer "ir_sort::parameter::reset_box")
        int_fast32_t box1[256];
        int_fast32_t box2[256];
        int_fast32_t box3[256];
        int_fast32_t box4[256];
        int_fast32_t box5[256];
        int_fast32_t box6[256];
        int_fast32_t box7[256];
        int_fast32_t box8[256];

        // initial_bits_shift : the initial bits to be skipped or shifted and then use next "min_bits_shift" for sorting
        //                      (updation: refer "ir_sort::parameter::set_BitShift_Iterations")
        //                      (updation: refer "ir_sort::parameter::update_initial_bits_shift")
        //                      (NOTE: updated regularly)
        // iterations_required : number of columns of size 8 bits to be sorted
        //                       (updation: refer "ir_sort::parameter::set_BitShift_Iterations")
        int_fast32_t initial_bits_shift;
        int_fast32_t iterations_required;

        /*
         * This will fill "box" with 0
         * Default value of all the elements of the array "box" = 0
         *
         * */
        inline void reset_box1() {
            for (auto &i: box1) i = 0;
        }

        inline void reset_box2() {
            for (int i = 0; i < 256; ++i) box1[i] = box2[i] = 0;
        }

        inline void reset_box4() {
            for (int i = 0; i < 256; ++i) box1[i] = box2[i] = box3[i] = box4[i] = 0;
        }

        inline void reset_box6() {
            for (int i = 0; i < 256; ++i) box1[i] = box2[i] = box3[i] = box4[i] = box5[i] = box6[i] = 0;
        }

        inline void reset_box8() {
            for (int i = 0; i < 256; ++i)
                box1[i] = box2[i] = box3[i] = box4[i] = box5[i] = box6[i] = box7[i] = box8[i] = 0;
        }

        /*
         * Updates the following variables:
         * 1. initial_bits_shift
         *
         * */
        inline void update_initial_bits_shift(const int increment_value) {
            initial_bits_shift += increment_value;
        }

        /*
         * Updates the following variables:
         * 1. initial_bits_shift
         * 2. iterations_required
         *
         * */
        inline void set_BitShift_Iterations(const int_fast32_t arr_max_bits) {
            initial_bits_shift = 0;
            iterations_required = (arr_max_bits >> 3) + ((arr_max_bits & 7) != 0);
        }
    };

    namespace key_functions {
        int_fast32_t size;

        template<typename T>
        inline void set_size() {
            size = sizeof(T);
        }

        template<typename T>
        inline T get_unsigned_key(const T &key) {
            return key;
        }

        template<typename T>
        inline T get_signed_key(const T &key) {
            return (key ^ (128 << ((size - 1) << 3)));
        }
    }

    namespace stable_integer_sort_functions {

//############################################################################
//                                                                          ##
//                        C O U N T I N G   S O R T                         ##
//                                                                          ##
//############################################################################

        template<typename RandomAccessIterator, typename ArrayValueType>
        inline void
        countingSort_asc(RandomAccessIterator first_arr, const RandomAccessIterator last_arr,
                         const ArrayValueType arrMinElement, const ArrayValueType arrMaxElement) {
            // NOTE: This is the modified version created by me so that it works for negative numbers too.
            //       This sorting algorithm is not valid for negative numbers, however I have modified it
            //       so that it works for -ve numbers also.

            // arrMaxElement: largest element in the array to be sorted. Can be +ve or -ve.
            // arrMinElement: smallest element in the array to be sorted. Can be +ve or -ve.

            unsigned long boxLength = arrMaxElement - arrMinElement + 1;

            // the "()" is used create new array and make all the elements of the array to 0
            // auto box = new uint32_t[boxLength]();
            std::vector<uint32_t> box(boxLength);

            // This for loop is used to increment the value of the i positioned element of the
            // counting array(+ve and -ve) to say that the element is present in that quantity
            for (auto key = first_arr; key != last_arr; ++key) {
                ++box[(*key) - arrMinElement];
            }

            // i: used for iterations
            // j: used for the index which is to be used to store the next sorted element
            // int_fast64_t i, j;
            // for (i = 0, j = 0; i < boxLength; ++i) {
            //     while (box[i]-- > 0) first_arr[j++] = i + arrMinElement;
            // }
            for (int_fast64_t i = 0; i < boxLength; ++i) {
                while (box[i]-- > 0) (*first_arr++) = i + arrMinElement;
            }

            // delete[] box;
        }

//############################################################################
//                                                                          ##
//                     R A D I X   S O R T   -   N E W                      ##
//                                                                          ##
//############################################################################

        template<typename RandomAccessIterator, typename Buffer>
        static void
        partial_radix_sort_asc8(RandomAccessIterator start_arr, Buffer start_buffer) {
            parameter::reset_box8();

            // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
            for (int_fast32_t i = 0; i < parameter::length; ++i) {
                auto key = start_arr[i] >> parameter::initial_bits_shift;
                ++parameter::box1[(key) & 255];
                ++parameter::box2[(key >> 8) & 255];
                ++parameter::box3[(key >> 16) & 255];
                ++parameter::box4[(key >> 24) & 255];
                ++parameter::box5[(key >> 32) & 255];
                ++parameter::box6[(key >> 40) & 255];
                ++parameter::box7[(key >> 48) & 255];
                ++parameter::box8[(key >> 56) & 255];
            }

            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (int_fast32_t i = 1; i < 256; ++i) {
                parameter::box1[i] += parameter::box1[i - 1];
                parameter::box2[i] += parameter::box2[i - 1];
                parameter::box3[i] += parameter::box3[i - 1];
                parameter::box4[i] += parameter::box4[i - 1];
                parameter::box5[i] += parameter::box5[i - 1];
                parameter::box6[i] += parameter::box6[i - 1];
                parameter::box7[i] += parameter::box7[i - 1];
                parameter::box8[i] += parameter::box8[i - 1];
            }

            // this for loop is used to insert the elements of "start_arr" into "start_buffer" in sorted
            // order of the column being scanned of the original array
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_buffer[--parameter::box1[(start_arr[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_arr[i]);
            }
            parameter::update_initial_bits_shift(8);
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_arr[--parameter::box2[(start_buffer[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_buffer[i]);
            }
            parameter::update_initial_bits_shift(8);
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_buffer[--parameter::box3[(start_arr[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_arr[i]);
            }
            parameter::update_initial_bits_shift(8);
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_arr[--parameter::box4[(start_buffer[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_buffer[i]);
            }
            parameter::update_initial_bits_shift(8);
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_buffer[--parameter::box5[(start_arr[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_arr[i]);
            }
            parameter::update_initial_bits_shift(8);
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_arr[--parameter::box6[(start_buffer[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_buffer[i]);
            }
            parameter::update_initial_bits_shift(8);
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_buffer[--parameter::box7[(start_arr[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_arr[i]);
            }
            parameter::update_initial_bits_shift(8);
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_arr[--parameter::box8[(start_buffer[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_buffer[i]);
            }
            parameter::update_initial_bits_shift(8);
        }

        template<typename RandomAccessIterator, typename Buffer>
        static void
        partial_radix_sort_asc4(RandomAccessIterator start_arr, Buffer start_buffer) {
            parameter::reset_box4();

            // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
            for (int_fast32_t i = 0; i < parameter::length; ++i) {
                auto key = start_arr[i] >> parameter::initial_bits_shift;
                ++parameter::box1[(key) & 255];
                ++parameter::box2[(key >> 8) & 255];
                ++parameter::box3[(key >> 16) & 255];
                ++parameter::box4[(key >> 24) & 255];
            }

            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (int_fast32_t i = 1; i < 256; ++i) {
                parameter::box1[i] += parameter::box1[i - 1];
                parameter::box2[i] += parameter::box2[i - 1];
                parameter::box3[i] += parameter::box3[i - 1];
                parameter::box4[i] += parameter::box4[i - 1];
            }

            // this for loop is used to insert the elements of "start_arr" into "start_buffer" in sorted
            // order of the column being scanned of the original array
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_buffer[--parameter::box1[(start_arr[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_arr[i]);
            }
            parameter::update_initial_bits_shift(8);
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_arr[--parameter::box2[(start_buffer[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_buffer[i]);
            }
            parameter::update_initial_bits_shift(8);
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_buffer[--parameter::box3[(start_arr[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_arr[i]);
            }
            parameter::update_initial_bits_shift(8);
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_arr[--parameter::box4[(start_buffer[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_buffer[i]);
            }
            parameter::update_initial_bits_shift(8);
        }

        template<typename RandomAccessIterator, typename Buffer>
        static void
        partial_radix_sort_asc2(RandomAccessIterator start_arr, Buffer start_buffer) {
            parameter::reset_box2();

            // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
            for (int_fast32_t i = 0; i < parameter::length; ++i) {
                auto key = start_arr[i] >> parameter::initial_bits_shift;
                ++parameter::box1[(key) & 255];
                ++parameter::box2[(key >> 8) & 255];
            }

            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (int_fast32_t i = 1; i < 256; ++i) {
                parameter::box1[i] += parameter::box1[i - 1];
                parameter::box2[i] += parameter::box2[i - 1];
            }

            // this for loop is used to insert the elements of "start_arr" into "start_buffer" in sorted
            // order of the column being scanned of the original array
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_buffer[--parameter::box1[(start_arr[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_arr[i]);
            }
            parameter::update_initial_bits_shift(8);
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_arr[--parameter::box2[(start_buffer[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_buffer[i]);
            }
            parameter::update_initial_bits_shift(8);

        }

        template<typename RandomAccessIterator, typename Buffer>
        inline void
        partial_radix_sort_asc1(RandomAccessIterator start_arr, Buffer start_buffer) {
            parameter::reset_box1();

            // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
            for (int_fast32_t i = 0; i < parameter::length; ++i) {
                ++parameter::box1[(start_arr[i] >> parameter::initial_bits_shift) & 255];
            }

            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (int_fast32_t i = 1; i < 256; ++i) {
                parameter::box1[i] += parameter::box1[i - 1];
            }

            // this for loop is used to insert the elements of "start_arr" into "start_buffer" in sorted
            // order of the column being scanned of the original array
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_buffer[--parameter::box1[(start_arr[i] >> parameter::initial_bits_shift) & 255]] = std::move(
                        start_arr[i]);
            }

            parameter::update_initial_bits_shift(8);
        }

        // MODIFIED: only added the XOR for the sign bit
        template<typename RandomAccessIterator, typename Buffer>
        inline void
        partial_radix_sort_asc_flip_bit(RandomAccessIterator start_arr, Buffer start_buffer) {
            parameter::reset_box1();

            // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column being scanned
            for (int_fast32_t i = 0; i < parameter::length; ++i) {
                ++parameter::box1[((start_arr[i] >> parameter::initial_bits_shift) ^ 128) & 255];
            }

            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (int_fast32_t i = 1; i < 256; ++i) {
                parameter::box1[i] += parameter::box1[i - 1];
            }

            // this for loop is used to insert the elements of "start_arr" into "start_buffer" in sorted
            // order of the column being scanned of the original array
            for (int_fast32_t i = parameter::length - 1; i >= 0; --i) {
                start_buffer[--parameter::box1[((start_arr[i] >> parameter::initial_bits_shift) ^ 128) &
                                               255]] = std::move(start_arr[i]);
            }

            parameter::update_initial_bits_shift(8);
        }

        template<typename RandomAccessIterator, typename ArrayValueType, typename Buffer>
        inline void
        radix_sort_asc_signed(RandomAccessIterator start_arr, Buffer buffer) {
            key_functions::set_size<ArrayValueType>();

            int_fast32_t i = 1;
            while ((i + 8) <= parameter::iterations_required) {
                partial_radix_sort_asc8(start_arr, buffer);
                i += 8;
            }
            while ((i + 4) <= parameter::iterations_required) {
                partial_radix_sort_asc4(start_arr, buffer);
                i += 4;
            }
            while ((i + 2) <= parameter::iterations_required) {
                partial_radix_sort_asc2(start_arr, buffer);
                i += 2;
            }
            while (i < parameter::iterations_required) {
                partial_radix_sort_asc1(start_arr, buffer);
                ++i;

                if (i == parameter::iterations_required) break;
                partial_radix_sort_asc1(buffer, start_arr);
                ++i;
            }

            // the array is a mixture of +ve and -ve numbers, then use descending order
            // for last column which contains the bit which decides the sign of a number

            switch (parameter::iterations_required % 2) {
                case 0: // even length
                    if (parameter::signed_sorting)
                        partial_radix_sort_asc_flip_bit(buffer, start_arr);
                    else
                        partial_radix_sort_asc1(buffer, start_arr);
                    break;
                case 1: // odd length
                    if (parameter::signed_sorting)
                        partial_radix_sort_asc_flip_bit(start_arr, buffer);
                    else
                        partial_radix_sort_asc1(start_arr, buffer);

                    // the sorted arrays is copied into the original input array if parameter::length is odd
                    // for (i = 0; i < parameter::length; ++i) {
                    //     start_arr[i] = buffer[i];
                    // }
                    std::copy(buffer, std::next(buffer, parameter::length), start_arr);
            }
        }

        template<typename RandomAccessIterator, typename ArrayValueType, typename Buffer>
        inline void
        radix_sort_asc_unsigned(RandomAccessIterator start_arr, Buffer buffer) {
            key_functions::set_size<ArrayValueType>();

            int_fast32_t i = 0;
            while ((i + 8) <= parameter::iterations_required) {
                partial_radix_sort_asc8(start_arr, buffer);
                i += 8;
            }
            while ((i + 4) <= parameter::iterations_required) {
                partial_radix_sort_asc4(start_arr, buffer);
                i += 4;
            }
            while ((i + 2) <= parameter::iterations_required) {
                partial_radix_sort_asc2(start_arr, buffer);
                i += 2;
            }
            while (i < parameter::iterations_required) {
                partial_radix_sort_asc1(start_arr, buffer);
                ++i;

                if (i == parameter::iterations_required) break;
                partial_radix_sort_asc1(buffer, start_arr);
                ++i;
            }

            // the array is a mixture of +ve and -ve numbers, then use descending order
            // for last column which contains the bit which decides the sign of a number

            if (parameter::iterations_required % 2) {
                std::copy(buffer, std::next(buffer, parameter::length), start_arr);
            }
        }

//############################################################################
//                                                                          ##
//                         I N T E G E R   S O R T                          ##
//                                                                          ##
//############################################################################

        /* ### PARAMETERS
         * arr: the array to be sorted(values from [low, high] will be sorted)
         * low: the lower index for sorting
         * high: the higher index for sorting(the element at index high will also be considered for sorting)
         * ascendingOrder: this is used to decide whether the user wants the array to be sorted in ascending order or descending order
         * forceLinearSort: this has 4 possible values(-1, 0, 1, 2)
         *                :  0 is for dynamic selection of the sorting technique
         *                :  1 is for radix_sort (without scanning for largest and smallest members)
         * */
        template<typename RandomAccessIterator>
        inline void
        integer_sort(RandomAccessIterator first_arr, RandomAccessIterator last_arr, std::random_access_iterator_tag) {
            // NOTE: determine the type of elements stored in the array to be sorted, and it pointer type
            using ArrayValueType = typename std::iterator_traits<RandomAccessIterator>::value_type;

            // ==========================================================
            // use pdqsort if length <= 512
            if (parameter::length < 512) {
                boost::sort::pdqsort<RandomAccessIterator>(first_arr, last_arr);
                return;
            }

            // ==========================================================
            // arrMinElement: smallest element in "arr"
            // arrMaxElement: largest element in "arr"
            ArrayValueType arrMinElement, arrMaxElement;
            arrMinElement = arrMaxElement = first_arr[0];

            /*
             * Condition 1: the parameter passed if 1 for force_linear_sort
             * Condition 2: Take largest of the following three number, i.e. the first, middle and last number.
             *              If the largest number if >= 72057594037927935, implies that bits_arrMaxElement > 56.
             *              Hence it is better to skip the for loop which is used to find arrMinElement and arrMaxElement
             * */
            if (parameter::force_linear_sort == 1 ||
                (std::max(first_arr[0],
                          std::max(first_arr[parameter::length >> 1], first_arr[parameter::length - 1])) >=
                 72057594037927936)) {
                parameter::force_linear_sort = 1; // this statement is required if the condition 2 is true
                arrMaxElement = 1;
                arrMinElement = (ArrayValueType(-1) > ArrayValueType(0)) ? 0 : -1;
            } else {
                // this for loop is used to FIND THE MAXIMUM and MINIMUM of all the elements in the given array with the help of RandomAccessIterator
                for (int_fast32_t i = 1; i < parameter::length; ++i) {
                    arrMaxElement = (first_arr[i] > arrMaxElement) ? first_arr[i] : arrMaxElement;
                    arrMinElement = (first_arr[i] < arrMinElement) ? first_arr[i] : arrMinElement;
                }

                // counting sort
                if (((1.0 * parameter::length) / (arrMaxElement - arrMinElement)) > 0.71) {
                    countingSort_asc<RandomAccessIterator, ArrayValueType>(first_arr, last_arr, arrMinElement,
                                                                           arrMaxElement);
                    return;
                }
            }

            // ==========================================================
            int_fast16_t onlyPositiveNumbers = 0;                       //  0 means +ve and -ve numbers
            if (arrMinElement >= 0) onlyPositiveNumbers = 1;            //  1 means +ve numbers ONLY
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
                ++bits_arrMinElement;
                if (bits_arrMinElement <= bits_arrMaxElement) ++bits_arrMaxElement;
                else bits_arrMaxElement = bits_arrMinElement;
            } else if (bits_arrMinElement == bits_arrMaxElement) ++bits_arrMaxElement;
            else if (bits_arrMinElement > bits_arrMaxElement) bits_arrMaxElement = bits_arrMinElement;

            if (parameter::force_linear_sort == 1) {
                bits_arrMaxElement = sizeof(ArrayValueType) << 3;
            }

            // ==========================================================
            // this dynamic array is used to store the partially sorted array
            // FAST as compared to vector<ArrayValueType>
            // RELIABLE as compared the array on stack created using ArrayValueType buffer[parameter::length]
            auto buffer = new ArrayValueType[parameter::length];

            parameter::set_BitShift_Iterations(bits_arrMaxElement);

            if (parameter::signed_sorting)
                radix_sort_asc_signed<RandomAccessIterator, ArrayValueType, ArrayValueType *>(first_arr, buffer);
            else radix_sort_asc_unsigned<RandomAccessIterator, ArrayValueType, ArrayValueType *>(first_arr, buffer);

            delete[] buffer;
        }
    }

    /**
     *  @brief Sort the elements of a sequence.
     *  @ingroup sorting_algorithms
     *  @param  first                   An iterator.
     *  @param  last                    Another iterator.
     *  @param  force_linear_sort       int.
     *  @return  Nothing.
     *
     *  Sorts the elements in the range @p [first,last) in ascending order,
     *  such that for each iterator @e i in the range @p [first,last-1),
     *  *(i+1) < *i is false.
     *
     *  The relative ordering of equivalent elements is preserved.
     *  Valid only for Random Access Iterators.
     *
     *  Time complexity             : O(nk) .
     *  Auxiliary space complexity  : O(n)  .
    */
    template<typename RandomAccessIterator>
    inline void
    integer_sort_stable(RandomAccessIterator first, RandomAccessIterator last, int_fast32_t force_linear_sort = 0) {
        // WORK: return if RandomAccessIterator is not a random_access_iterator
        if (typeid(typename std::iterator_traits<RandomAccessIterator>::iterator_category) !=
            typeid(std::random_access_iterator_tag))
            return;

        // WORK: return if the given RandomAccessIterator is not for an integer
        if (!std::is_integral<typename std::iterator_traits<RandomAccessIterator>::value_type>::value)
            return;

        if (first >= last) return;

        parameter::force_linear_sort = force_linear_sort;
        parameter::length = std::distance(first, last);
        stable_integer_sort_functions::integer_sort<RandomAccessIterator>(
                first,
                last,
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
        int_fast64_t arrMaxElement = getIndex(
                arr[high]);       // "arrMaxElement" is used to store the largest element in "arr"
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
        for (i = 0; i < high; ++i) {
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
        // for (i = 0; i <= high; ++i) arr_index[i] = i;
        std::iota(&arr_index[0], &arr_index[high] + 1, 0);

        // START ACTUAL SORTING
        // This condition is used so that all the digits of each element are checked and "arr" is sorted according to the digit considered
        while (iterationsRequired-- > 0) {
            // default values of all the elements of the array "box" = 0
            for (i = 0; i < boxArraySize;) {
                box[i++] = 0;
            }

            // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column
            for (i = 0; i <= high; ++i) {
                box[(getIndex(p_arr[i]) >> bitsShiftFactor) & modNum]++;
            }

            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (i = 1; i < boxArraySize; ++i) {
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
            /*for (i = high; i >= 0; --i) {
                p_arr_index[--box[((getIndex(p_arr[p_copyArray_index[i]]) >> bitsShiftFactor) & modNum)]] = p_copyArray_index[i];
            }*/
            if (iterationsRequired == 0) {
                // this section will run for the last iteration over the input array
                // this block will fill the array "indexTracker"
                uint_fast64_t positionFor_p_index_arr;
                for (i = high; i >= 0; --i) {
                    positionFor_p_index_arr = --box[((getIndex(p_arr[p_copyArray_index[i]]) >> bitsShiftFactor) &
                                                     modNum)];
                    p_arr_index[positionFor_p_index_arr] = p_copyArray_index[i];
                    indexTracker[p_copyArray_index[i]] = positionFor_p_index_arr;
                }
            } else {
                for (i = high; i >= 0; --i) {
                    p_arr_index[--box[((getIndex(p_arr[p_copyArray_index[i]]) >> bitsShiftFactor) &
                                       modNum)]] = p_copyArray_index[i];
                }
            }

            // this operation is done so that we move to the next column of array elements
            bitsShiftFactor += bitsShift;
        }

        // now output of "p_arr_index" = numpy.argsort(arr)
        // DONE: write loop to sort the array using "p_arr_index"
        // DONE: refer the following link https://stackoverflow.com/questions/13102277/sort-an-array-by-an-index-array-in-c
        for (i = 0; i <= high; ++i) {
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
        // for(int_fast64_t i = low; i<= high; ++i) {
        //     if(getIndex(arr[i]) < 0) {allPositive=false; break;}
        // }
        // if(allPositive) radixSort_Positive_asc<ArrayElementType, T, myGetIndex>(arr, low, high, sortArrayLength, getIndex);

        // using IndexType = std::remove_reference_t<std::remove_const_t<decltype(getIndex(arr[0]))>>;
        using IndexType = int32_t;
        if (high - low > (1ll << 32)) using IndexType = int64_t;

        // SUBORDINATE VARIABLES
        int_fast64_t arrMaxElement = getIndex(
                arr[high]);       // "arrMaxElement" is used to store the largest element in "arr"
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
        for (i = 0; i < high; ++i) {
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
        // for (i = 0; i <= high; ++i) arr_index[i] = i;
        std::iota(&arr_index[0], &arr_index[high] + 1, 0);

        // START ACTUAL SORTING
        // This condition is used so that all the digits of each element are checked and "arr" is sorted according to the digit considered
        while (iterationsRequired-- > 0) {
            // default values of all the elements of the array "box" = 0
            for (i = 0; i < boxArraySize;) {
                box[i++] = 0;
            }

            // this for loop will note number of zeros, ones, twos ... "boxArraySize - 1" present in the column
            for (i = 0; i <= high; ++i) {
                box[(getIndex(p_arr[i]) >> bitsShiftFactor) & modNum]++;
            }

            // this for loop is used to add the values of the previous element to the next element of the array "box"
            for (i = 1; i < boxArraySize; ++i) {
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
            /*for (i = high; i >= 0; --i) {
                p_arr_index[--box[((getIndex(p_arr[p_copyArray_index[i]]) >> bitsShiftFactor) & modNum)]] = p_copyArray_index[i];
            }*/
            if (iterationsRequired == 0) {
                // this section will run for the last iteration over the input array
                // this block will fill the array "indexTracker"
                uint_fast64_t positionFor_p_index_arr;
                for (i = high; i >= 0; --i) {
                    positionFor_p_index_arr = --box[((getIndex(p_arr[p_copyArray_index[i]]) >> bitsShiftFactor) &
                                                     modNum)];
                    p_arr_index[positionFor_p_index_arr] = p_copyArray_index[i];
                    indexTracker[p_copyArray_index[i]] = positionFor_p_index_arr;
                }
            } else {
                for (i = high; i >= 0; --i) {
                    p_arr_index[--box[((getIndex(p_arr[p_copyArray_index[i]]) >> bitsShiftFactor) &
                                       modNum)]] = p_copyArray_index[i];
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
        if (binarySearch_low <= high)
            while (binarySearch_low >= 0 && getIndex(p_arr[p_arr_index[binarySearch_low]]) < 0)--binarySearch_low;
        if (binarySearch_low >= 0)
            while (binarySearch_low <= high && getIndex(p_arr[p_arr_index[binarySearch_low]]) >= 0)++binarySearch_low;

        if (!(binarySearch_low == (-1) || binarySearch_low == (high + 1))) {
            int64_t j;
            std::swap(p_arr_index, p_copyArray_index);
            for (i = 0, j = binarySearch_low; j <= high;) p_arr_index[i++] = p_copyArray_index[j++];
            for (j = 0; j < binarySearch_low;) p_arr_index[i++] = p_copyArray_index[j++];
        }

        // ###### NEW END ######

        // std::cout<<"\nDEBUG: ";
        // for(int i1i1 = 0; i1i1 < sortArrayLength; i1i1++) std::cout<<p_arr_index[i1i1]<<", "; std::cout<<std::endl;
        // for(int i1i1 = 0; i1i1 < sortArrayLength; i1i1++) std::cout<<indexTracker[i1i1]<<", "; std::cout<<std::endl;
        // for(int i1i1 = 0; i1i1 < sortArrayLength; i1i1++) std::cout<<arr[i1i1]<<", "; std::cout<<std::endl;

        // now output of "p_arr_index" = numpy.argsort(arr)
        // DONE: write loop to sort the array using "p_arr_index"
        // DONE: refer the following link https://stackoverflow.com/questions/13102277/sort-an-array-by-an-index-array-in-c
        for (i = 0; i <= high; ++i) {
            std::swap(p_arr[i], p_arr[p_arr_index[i]]);
            p_arr_index[indexTracker[i]] = p_arr_index[i];
            indexTracker[p_arr_index[i]] = indexTracker[i];
        }

        delete[] indexTracker;
        delete[] arr_index;
        delete[] copyArray_index;
    }

} // namespace ir_sort


namespace fm_sort {

    template<typename RandomAccessIterator, typename RandomAccessIter2>
    void fm_sort_objects(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIter2 p_arr_index) {
        using IntegerIndexType = typename std::iterator_traits<RandomAccessIter2>::value_type;

        auto length = std::distance(first, last);
        std::vector<IntegerIndexType> index_tracker(static_cast<unsigned long>(length));
        for (int_fast64_t i = 0; i < length; ++i) {
            index_tracker[p_arr_index[i]] = i;
        }

        // std::cout<<"\nDEBUG fm_sort:\n";
        // std::cout<<"\t"; for(int_fast64_t i = 0; i < length; ++i) std::cout<<p_arr_index[i]<<", "; std::cout<<std::endl;
        // std::cout<<"\t"; for(int_fast64_t i = 0; i < length; ++i) std::cout<<index_tracker[i]<<", "; std::cout<<std::endl;

        // now output of "[b_first, b_last)" = numpy.argsort(arr)
        // DONE: write loop to sort the array using "[b_first, b_last)"
        // DONE: refer the following link https://stackoverflow.com/questions/13102277/sort-an-array-by-an-index-array-in-c
        for (int_fast64_t i = 0; i < length; ++i) {
            if (i != p_arr_index[i]) {
                std::swap(first[i], first[p_arr_index[i]]);
                index_tracker[p_arr_index[i]] = index_tracker[i];
            }
            p_arr_index[index_tracker[i]] = p_arr_index[i];
        }

        // std::vector<int64_t> p_arr_index(length+1), indexTracker(length+1);
        // auto b_first = &p_arr_index[0], b_last = &p_arr_index[length];
        //
        // std::iota(b_first, b_last, static_cast<int64_t>(0));
        //
        // // main work
        // boost::sort::spreadsort::integer_sort(b_first, b_last, [first, comp](const int64_t& val1, const ComparatorParameter& val2){return comp(first[val1], val2);});
        // // boost::sort::spreadsort::integer_sort(first, last, comp);
        //
        // for(int64_t i = 0; i < length; ++i) {
        //     indexTracker[p_arr_index[i]] = i;
        // }
    }

// const int_fast32_t COMPARISON_SORT_THRESHOLD = 1024;
#define COMPARISON_SORT_THRESHOLD 10000
#define LINEAR_SORT_THRESHOLD 10000

    /*
     * byteNumber: this varies from 8 to 1 for uint64_t, it denotes the byte to scan
     * */

    uint64_t sub_array_start_pos[256];

    // TODO: maybe not working
    template<typename RandomAccessIterator, typename ArrayValueType, typename Buffer>
    void fm_sort_unsigned(const RandomAccessIterator first, const RandomAccessIterator last, int_fast32_t byteNumber,
                          const Buffer buffer) {
        uint64_t box[256], sub_array_start[256], sub_array_last[256];
        int_fast32_t bitsShiftRight = (byteNumber - 1) * 8;

        for (auto &i: box) i = 0;
        for (RandomAccessIterator temp_first = first; temp_first != last; ++temp_first) {
            ++box[((*temp_first) >> bitsShiftRight) & 255];
        }

        // std::partial_sum(std::begin(box), std::end(box), std::begin(sub_array_last));
        sub_array_start[0] = sub_array_start_pos[0] = 0, sub_array_last[0] = sub_array_start[1] = sub_array_start_pos[1] = box[0];
        for (int_fast32_t i = 2; i < 256; ++i) {
            sub_array_start[i] = sub_array_start_pos[i] = sub_array_last[i - 1] = (box[i - 1] + sub_array_last[i - 2]);
        }
        sub_array_last[255] = (box[255] +
                               sub_array_last[254]); // SAME as: sub_array_last[255] = (box[255] + sub_array_start[255]);

        // TODO: inplace sorting
        auto temp = (first[0]);
        for (int_fast32_t i = 0; i < 256; ++i) {
            for (int_fast32_t j = sub_array_start_pos[i]; j < sub_array_last[i]; ++j) {
                auto temp_masked = (first[j] >> bitsShiftRight) & 255;
                while ((temp_masked = (first[j] >> bitsShiftRight) & 255) != i) {
                    // if the number belongs to the same sub-array, then we stop.
                    // else
                    //      it is moved to the new place and repeat this procedure
                    //      for all the numbers which are being replaced/overwritten

                    std::swap(first[j], first[sub_array_start_pos[temp_masked]]);
                    ++sub_array_start_pos[temp_masked];
                }
            }
        }

        --byteNumber;
        for (int_fast32_t i = 0; i < 256; ++i) {
            if (box[i] <= LINEAR_SORT_THRESHOLD) {
                // ir_sort::basic_sorts::merge_sort(std::next(first, sub_array_start[i]), std::next(first, sub_array_last[i]));
                // boost::sort::pdqsort(std::next(first, sub_array_start[i]), std::next(first, sub_array_last[i]));
                // boost::sort::spreadsort::integer_sort(std::next(first, sub_array_start[i]), std::next(first, sub_array_last[i]));
                ir_sort::parameter::iterations_required = byteNumber;
                ir_sort::parameter::length = box[i];
                ir_sort::stable_integer_sort_functions::radix_sort_asc_unsigned<RandomAccessIterator, ArrayValueType, Buffer>(
                        std::next(first, sub_array_start[i]), buffer);
            } else {
                fm_sort_unsigned<RandomAccessIterator, ArrayValueType, Buffer>
                        (std::next(first, sub_array_start[i]), std::next(first, sub_array_last[i]), byteNumber, buffer);
            }
        }
    }

    template<typename RandomAccessIterator>
    inline void fm_sort(const RandomAccessIterator first, const RandomAccessIterator last) {
        if (std::distance(first, last) <= COMPARISON_SORT_THRESHOLD) {
            // ir_sort::basic_sorts::merge_sort(first, last);
            // boost::sort::spreadsort::integer_sort(first, last);
            boost::sort::pdqsort(first, last);
            return;
        }

        // ==========================================================
        using ArrayValueType = typename std::iterator_traits<RandomAccessIterator>::value_type;

        // arrMinElement: smallest element in "arr"
        // arrMaxElement: largest element in "arr"
        ArrayValueType arrMinElement, arrMaxElement;
        arrMinElement = arrMaxElement = first[0];

        /*
         * Condition 1: the parameter passed if 1 for force_linear_sort
         * Condition 2: Take largest of the following three number, i.e. the first, middle and last number.
         *              If the largest number if >= 72057594037927935, implies that bits_arrMaxElement > 56.
         *              Hence it is better to skip the for loop which is used to find arrMinElement and arrMaxElement
         * NOTE: math.log2(72057594037927936) = 56
         * */
        using namespace ir_sort;
        if (parameter::force_linear_sort == 1 ||
            (std::max(first[0], std::max(first[parameter::length >> 1], first[parameter::length - 1])) >=
             72057594037927936)) {
            parameter::force_linear_sort = 1; // this statement is required if the condition 2 is true
            arrMaxElement = 1;
            arrMinElement = (ArrayValueType(-1) > ArrayValueType(0)) ? 0 : -1;
        } else {
            // this for loop is used to FIND THE MAXIMUM and MINIMUM of all the elements in the given array with the help of RandomAccessIterator
            for (int_fast32_t i = 1; i < parameter::length; ++i) {
                arrMaxElement = (first[i] > arrMaxElement) ? first[i] : arrMaxElement;
                arrMinElement = (first[i] < arrMinElement) ? first[i] : arrMinElement;
            }

            // TODO: uncomment this
            // counting sort
            // if (((1.0 * parameter::length) / (arrMaxElement - arrMinElement)) > 0.71) {
            //     ir_sort::stable_integer_sort_functions::countingSort_asc
            //             <RandomAccessIterator, ArrayValueType>(first, last, arrMinElement, arrMaxElement);
            //     return;
            // }
        }

        // ==========================================================
        int_fast16_t onlyPositiveNumbers = 0;                       //  0 means +ve and -ve numbers
        if (arrMinElement >= 0) onlyPositiveNumbers = 1;            //  1 means +ve numbers ONLY
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
            ++bits_arrMinElement;
            if (bits_arrMinElement <= bits_arrMaxElement) ++bits_arrMaxElement;
            else bits_arrMaxElement = bits_arrMinElement;
        } else if (bits_arrMinElement == bits_arrMaxElement) ++bits_arrMaxElement;
        else if (bits_arrMinElement > bits_arrMaxElement) bits_arrMaxElement = bits_arrMinElement;

        auto byteNumber = (parameter::force_linear_sort == 1) ? (sizeof(ArrayValueType)) : ((bits_arrMaxElement >> 3) +
                                                                                            ((bits_arrMaxElement & 7) !=
                                                                                             0));

        // TODO: make this generalised for signed numbers also
        auto buffer = new ArrayValueType[LINEAR_SORT_THRESHOLD];
        fm_sort_unsigned<RandomAccessIterator, ArrayValueType, ArrayValueType *>(first, last, byteNumber, buffer);
        delete[] buffer;
    }

#undef LINEAR_SORT_THRESHOLD
#undef COMPARISON_SORT_THRESHOLD
}

#endif //IR_SORT_CPP

