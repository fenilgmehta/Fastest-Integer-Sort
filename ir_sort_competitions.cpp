#include <iostream>
#include <algorithm>
#include <vector>
#include <type_traits>
using namespace std;

#define dbrange(_arr, _l, _h) {cerr << "\033[1;31m" << "\nDebug: " << "\033[0m" << #_arr << " [" << (_l) << " : " << (_h) << "] = "; for(int64_t _i = (_l); _i < (_h); _i++) cerr << (_arr)[_i] << ", "; cout<<"\n";}


// NOTE: these functions sort array or vector of +ve and -ve numbers in ascending order
// HOW TO USE: copy this namespace in your ".cpp" file and call it with parameters same as std::sort "integer_sort::ir_sort(<parameters>)"
namespace integer_sort{
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
        // vector<ArrayElementType> copyArray(sortArrayLength);
        auto *copyArray = new ArrayElementType[sortArrayLength];
        ArrayElementType *p_arr = &arr[low];
        ArrayElementType *p_copyArray = &copyArray[0];
        if((&arr[1])!=((&arr[0]) + 1)) p_arr=&arr[high];  // the given parameter "arr" is a reverse iterator
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
            // dbrange(arr, low, high+1)
            // dbrange(copyArray,low, high+1)
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

        // MODIFIED
        if (copyArray == p_copyArray){
            // "arr" is a reverse iterator
            if((&arr[1])!=((&arr[0]) + 1)) reverse(p_arr, p_arr+sortArrayLength);
        }else{
            // "arr" is a reverse iterator
            if((&arr[1])!=((&arr[0]) + 1)) reverse_copy(p_arr, p_arr+sortArrayLength, p_copyArray);
            else copy(p_arr, p_arr+sortArrayLength, p_copyArray);
        }

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
    template<typename RandomAccessIterator> inline void ir_sort(RandomAccessIterator start, RandomAccessIterator end, bool ascendingOrder = true) {
        auto revStart = make_reverse_iterator(end);
        auto revEnd = make_reverse_iterator(start);
        if(ascendingOrder == false){
            if((end-start) < 108) std::sort(revStart, revEnd);
            else ir_sort(revStart, 0, end - start - 1);
        }else{
            if((end-start) < 104) std::sort(start, end);
            else ir_sort(start, 0, end - start - 1);
        }
    }
}

#define iterArr(arr_name, iter_start, iter_end) &arr_name[iter_start],&arr_name[iter_end]

int main(){
    int arr[] = {870, 764, 350, 935, 672, 390, 1012, 48, 220, 583, 963, 637, 430, 997, 378, 438, 308, 890, 682, 801, 354, 243, 913, 263, 543, 128, 882, 316, 512, 855, 299, 703, 244, 232, 603, 862, 893, 309, 23, 519, 669, 732, 602, 36, 909, 88, 301, 556, 962, 789, 865, 107, 1018, 939, 733, 620, 490, 930, 242, 17, 978, 996, 651, 42, 197, 358, 937, 1004, 664, 698, 941, 90, 658, 379, 31, 821, 661, 594, 597, 716, 441, 930, 290, 445, 665, 617, 293, 638, 133, 801, 81, 235, 377, 945, 42, 515, 341, 916, 228, 708, 237, 657, 675, 692, 720, 1018, 126, 965, 218, 209};
    int len = sizeof(arr)/sizeof(int);
    // vector<int> arr {9,8,2,5,6,3,1,7,4,11};
    // int len = arr.size();

    // ##### USAGE: same as std::sort #####
    // ##### NOTE: will sort from index [start,end), i.e "end" will NOT be considered for sorting

    // ##### ASCENDING ORDER examples
    integer_sort::ir_sort(&arr[0],&arr[150]);
    // integer_sort::ir_sort(begin(arr),end(arr));

    // ##### DESCENDING ORDER examples, valid with ITERATORS ONLY
    // # TYPE - 1
    // integer_sort::ir_sort(&arr[0],&arr[150],false);
    // integer_sort::ir_sort(begin(arr),end(arr),false);
    // # TYPE - 2
    // integer_sort::ir_sort(rbegin(arr),rend(arr), true);

    // ##### WRONG
    // integer_sort::ir_sort(&arr[9],&arr[0]-1);
    // integer_sort::ir_sort(&arr[9],&arr[-1]);

    dbrange(arr, 0, len);

    return 0;
}
