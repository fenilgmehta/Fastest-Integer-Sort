#include <iostream>
#include <cstdint>

#include <iomanip>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <array>

#include <type_traits>

#include "integer_sort.cpp"

//#################################################
#define db1(x) cerr << "\nDebug: " << setw(30) << left << #x << " = " << (x);
#define dbArrSize(arr, size) cerr << "\nDebug: " << #arr << " [" << 0 << " : " << ((size)-1) << "] = "; for(int64_t i = 0; i < (size); i++) cerr << (arr)[i] << ", ";
#define dbArrLimit(arr, l, h) cerr << "\nDebug: " << #arr << " [" << (l) << " : " << (h) << "] = "; for(int64_t i = (l); i <= (h); i++) cerr << (arr)[i] << ", ";
#define endl '\n'

//#################################################
using namespace std;
using namespace std::chrono;

//#################################################
#define rangeup(_i, _startLimit, _endLimit) for(int64_t (_i) = (_startLimit); (_i) < (_endLimit); (_i)++)
const int columnWidth = 15;

// const int global_onlyPositiveNumbers = 1;      // +ve numbers
const int global_onlyPositiveNumbers = 0;      // +ve and -ve numbers
// const int global_onlyPositiveNumbers = -1;     // -ve numbers

// using ArrayDataType = int8_t;
// using ArrayDataType = int16_t;
// using ArrayDataType = int32_t;
using ArrayDataType = int64_t;

//#########################################################################################################################################
//-----------------------------------------------------------------------------------------------------------------------------------------
//#########################################################################################################################################
//-----------------------------------------------------------------------------------------------------------------------------------------
//#########################################################################################################################################
template<typename T>
int64_t maxIndex(const T arr[], const int64_t &size) {
    int64_t index = 0;
    rangeup(i, 1, size) if (arr[index] < arr[i]) index = i;

    return index;
}

template<typename T>
int64_t minIndex(const T arr[], const int64_t &size) {
    int64_t index = 0;
    rangeup(i, 1, size) if (arr[index] > arr[i]) index = i;

    return index;
}

template<typename T>
bool isSortedAscending(const T arr, const int64_t &low, const int64_t &high) {
    for (int64_t i = low; i < high; i++) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

template<typename T>
bool isSortedDescending(const T arr, const int64_t &low, const int64_t &high) {
    for (int64_t i = low; i < high; i++) {
        if (arr[i] < arr[i + 1]) return false;
    }
    return true;
}

template<typename T, typename MyGetIndex>
bool isSortedAscending(const T arr, const int64_t &low, const int64_t &high, MyGetIndex mygetindex) {
    for (int64_t i = low; i < high; i++) {
        if (mygetindex(arr[i]) > mygetindex(arr[i + 1])) return false;
    }
    return true;
}

template<typename T>
bool compareArray(const T &arr1, const T &arr2, const int64_t &low, const int64_t &high) {
    for (int64_t i = low; i <= high; i++) {
        if (arr1[i] != arr2[i]) {
            cout << "arrays not equal" << endl;
            return false;
        }
    }

    return true;
}

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
template<typename T>
void objFillRandArray(T &arr, const int64_t &low, const int64_t &high, const int &onlyPositive) {
    uint64_t i = low, temp1, temp2;
    if (onlyPositive == 1) {
        for (; i <= high; i++) {
            // arr[i] = (rand() % 2);                  // 2 ^ 1
            // arr[i] = (rand() % 125);                // 2 ^ 8
            // arr[i] = (rand() % 1023);               // 2 ^ 10
            // arr[i] = (rand() % 16381);              // 2 ^ 14
            // arr[i] = (rand() % 32767);              // 2 ^ 16
            // arr[i] = (rand() % 16777213);           // 2 ^ 24
            // arr[i] = (rand() % 1073741823);         // 2 ^ 32

            temp1 = (9223372036854675807 - (rand() % 2147483647));
            // arr[i] = (temp1 % 1099511627773);        // 2 ^ 40
            // arr[i] = (temp1 % 281474976710653);      // 2 ^ 48
            // arr[i] = (temp1 % 72057594037927931);    // 2 ^ 56
            arr[i] = temp1;                          // 2 ^ 64
        }
    } else if (onlyPositive == -1) {
        for (; i <= high; i++) {
            // arr[i] = (rand() % 2);                  // 2 ^ 1
            // arr[i] = (rand() % 125);                // 2 ^ 8
            // arr[i] = (rand() % 1023);               // 2 ^ 10
            // arr[i] = (rand() % 16381);              // 2 ^ 14
            // arr[i] = (rand() % 32767);              // 2 ^ 16
            // arr[i] = (rand() % 16777213);           // 2 ^ 24
            // arr[i] = (rand() % 1073741823);         // 2 ^ 32

            temp1 = (9223372036854675807 - (rand() % 2147483647));
            // arr[i] = (temp1 % 1099511627773);        // 2 ^ 40
            // arr[i] = (temp1 % 281474976710653);      // 2 ^ 48
            // arr[i] = (temp1 % 72057594037927931);    // 2 ^ 56
            arr[i] = temp1;                          // 2 ^ 64
        }
    } else {
        for (; i <= high; i++) {
            // arr[i] = ((rand() % 2) - (rand() % 2));                     // 2 ^ 1
            // arr[i] = ((rand() % 125) - (rand() % 125));                 // 2 ^ 8
            // arr[i] = ((rand() % 1023) - (rand() % 1023));               // 2 ^ 10
            // arr[i] = ((rand() % 16381) - (rand() % 16381));             // 2 ^ 14
            // arr[i] = ((rand() % 32767) - (rand() % 32767));             // 2 ^ 16
            // arr[i] = ((rand() % 16777213) - (rand() % 16777213));       // 2 ^ 24
            // arr[i] = ((rand() % 1073741823) - (rand() % 1073741823));   // 2 ^ 32

            temp1 = (9223372036854675807 - (rand() % 2147483647));
            temp2 = (9223372036854675807 - (rand() % 2147483647));
            // arr[i] = ((temp1 % 1099511627773) - (temp1 % 1099511627773));           // 2 ^ 40
            // arr[i] = ((temp1 % 281474976710653) - (temp1 % 281474976710653));       // 2 ^ 48
            // arr[i] = ((temp1 % 72057594037927931) - (temp1 % 72057594037927931));   // 2 ^ 56
            arr[i] = temp1 - temp2;                                                 // 2 ^ 64
        }
    }
}

//#########################################################################################################################################
#define startTime copy(&baseArray[0], &baseArray[0] + arrayLength, &arr[0]); start = high_resolution_clock::now();
#define endTime stop = high_resolution_clock::now(); duration = duration_cast<nanoseconds>(stop - start);
#define checkSortingRange_asc(_arr, _low, _high) if (!(isSortedAscending(_arr, _low, _high))) {cout << "\nERROR: array is not sorted in ascending order\n"; dbArrLimit(_arr, _low, _high);}
#define checkSortingRange_desc(_arr, _low, _high) if (!(isSortedDescending(_arr, _low, _high))) {cout << "\nERROR: array is not sorted in descending order\n"; dbArrLimit(_arr, _low, _high);}

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
int main() {
    int setwLen = 11, setwLenRatio = 11;
    long double irSortRatio = 0, stdSortRatio = 0;
    cout << setprecision(3);

    auto start = high_resolution_clock::now();      // Get starting time
    auto stop = start;      // Get ending time
    nanoseconds duration;   // Get duration. Subtract start time point to get duration. To cast it to proper unit use duration cast method

    int64_t minArrayLength = 0, minMaxArrayLengthDiff = 0, testCases = 0;
    cout << "Input: (minArrayLength, minMaxDiff, testCases;) = ";
    cin >> minArrayLength >> minMaxArrayLengthDiff >> testCases;
    cout << endl;
    int64_t maxArrayLength = minArrayLength + minMaxArrayLengthDiff;

    // ARRAY
    vector<ArrayDataType> arr(minArrayLength+minMaxArrayLengthDiff);            // used to check sorting time, it will the values from the baseArray
    vector<ArrayDataType> baseArray(minArrayLength+minMaxArrayLengthDiff);      // used to store the random array generated

    rangeup(arrayLength, minArrayLength, maxArrayLength + 1) {
        cout << "#####################################################################";
        db1(arrayLength)
        db1(testCases)

        irSortRatio = 0, stdSortRatio = 0;

        int64_t timeArrLength = {2};
        int64_t timeArr[timeArrLength];             // used to store the time taken for a particular sorting technique
        int64_t bestThreshold[timeArrLength];       // used to keep track of the best THRESHOLD
        fill_n(timeArr, timeArrLength, 0);
        fill_n(bestThreshold, timeArrLength, 0);

        cout << "\n\nTime taken in nano-seconds, Time ratio with respect to ir_sort";
        cout << endl << left << setw(setwLen) << "ir_sort" << "\t," << setw(setwLen) << "std::sort" << "\t,"
             << setw(setwLenRatio) << "ratio ir_sort" << "\t," << setw(setwLenRatio) << "ratio std::sort";

        rangeup(__, 0, testCases) {
            int64_t myTempLow = 0, myTempHigh = arrayLength - 1;

            // Fill up the array
            objFillRandArray(baseArray, 0, arrayLength - 1, global_onlyPositiveNumbers);
            // index to insert data in timeArr
            int64_t timeArrIndex = 0;

            startTime
            ir_sort(&arr[0]+myTempLow, &arr[0]+myTempHigh+1, true);      // first two parameters same as std::sort, ascending order
            // ir_sort(&arr[0]+myTempLow, &arr[0]+myTempHigh+1, false);     // first two parameters same as std::sort, descending order
            // ir_sort(arr, myTempLow, myTempHigh, true);      // ascending order
            // ir_sort(arr, myTempLow, myTempHigh, false);     // descending order
            endTime
            timeArr[timeArrIndex++] = duration.count();
            checkSortingRange_asc(arr, myTempLow, myTempHigh)       // check ascending order sorting
            // checkSortingRange_desc(arr, myTempLow, myTempHigh)      // check descending order sorting

            startTime
            sort(&arr[0] + myTempLow, &arr[0] + myTempHigh + 1);    // ascending order
            // sort(&arr[0] + myTempLow, &arr[0] + myTempHigh + 1, [](ArrayDataType &a, ArrayDataType &b) { return a > b; });   // descending order
            endTime
            timeArr[timeArrIndex++] = duration.count();
            checkSortingRange_asc(arr, myTempLow, myTempHigh)       // check ascending order sorting
            // checkSortingRange_desc(arr, myTempLow, myTempHigh)      // check descending order sorting

            cout << endl << left << setw(setwLen) << timeArr[0] << "\t," << setw(setwLen) << timeArr[1] << "\t,"
                 << setw(setwLenRatio) << timeArr[0] / timeArr[0] << "\t,"
                 << setw(setwLenRatio) << ((1.0 * timeArr[1]) / timeArr[0]);
            irSortRatio += (timeArr[0] / timeArr[0]);
            stdSortRatio += (((1.0 * timeArr[1]) / timeArr[0]));

            //#################################################
            bestThreshold[minIndex(timeArr, timeArrLength)]++;
        }

        cout << endl;
        dbArrSize(bestThreshold, timeArrLength);    // This will print the number of times ir_sort was fast and number of times std::sort was fast
        int64_t maxSpeedIndex = maxIndex(bestThreshold, timeArrLength);
        db1(bestThreshold[maxSpeedIndex])                           // Number of times ir_sort was faster than std::sort out of total testCases or vice-versa

        if (maxSpeedIndex == 0) cout << "\n\nir_sort is faster than std::sort ";
        else cout << "\n\nstd::sort is faster than ir_sort ";
        cout << (bestThreshold[maxSpeedIndex] * 100.0 / testCases) << " % times, for testCases = " << testCases;

        cout << "\n\nir_sort speed ratio average = " << irSortRatio / testCases;
        cout << "\nstd::sort speed ratio average = " << stdSortRatio / testCases;

        cout << endl << endl;
    }

    return 0;
}

