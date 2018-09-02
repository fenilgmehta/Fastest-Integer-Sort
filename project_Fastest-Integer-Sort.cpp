#include <iostream>

#include <iomanip>
#include <algorithm>
#include <chrono>

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

const int64_t __mySize__ = 100000000;   //10_00_00000

// const bool global_onlyPositiveNumbers = true;        // +ve numbers
const bool global_onlyPositiveNumbers = false;          // +ve and -ve numbers

// using ArrayDataType = int32_t;

using ArrayDataType = int64_t;


//#########################################################################################################################################
//#########################################################################################################################################
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

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
template<typename T>
void objFillRandArray(T &arr, const int64_t &low, const int64_t &high, const bool &onlyPositive = true) {
    int64_t i = low, temp1, temp2;
    if (onlyPositive) {
        // only +ve random numbers
        for (; i <= high; i++) {
            // arr[i] = (rand() % 2);                  // 2 ^ 1
            // arr[i] = (rand() % 251);                // 2 ^ 8
            // arr[i] = (rand() % 1023);               // 2 ^ 10
            // arr[i] = (rand() % 16381);              // 2 ^ 14
            // arr[i] = (rand() % 65521);              // 2 ^ 16
            // arr[i] = (rand() % 16777213);           // 2 ^ 24
            // arr[i] = (rand() % 2147483647);         // 2 ^ 32

            temp1 = (9223372036854775807 - (rand() % 2147483647));
            // arr[i] = (temp1 % 1099511627773);        // 2 ^ 40
            // arr[i] = (temp1 % 281474976710653);      // 2 ^ 48
            // arr[i] = (temp1 % 72057594037927931);    // 2 ^ 56
            arr[i] = temp1;                          // 2 ^ 64
        }
    } else {
        // +ve and -ve random numbers
        for (; i <= high; i++) {
            // arr[i] = ((rand() % 2) - (rand() % 2));                     // 2 ^ 1
            // arr[i] = ((rand() % 251) - (rand() % 251));                 // 2 ^ 8
            // arr[i] = ((rand() % 1023) - (rand() % 1023));               // 2 ^ 10
            // arr[i] = ((rand() % 16381) - (rand() % 16381));             // 2 ^ 14
            // arr[i] = ((rand() % 65521) - (rand() % 65521));             // 2 ^ 16
            // arr[i] = ((rand() % 16777213) - (rand() % 16777213));       // 2 ^ 24
            // arr[i] = ((rand() % 2147483647) - (rand() % 2147483647));   // 2 ^ 32

            temp1 = (9223372036854775807 - (rand() % 2147483647));
            temp2 = (9223372036854775807 - (rand() % 2147483647));
            // arr[i] = ((temp1 % 1099511627773) - (temp1 % 1099511627773));           // 2 ^ 40
            // arr[i] = ((temp1 % 281474976710653) - (temp1 % 281474976710653));       // 2 ^ 48
            // arr[i] = ((temp1 % 72057594037927931) - (temp1 % 72057594037927931));   // 2 ^ 56
            arr[i] = temp1 - temp2;                          // 2 ^ 64
        }
    }

}

//#########################################################################################################################################
#define startTime copy(&baseArray[0], &baseArray[0] + arrayLength, &arr[0]); start = high_resolution_clock::now();
#define endTime stop = high_resolution_clock::now(); duration = duration_cast<nanoseconds>(stop - start);
#define checkSortingRange_asc(_arr, _low, _high) if (!(isSortedAscending(_arr, _low, _high))) {cout << "\nERROR: array is not sorted in ascending order\n"; dbArrLimit(_arr, _low, _high);}

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
int main() {
    auto start = high_resolution_clock::now();      // Get starting time
    auto stop = start;      // Get ending time
    nanoseconds duration;   // Get duration. Subtract start time point to get duration. To cast it to proper unit use duration cast method

    int64_t minArrayLength = 0, minMaxArrayLengthDiff = 0, testCases = 0;

    cout << "Input: (minArrayLength, minMaxDiff, testCases;) = ";
    cin >> minArrayLength >> minMaxArrayLengthDiff >> testCases;
    cout << endl;
    int64_t maxArrayLength = minArrayLength + minMaxArrayLengthDiff;

    static ArrayDataType *arr = new ArrayDataType[__mySize__];            // used to store the random array generated
    static ArrayDataType *baseArray = new ArrayDataType[__mySize__];      // used to check sorting time, it will the values from the baseArray

    rangeup(arrayLength, minArrayLength, maxArrayLength + 1) {
        cout << "#####################################################################";
        db1(arrayLength)
        db1(testCases)

        int64_t timeArrLength = {2};
        int64_t timeArr[timeArrLength];             // used to store the time taken for a particular sorting technique
        int64_t bestThreshold[timeArrLength];       // used to keep track of the best THRESHOLD
        fill_n(timeArr, timeArrLength, 0);
        fill_n(bestThreshold, timeArrLength, 0);

        cout << "\n\nTime taken in nano-seconds:";
        cout << "\nir_sort\t,std::sort";
        rangeup(__, 0, testCases) {
            int64_t myTempLow = 0, myTempHigh = arrayLength - 1;

            // Fill up the array
            objFillRandArray(baseArray, 0, arrayLength - 1, global_onlyPositiveNumbers);
            int64_t timeArrIndex = 0;       // index to insert data in timeArr

            startTime
            ir_sort(arr, myTempLow, myTempHigh, true);
            endTime
            timeArr[timeArrIndex++] = duration.count();
            checkSortingRange_asc(arr, myTempLow, myTempHigh)

            startTime
            sort(&arr[0] + myTempLow, &arr[0] + myTempHigh + 1);
            endTime
            timeArr[timeArrIndex++] = duration.count();
            checkSortingRange_asc(arr, myTempLow, myTempHigh)

            cout << endl << timeArr[0] << "\t," << timeArr[1];

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

        cout << endl << endl;
    }

    delete[] arr;
    delete[] baseArray;
    cout << endl;
    return 0;
}

