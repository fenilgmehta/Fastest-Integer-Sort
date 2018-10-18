#include <iostream>
#include <algorithm>
using namespace std;

// NOTE: these functions sort array or vector of +ve and -ve numbers in ascending order
// HOW TO USE: copy this namespace in your ".cpp" file and call it with parameters same as std::sort "integer_sort_asc::ir_sort(<parameters>)"
namespace integer_sort_asc{
    template<typename T>
    inline int_fast16_t countBits(const T &num) {
        uint_fast64_t number = static_cast<uint_fast64_t>(num);;
        if (num < 0) number = static_cast<uint_fast64_t>(-num);;
        int_fast16_t result = 0;
        while (number > 0) {number >>= 1;result++;}
        return result;
    }

    template<typename ArrayElementType, typename T> inline void
    countingSort_asc(T &arr, const int_fast64_t &low, const int_fast64_t &high,
                     const ArrayElementType &arrMax, const ArrayElementType &arrMin) {
        int_fast64_t i, j;
        int_fast64_t boxLength = arrMax - arrMin + 1;
        auto box = new uint32_t[boxLength]();
        for (i = low; i <= high; i++) box[arr[i] - arrMin]++;
        for (i = 0, j = 0; i < boxLength; i++) while (box[i]-- > 0) arr[j++] = i + arrMin;
        delete[] box;
    }

    inline void set_BitShift_Iterations(const int_fast16_t &bitA, int_fast16_t &bitsShift, int_fast16_t &iterationsRequired) {
        if (bitA <= 8) {bitsShift = bitA; iterationsRequired = 1;}
        else if (bitA <= 10) {bitsShift = 5; iterationsRequired = 2;}
        else if (bitA <= 12) {bitsShift = 6; iterationsRequired = 2;}
        else if (bitA <= 14) {bitsShift = 7; iterationsRequired = 2;}
        else if (bitA <= 16) {bitsShift = 8; iterationsRequired = 2;}
        else if (bitA <= 18) {bitsShift = 6; iterationsRequired = 3;}
        else if (bitA <= 21) {bitsShift = 7; iterationsRequired = 3;}
        else if (bitA <= 24) {bitsShift = 8; iterationsRequired = 3;}
        else if (bitA <= 28) {bitsShift = 7; iterationsRequired = 4;}
        else if (bitA <= 32) {bitsShift = 8; iterationsRequired = 4;}
        else if (bitA <= 35) {bitsShift = 7; iterationsRequired = 5;}
        else if (bitA <= 40) {bitsShift = 8; iterationsRequired = 5;}
        else if (bitA <= 42) {bitsShift = 7; iterationsRequired = 6;}
        else if (bitA <= 48) {bitsShift = 8; iterationsRequired = 6;}
        else if (bitA == 49) {bitsShift = 7; iterationsRequired = 7;}
        else if (bitA <= 56) {bitsShift = 8; iterationsRequired = 7;}
        else {bitsShift = 8; iterationsRequired = 8;}
    }

    template<typename ArrayElementType, typename T> inline void
    radixSort_asc(T &arr, const int_fast64_t &low, int_fast64_t high,
                  const int_fast64_t &sortArrayLength, const int_fast16_t &maxBits) {
        if (low >= high) return;

        // SUBORDINATE VARIABLES
        auto *copyArray = new ArrayElementType[sortArrayLength];
        ArrayElementType *p_arr = &arr[low];
        ArrayElementType *p_copyArray = &copyArray[0];
        high -= low;

        // REQUIRED VARIABLES
        int_fast64_t i, j;
        int_fast16_t bitsShift;
        int_fast16_t iterationsRequired;
        set_BitShift_Iterations(maxBits, bitsShift, iterationsRequired);
        int_fast16_t bitsShiftFactor = 0;
        const int_fast32_t boxArraySize = static_cast<int_fast32_t>(1) << (bitsShift);
        const int_fast32_t modNum = boxArraySize - 1;
        uint_fast64_t box[boxArraySize];

        // START ACTUAL SORTING
        while (iterationsRequired-- > 0) {
            for (i = 0; i < boxArraySize;) box[i++] = 0;
            for (i = 0; i <= high; i++) box[(p_arr[i] >> bitsShiftFactor) & modNum]++;
            for (i = 1; i < boxArraySize; i++) box[i] += box[i - 1];
            std::swap<ArrayElementType *>(p_arr, p_copyArray);
            for (i = high; i >= 0; i--) p_arr[--box[((p_copyArray[i] >> bitsShiftFactor) & modNum)]] = p_copyArray[i];
            bitsShiftFactor += bitsShift;
        }

        // till here: 0,1,2,3,...,-3,-2,-1
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

        if ((&arr[low]) != p_arr) for (i = 0; i < sortArrayLength; i++) p_copyArray[i] = p_arr[i];
        delete[] copyArray;
    }

    /* 
     * ### PARAMETERS
     * arr: the array to be sorted(values from [low, high] will be sorted)
     * low: the lower index for sorting
     * high: the higher index for sorting(the element at index high will also be considered for sorting)
     *
     * ### NOTE: all the elements from arr[low] to arr[high] are sorted, both low and high inclusive
     *
     * */
    template<typename T> inline void
    ir_sort(T &arr, const int_fast64_t &low, const int_fast64_t &high) {

        if (low >= high) return;
        using ArrayElementType = std::remove_reference_t<std::remove_const_t<decltype(arr[0])>>; // this should be used if the template parameter is only "T"
        const int_fast64_t sortArrayLength = high - low + 1;

        if(sortArrayLength < 104) {
            std::sort(&arr[low],&arr[high+1]);
            return;
        }

        ArrayElementType arrMax = arr[high], arrMin = arr[high];

        for (int_fast64_t i = low; i < high; i++) {
            if (arr[i] > arrMax) arrMax = arr[i];
            else if (arr[i] < arrMin) arrMin = arr[i];
        }

        // counting sort
        if ((1.0 * sortArrayLength) / (arrMax - arrMin) > 0.71) {
            countingSort_asc<ArrayElementType>(arr, low, high, arrMax, arrMin);
            return;
        }

        int_fast16_t onlyPositiveNumbers = 0;                       //  0 means +ve and -ve numbers
        if (arrMin > -1) onlyPositiveNumbers = 1;            //  1 means +ve numbers ONLY
        else if (arrMax < 0) onlyPositiveNumbers = -1;       // -1 means -ve numbers ONLY

        int_fast16_t bitsMax = countBits(arrMax), bitsMin = countBits(arrMin);

        // IMPORTANT
        if (onlyPositiveNumbers == 0) {
            bitsMin++;
            if (bitsMin <= bitsMax) bitsMax++;
            else bitsMax = bitsMin;
        } else if (bitsMin > bitsMax) bitsMax = bitsMin;

        radixSort_asc<ArrayElementType, T>(arr, low, high, sortArrayLength, bitsMax);
    }

    // this function call has parameters similar to std::sort
    template<typename RandomAccessIterator> inline void
    ir_sort(RandomAccessIterator start, RandomAccessIterator end) {
        if(end-start < 104) std::sort(start, end);
        else ir_sort(start, 0, end - start - 1);
    }
}

int main(){
    int arr[] = {9,8,2,5,6,3,1,7,4,0};
    integer_sort_asc::ir_sort(&arr[0],&arr[10]); // USAGE: same as std::sort

    for(auto &i: arr) cout<<i<<" "; cout<<endl;
    return 0;
}
