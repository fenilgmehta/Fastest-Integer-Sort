#ifndef PROJECT_SORTING_BASIC_SORTS_H
#define PROJECT_SORTING_BASIC_SORTS_H

#include "integer_sort.h"

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
// INSERTION SORT
template<typename ArrayElementType, typename T>
inline void insertion_sort_basic_asc(T &arr, const int_fast64_t &low, const int_fast64_t &high) {
    ArrayElementType key;

    for (int_fast64_t i = low + 1, j; i <= high; i++) {
        key = arr[i];
        j = i - 1;

        // Move elements of arr[low...i-1], that are greater than key, to one position ahead of their current position
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// MERGE sort
template<typename ArrayElementType, typename T>
inline void merge_basic_asc(T &arr, const int_fast64_t &low, const int_fast64_t &mid, const int_fast64_t &high) {
    // "mid" points to the END of FIRST sub-array
    // arr[mid] is the last element of the first sub-array

    int_fast64_t i, j, k;
    int_fast64_t leftArray_length = mid - low + 1, rightArray_length = high - mid;
    ArrayElementType L[leftArray_length];
    ArrayElementType *R = &arr[mid + 1];

    for (i = low, j = 0; j < leftArray_length; i++, j++) {
        L[j] = arr[i];
    }

    for (i = 0, j = 0, k = low; k <= high; k++) {
        if (i < leftArray_length && j < rightArray_length) {
            if (L[i] <= R[j]) {
                arr[k] = L[i++];
                // arr[k] = L[i];
                // i++;
            } else {
                arr[k] = R[j++];
                // arr[k] = R[j];
                // j++;
            }
        } else if (i == leftArray_length) {
            return;
        } else if (j == rightArray_length) {
            while (k <= high) arr[k++] = L[i++];
            return;
        }
    }
}

template<typename ArrayElementType, typename T>
inline void merge_sort_basic_asc(T &arr, const int64_t &low, const int64_t &high) {
    if (low >= high) return;

    if ((high - low) < INSERTION_SORT_THRESHOLD) {
        insertion_sort_basic_asc<ArrayElementType, T>(arr, low, high);
        return;
    }

    int_fast64_t mid = (low + high) >> 1;    // int64_t mid = (low + high) / 2;

    if ((mid - low) < INSERTION_SORT_THRESHOLD) insertion_sort_basic_asc<ArrayElementType, T>(arr, low, mid);
    else merge_sort_basic_asc<ArrayElementType, T>(arr, low, mid);

    if ((high - mid - 1) < INSERTION_SORT_THRESHOLD) insertion_sort_basic_asc<ArrayElementType, T>(arr, mid + 1, high);
    else merge_sort_basic_asc<ArrayElementType, T>(arr, mid + 1, high);

    merge_basic_asc<ArrayElementType, T>(arr, low, mid, high);
}

//#################################################
// INSERTION SORT
template<typename ArrayElementType, typename T>
inline void insertion_sort_basic_desc(T &arr, const int_fast64_t &low, const int_fast64_t &high) {
    ArrayElementType key;

    for (int_fast64_t i = low + 1, j; i <= high; i++) {
        key = arr[i];
        j = i - 1;

        // Move elements of arr[low...i-1], that are greater than key, to one position ahead of their current position
        while (j >= low && arr[j] < key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// MERGE sort
template<typename ArrayElementType, typename T>
inline void merge_basic_desc(T &arr, const int_fast64_t &low, const int_fast64_t &mid, const int_fast64_t &high) {
    // "mid" points to the END of FIRST sub-array
    // arr[mid] is the last element of the first sub-array

    int_fast64_t i, j, k;
    int_fast64_t leftArray_length = mid - low + 1, rightArray_length = high - mid;
    ArrayElementType L[leftArray_length];
    ArrayElementType *R = &arr[mid + 1];

    for (i = low, j = 0; j < leftArray_length; i++, j++) {
        L[j] = arr[i];
    }

    for (i = 0, j = 0, k = low; k <= high; k++) {
        if (i < leftArray_length && j < rightArray_length) {
            if (L[i] >= R[j]) {
                arr[k] = L[i++];
                // arr[k] = L[i];
                // i++;
            } else {
                arr[k] = R[j++];
                // arr[k] = R[j];
                // j++;
            }
        } else if (i == leftArray_length) {
            return;
        } else if (j == rightArray_length) {
            while (k <= high) arr[k++] = L[i++];
            return;
        }
    }
}

template<typename ArrayElementType, typename T>
inline void merge_sort_basic_desc(T &arr, const int_fast64_t &low, const int_fast64_t &high) {
    if (low >= high) return;

    if ((high - low) < INSERTION_SORT_THRESHOLD) {
        insertion_sort_basic_desc<ArrayElementType, T>(arr, low, high);
        return;
    }

    int_fast64_t mid = (low + high) >> 1;    // int64_t mid = (low + high) / 2;

    if ((mid - low) < INSERTION_SORT_THRESHOLD) insertion_sort_basic_desc<ArrayElementType, T>(arr, low, mid);
    else merge_sort_basic_desc<ArrayElementType, T>(arr, low, mid);

    if ((high - mid - 1) < INSERTION_SORT_THRESHOLD) insertion_sort_basic_desc<ArrayElementType, T>(arr, mid + 1, high);
    else merge_sort_basic_desc<ArrayElementType, T>(arr, mid + 1, high);

    merge_basic_desc<ArrayElementType, T>(arr, low, mid, high);
}

//#################################################
// MERGE SORT
template<typename ArrayElementType, typename T>
inline void merge_sort(T &arr, const int_fast64_t &low, const int_fast64_t &high, const bool &ascendingOrder) {
    // using ArrayElementType = std::remove_reference_t<std::remove_const_t<decltype(arr[0])>>;

    INSERTION_SORT_THRESHOLD = THRESHOLD_INSERTION_FOR_MERGE[high - low + 1];
    if (ascendingOrder) merge_sort_basic_asc<ArrayElementType, T>(arr, low, high);
    else merge_sort_basic_desc<ArrayElementType, T>(arr, low, high);
}

//#########################################################################################################################################
//-----------------------------------------------------------------------------------------------------------------------------------------
//#########################################################################################################################################
//-----------------------------------------------------------------------------------------------------------------------------------------
//#########################################################################################################################################
// INSERTION SORT
template<typename ArrayElementType, typename T, typename myGetIndex>
inline void insertion_sort_asc(T &arr, const int_fast64_t &low, const int_fast64_t &high, myGetIndex getIndex) {
    ArrayElementType key;

    for (int_fast64_t i = low + 1, j; i <= high; i++) {
        key = arr[i];
        j = i - 1;

        // Move elements of arr[low...i-1], that are greater than key, to one position ahead of their current position
        while (j >= low && getIndex(arr[j]) > getIndex(key)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// MERGE SORT
template<typename ArrayElementType, typename T, typename myGetIndex>
inline void merge_asc(T &arr, const int_fast64_t &low, const int_fast64_t &mid, const int_fast64_t &high, myGetIndex getIndex) {
    // "mid" points to the END of FIRST sub-array
    // arr[mid] is the last element of the first sub-array

    int_fast64_t i, j, k;
    int_fast64_t leftArray_length = mid - low + 1, rightArray_length = high - mid;
    ArrayElementType L[leftArray_length];
    ArrayElementType *R = &arr[mid + 1];

    for (i = low, j = 0; j < leftArray_length; i++, j++) {
        L[j] = arr[i];
    }

    for (i = 0, j = 0, k = low; k <= high; k++) {
        if (i < leftArray_length && j < rightArray_length) {
            if (getIndex(L[i]) <= getIndex(R[j])) {
                arr[k] = L[i++];
                // arr[k] = L[i];
                // i++;
            } else {
                arr[k] = R[j++];
                // arr[k] = R[j];
                // j++;
            }
        } else if (i == leftArray_length) {
            return;
        } else if (j == rightArray_length) {
            while (k <= high) arr[k++] = L[i++];
            return;
        }
    }
}

template<typename ArrayElementType, typename T, typename myGetIndex>
inline void merge_sort_asc(T &arr, const int64_t &low, const int64_t &high, myGetIndex getIndex) {
    if (low >= high) return;

    if ((high - low) < INSERTION_SORT_THRESHOLD) {
        insertion_sort_asc<ArrayElementType, T>(arr, low, high, getIndex);
        return;
    }

    int_fast64_t mid = (low + high) >> 1;    // int64_t mid = (low + high) / 2;

    if ((mid - low) < INSERTION_SORT_THRESHOLD) insertion_sort_asc<ArrayElementType, T>(arr, low, mid, getIndex);
    else merge_sort_asc<ArrayElementType, T>(arr, low, mid, getIndex);

    if ((high - mid - 1) < INSERTION_SORT_THRESHOLD) insertion_sort_asc<ArrayElementType, T>(arr, mid + 1, high, getIndex);
    else merge_sort_asc<ArrayElementType, T>(arr, mid + 1, high, getIndex);

    merge_asc<ArrayElementType, T>(arr, low, mid, high, getIndex);
}

//#################################################
// INSERTION SORT
template<typename ArrayElementType, typename T, typename myGetIndex>
inline void insertion_sort_desc(T &arr, const int_fast64_t &low, const int_fast64_t &high, myGetIndex getIndex) {
    ArrayElementType key;

    for (int_fast64_t i = low + 1, j; i <= high; i++) {
        key = arr[i];
        j = i - 1;

        // Move elements of arr[low...i-1], that are greater than key, to one position ahead of their current position
        while (j >= low && getIndex(arr[j]) < getIndex(key)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// MERGE SORT
template<typename ArrayElementType, typename T, typename myGetIndex>
inline void merge_desc(T &arr, const int_fast64_t &low, const int_fast64_t &mid, const int_fast64_t &high, myGetIndex getIndex) {
    // "mid" points to the END of FIRST sub-array
    // arr[mid] is the last element of the first sub-array

    int_fast64_t i, j, k;
    int_fast64_t leftArray_length = mid - low + 1, rightArray_length = high - mid;
    ArrayElementType L[leftArray_length];
    ArrayElementType *R = &arr[mid + 1];

    for (i = low, j = 0; j < leftArray_length; i++, j++) {
        L[j] = arr[i];
    }

    for (i = 0, j = 0, k = low; k <= high; k++) {
        if (i < leftArray_length && j < rightArray_length) {
            if (getIndex(L[i]) >= getIndex(R[j])) {
                arr[k] = L[i++];
                // arr[k] = L[i];
                // i++;
            } else {
                arr[k] = R[j++];
                // arr[k] = R[j];
                // j++;
            }
        } else if (i == leftArray_length) {
            return;
        } else if (j == rightArray_length) {
            while (k <= high) arr[k++] = L[i++];
            return;
        }
    }
}

template<typename ArrayElementType, typename T, typename myGetIndex>
inline void merge_sort_desc(T &arr, const int_fast64_t &low, const int_fast64_t &high, myGetIndex getIndex) {
    if (low >= high) return;

    if ((high - low) < INSERTION_SORT_THRESHOLD) {
        insertion_sort_desc<ArrayElementType, T>(arr, low, high, getIndex);
        return;
    }

    int_fast64_t mid = (low + high) >> 1;    // int64_t mid = (low + high) / 2;

    if ((mid - low) < INSERTION_SORT_THRESHOLD) insertion_sort_desc<ArrayElementType, T>(arr, low, mid, getIndex);
    else merge_sort_desc<ArrayElementType, T>(arr, low, mid, getIndex);

    if ((high - mid - 1) < INSERTION_SORT_THRESHOLD) insertion_sort_desc<ArrayElementType, T>(arr, mid + 1, high, getIndex);
    else merge_sort_desc<ArrayElementType, T>(arr, mid + 1, high, getIndex);

    merge_desc<ArrayElementType, T>(arr, low, mid, high, getIndex);
}

//#################################################
// MERGE SORT
template<typename ArrayElementType, typename T, typename myGetIndex>
inline void merge_sort(T &arr, const int_fast64_t &low, const int_fast64_t &high, const bool &ascendingOrder, myGetIndex getIndex) {
    // using ArrayElementType = std::remove_reference_t<std::remove_const_t<decltype(arr[0])>>;

    INSERTION_SORT_THRESHOLD = THRESHOLD_INSERTION_FOR_MERGE[high - low + 1];
    if (ascendingOrder) merge_sort_asc<ArrayElementType, T, myGetIndex>(arr, low, high, getIndex);
    else merge_sort_desc<ArrayElementType, T, myGetIndex>(arr, low, high, getIndex);
}

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################

#endif //PROJECT_SORTING_BASIC_SORTS_H
