#ifndef IR_SORT_BASIC_SORTS_HPP
#define IR_SORT_BASIC_SORTS_HPP

#include <cstdint>
#include <iterator>

namespace ir_sort {
    namespace basic_sorts {

//############################################################################
//                                                                          ##
//                            T H R E S H O L D S                           ##
//                                                                          ##
//############################################################################

        int_fast32_t INSERTION_SORT_THRESHOLD = 30;  // 25
        int_fast32_t MERGE_SORT_THRESHOLD = 256;     // 135

        // insertion_sort threshold in merge_sort
        // 0 to 1360
        const int_fast32_t THRESHOLD_INSERTION_FOR_MERGE[] = {0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 9, 10, 10, 12, 11, 14, 15, 13, 13, 16, 14, 15, 15, 19, 21, 19, 20,
                                                            25, 21, 22, 22, 23, 23, 23, 33, 23, 24, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24,
                                                            24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36,
                                                            17, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23,
                                                            23, 24, 24, 24, 24, 25, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29,
                                                            29, 30, 30, 30, 30, 31, 31, 31, 31, 32, 32, 32, 32, 33, 33, 33, 33, 34, 34, 34, 34, 35, 35, 35,
                                                            35, 36, 36, 36, 36, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20,
                                                            20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23,
                                                            23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26,
                                                            26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29,
                                                            29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 32, 32, 32,
                                                            32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34, 34, 34, 35, 35, 35,
                                                            35, 35, 35, 35, 35, 36, 36, 36, 36, 36, 36, 36, 36, 37, 37, 37, 37, 18, 18, 18, 18, 18, 18, 18,
                                                            18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20,
                                                            20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
                                                            21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23,
                                                            23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
                                                            24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26,
                                                            26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
                                                            27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29,
                                                            29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                                                            30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 32, 32,
                                                            32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
                                                            33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35, 35, 35,
                                                            35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
                                                            36, 36, 36, 36, 36, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 18, 18, 18, 18, 18, 18, 18, 18,
                                                            18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
                                                            19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20,
                                                            20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
                                                            20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
                                                            21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
                                                            22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23,
                                                            23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
                                                            23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
                                                            24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
                                                            25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26,
                                                            26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
                                                            26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
                                                            27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
                                                            28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29,
                                                            29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
                                                            29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                                                            30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
                                                            31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 32, 32,
                                                            32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
                                                            32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
                                                            33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
                                                            34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35, 35, 35,
                                                            35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
                                                            35, 35, 35, 35, 35, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
                                                            36, 36, 36, 36, 17, 17, 36, 36, 36, 36, 36, 36, 36, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
                                                            18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
                                                            18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
                                                            18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
                                                            19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
                                                            19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20,
                                                            20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};

//############################################################################
//                                                                          ##
//   B A S I C   S O R T S   F O R   P R I M I T I V E   D A T A T Y P E S  ##
//                                                                          ##
//############################################################################

        // INSERTION SORT
        template<typename RandomAccessIterator, typename ArrayValueType>
        inline void insertion_sort_basic_asc(RandomAccessIterator arr, const int_fast64_t low, const int_fast64_t high) {
            ArrayValueType key;

            for (int_fast64_t i = low + 1, j; i <= high; ++i) {
                key = arr[i];
                j = i - 1;

                // Move elements of arr[low...i-1], that are greater than key, to one position ahead of their current position
                while (j >= low && arr[j] > key) {
                    arr[j + 1] = arr[j];
                    --j;
                }
                arr[j + 1] = key;
            }
        }

        // MERGE sort
        template<typename RandomAccessIterator, typename ArrayValueType>
        inline void merge_basic_asc(RandomAccessIterator arr, const int_fast64_t low, const int_fast64_t mid, const int_fast64_t high) {
            // "mid" points to the END of FIRST sub-array
            // arr[mid] is the last element of the first sub-array

            int_fast64_t i, j, k;
            int_fast64_t leftArray_length = mid - low + 1, rightArray_length = high - mid;
            ArrayValueType L[leftArray_length]; // this will work here because we use merge_sort only for array size < approx 500
            RandomAccessIterator R = std::next(arr, mid+1);

            for (i = low, j = 0; j < leftArray_length; ++i, ++j) {
                L[j] = arr[i];
            }

            for (i = 0, j = 0, k = low; k <= high; ++k) {
                if (i < leftArray_length && j < rightArray_length) {
                    if (L[i] <= R[j]) {
                        arr[k] = L[i++];
                        // arr[k] = L[i];
                        // ++i;
                    } else {
                        arr[k] = R[j++];
                        // arr[k] = R[j];
                        // ++j;
                    }
                } else if (i == leftArray_length) {
                    return;
                } else if (j == rightArray_length) {
                    while (k <= high) arr[k++] = L[i++];
                    return;
                }
            }
        }

        template<typename RandomAccessIterator, typename ArrayValueType>
        inline void merge_sort_basic_asc(RandomAccessIterator arr, const int64_t &low, const int64_t &high) {
            if (low >= high) return;

            if ((high - low) < INSERTION_SORT_THRESHOLD) {
                insertion_sort_basic_asc<RandomAccessIterator, ArrayValueType>(arr, low, high);
                return;
            }

            int_fast64_t mid = (low + high) >> 1;    // int64_t mid = (low + high) / 2;

            if ((mid - low) < INSERTION_SORT_THRESHOLD) insertion_sort_basic_asc<RandomAccessIterator, ArrayValueType>(arr, low, mid);
            else merge_sort_basic_asc<RandomAccessIterator, ArrayValueType>(arr, low, mid);

            if ((high - mid - 1) < INSERTION_SORT_THRESHOLD) insertion_sort_basic_asc<RandomAccessIterator, ArrayValueType>(arr, mid + 1, high);
            else merge_sort_basic_asc<RandomAccessIterator, ArrayValueType>(arr, mid + 1, high);

            merge_basic_asc<RandomAccessIterator, ArrayValueType>(arr, low, mid, high);
        }

//=================================================
        // MERGE SORT and INSERTION SORT hybrid
        template<typename RandomAccessIterator>
        inline void merge_sort(RandomAccessIterator start, RandomAccessIterator end) {
            // return if RandomAccessIterator is not a random_access_iterator
            if (typeid(typename std::iterator_traits<RandomAccessIterator>::iterator_category) != typeid(std::random_access_iterator_tag))
                return;

            // return if the given RandomAccessIterator is not for an integer
            if (! std::is_integral<typename std::iterator_traits<RandomAccessIterator>::value_type>::value)
                return;

            int low = 0, high = end-start-1;
            if ((high - low + 1) <= 1360) INSERTION_SORT_THRESHOLD = THRESHOLD_INSERTION_FOR_MERGE[high - low + 1];

            // NOTE: determine the type of elements stored in the array to be sorted, and it pointer type
            using ArrayValueType = typename std::iterator_traits<RandomAccessIterator>::value_type;

            merge_sort_basic_asc<RandomAccessIterator, ArrayValueType>(start, low, high);
        }

//############################################################################
//                                                                          ##
//            B A S I C   S O R T S   F O R   O B J E C T                   ##
//                                                                          ##
//############################################################################

        // INSERTION SORT
        template<typename RandomAccessIterator, typename ArrayValueType, typename myGetIndex>
        inline void insertion_sort_asc(RandomAccessIterator arr, const int_fast64_t low, const int_fast64_t high, myGetIndex getIndex) {
            ArrayValueType key;

            for (int_fast64_t i = low + 1, j; i <= high; ++i) {
                key = arr[i];
                j = i - 1;

                // Move elements of arr[low...i-1], that are greater than key, to one position ahead of their current position
                while (j >= low && getIndex(arr[j]) > getIndex(key)) {
                    arr[j + 1] = arr[j];
                    --j;
                }
                arr[j + 1] = key;
            }
        }

        // MERGE SORT
        template<typename RandomAccessIterator, typename ArrayValueType, typename myGetIndex>
        inline void merge_asc(RandomAccessIterator arr, const int_fast64_t low, const int_fast64_t mid, const int_fast64_t high, myGetIndex getIndex) {
            // "mid" points to the END of FIRST sub-array
            // arr[mid] is the last element of the first sub-array

            int_fast64_t i, j, k;
            int_fast64_t leftArray_length = mid - low + 1, rightArray_length = high - mid;
            ArrayValueType L[leftArray_length]; // CHECK: not sure if this would suffice for large object arrays
            RandomAccessIterator R = std::next(arr, mid+1);

            for (i = low, j = 0; j < leftArray_length; ++i, ++j) {
                L[j] = arr[i];
            }

            for (i = 0, j = 0, k = low; k <= high; ++k) {
                if (i < leftArray_length && j < rightArray_length) {
                    if (getIndex(L[i]) <= getIndex(R[j])) {
                        arr[k] = L[i++];
                        // arr[k] = L[i];
                        // ++i;
                    } else {
                        arr[k] = R[j++];
                        // arr[k] = R[j];
                        // ++j;
                    }
                } else if (i == leftArray_length) {
                    return;
                } else if (j == rightArray_length) {
                    while (k <= high) arr[k++] = L[i++];
                    return;
                }
            }
        }

        template<typename RandomAccessIterator, typename ArrayValueType, typename myGetIndex>
        inline void merge_sort_asc(RandomAccessIterator arr, const int64_t &low, const int64_t &high, myGetIndex getIndex) {
            if (low >= high) return;

            if ((high - low) < INSERTION_SORT_THRESHOLD) {
                insertion_sort_asc<RandomAccessIterator, ArrayValueType, myGetIndex>(arr, low, high, getIndex);
                return;
            }

            int_fast64_t mid = (low + high) >> 1;    // int64_t mid = (low + high) / 2;

            if ((mid - low) < INSERTION_SORT_THRESHOLD) insertion_sort_asc<RandomAccessIterator, ArrayValueType, myGetIndex>(arr, low, mid, getIndex);
            else merge_sort_asc<RandomAccessIterator, ArrayValueType, myGetIndex>(arr, low, mid, getIndex);

            if ((high - mid - 1) < INSERTION_SORT_THRESHOLD) insertion_sort_asc<RandomAccessIterator, ArrayValueType, myGetIndex>(arr, mid + 1, high, getIndex);
            else merge_sort_asc<RandomAccessIterator, ArrayValueType, myGetIndex>(arr, mid + 1, high, getIndex);

            merge_asc<RandomAccessIterator, ArrayValueType, myGetIndex>(arr, low, mid, high, getIndex);
        }

//=================================================
        // MERGE SORT
        template<typename RandomAccessIterator, typename myGetIndex>
        inline void merge_sort(RandomAccessIterator first, RandomAccessIterator last, myGetIndex getIndex) {
            // return if RandomAccessIterator is not a random_access_iterator
            if (typeid(typename std::iterator_traits<RandomAccessIterator>::iterator_category) != typeid(std::random_access_iterator_tag))
                return;

            // return if getIndex does not return integer for the given RandomAccessIterator
            if (! std::is_integral<decltype(getIndex(first[0]))>::value)
                return;

            int low = 0, high = last-first-1;
            if ((high - low + 1) <= 1360) INSERTION_SORT_THRESHOLD = THRESHOLD_INSERTION_FOR_MERGE[high - low + 1];

            // NOTE: determine the type of elements stored in the array to be sorted, and it pointer type
            using ArrayValueType = typename std::iterator_traits<RandomAccessIterator>::value_type;

            merge_sort_asc<RandomAccessIterator, ArrayValueType, myGetIndex>(first, low, high, getIndex);
        }

    } // namespace basic_sorts
} // namespace ir_sort


//=========================================================================================================================================
//=========================================================================================================================================
//=========================================================================================================================================

#endif //IR_SORT_BASIC_SORTS_HPP
