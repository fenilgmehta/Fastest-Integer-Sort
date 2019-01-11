#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
#pragma clang diagnostic ignored "-Wfor-loop-analysis"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma ide diagnostic ignored "OCSimplifyInspection"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

#include <bits/stdc++.h>

// #include <iostream>
// #include <cstdint>

// #include <iomanip>
// #include <algorithm>
// #include <chrono>
// #include <cstring>
// #include <array>
// #include <vector>

// #include <type_traits>

#include "../src/integer_sort.cpp"

//#################################################
using namespace std;
using namespace std::chrono;

//#################################################
#define endl '\n'
#define db1(x) cout << "\nDebug: " << setw(30) << left << #x << " = " << (x);
#define db2(x, y) cout << "\nDebug: " << #x << " = " << (x) << ",   " << #y << " = " << (y);
#define db3(x, y, z) cout << "\nDebug: " << #x << " = " << (x) << ",   " << #y << " = " << (y) << ",   " << #z << " = " << (z);
#define dbArrSize(arr, size) cout << "\nDebug: " << #arr << " [" << 0 << " : " << ((size)-1) << "] = "; for(int64_t i = 0; i < (size); i++) cout << (arr)[i] << ", ";
#define dbArrLimit(arr, l, h) cout << "\nDebug: " << #arr << " [" << (l) << " : " << (h) << "] = "; for(int64_t i = (l); i <= (h); i++) cout << (arr)[i] << ", ";
#define dbiter(name, first, len) cerr << "\033[1;31m" << "Debug: " << "\033[0m" << name << " = "; for(int64_t i_temp1 = 0, len_temp1 = (len); i_temp1 < len_temp1; ++i_temp1) cerr<<first[i_temp1]<<", "; cerr<<endl;
#define min(_a, _b) ((_a) < (_b)) ? (_a) : (_b)

//#################################################
#define rangeup(_i, _startLimit, _endLimit) for(int64_t (_i) = (_startLimit); (_i) < (_endLimit); (_i)++)
const int32_t columnWidth = 15;

class myClassInt;

using ArrayIndexType = int64_t;

// ##############################
// ####### SETTINGS start #######
// ##############################

/*
 * This macro will decide whether to print the time comparison statistics or not
 * true  : will print
 * false : will not print the stats
 *
 * */
#define settings_PRINT_TIME_COMPARISON false

/*
 * Decide whether to sort the array in ascending order or descending order
 * true  : ascending order
 * false : descending order
 *
 * */
#define settings_SORT_IN_ASCENDING_ORDER true

/*
 * Whether to check integer array sorting speed or object array sorting speed
 * Commented : use integer array for speed test
 * NOT commented : use object array for speed test
 *
 * */
#define settings_OBJ_ARRAY_COMPARISON

/*
 * Valid if the above option is uncommented
 * This number will decide the size of object to be used to create the array
 * This size if a multiple of 64 bits or 8 bytes
 *
 * Example: 2 means that the size of each object will be 2 * 64 bits = 128 bits
 *
 * */
#define settings_OBJ_SIZE_IN_MULTIPLE_64_BITS 100

/*
 * This macro will decide the sign of the numbers which are used to fill the array
 *
 * global_onlyPositiveNumbers = 0;      // +ve and -ve numbers
 * global_onlyPositiveNumbers = 1;      // +ve numbers
 * global_onlyPositiveNumbers = 2;      // -ve numbers
 *
 * */
#define settings_onlyPositiveNumbers 1

/*
 * Number of bits to be used
 *
 * */
const int32_t myBits = 26;


// ############################
// ####### SETTINGS end #######
// ############################


#ifndef settings_OBJ_ARRAY_COMPARISON
// using ArrayDataType = int8_t;
// using ArrayDataType = int16_t;
// using ArrayDataType = int32_t;
using ArrayDataType = int64_t;
#else
using ArrayDataType = myClassInt;
#endif


class myClassInt {
private:
    int64_t arr[settings_OBJ_SIZE_IN_MULTIPLE_64_BITS];
public:
    inline myClassInt() { arr[0] = -1; }

    template<typename T>
    explicit inline myClassInt(const T &ii) { arr[0] = ii; }

    inline int64_t classGetIndex() const { return arr[0]; }

    inline int64_t &operator=(const int64_t &b) { return (arr[0] = b); }

    inline bool operator==(const myClassInt &a) { return arr[0] == a.arr[0]; }

    inline bool operator!=(const myClassInt &a) { return arr[0] != a.arr[0]; }

    inline bool operator<(const myClassInt &a) { return arr[0] < a.arr[0]; }

    inline bool operator>(const myClassInt &a) { return arr[0] > a.arr[0]; }

    inline friend ostream &operator<<(ostream &, myClassInt &);

};

ostream &operator<<(ostream &out, myClassInt &a) {
    out << a.arr[0];
    return out;
}

inline int64_t f1_getIndex(const myClassInt &a) { return a.classGetIndex(); }
inline bool f2_ascendingOrder(const myClassInt &a, const myClassInt &b) { return a.classGetIndex() < b.classGetIndex(); }
inline bool f3_descendingOrder(const myClassInt &a, const myClassInt &b) { return a.classGetIndex() > b.classGetIndex(); }
inline bool f4_notEquals(const myClassInt &a, const myClassInt &b) { return a.classGetIndex() != b.classGetIndex(); }

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
template<typename RandomAccessIterator>
bool compareArray(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator second){
    RandomAccessIterator it_first = first, it_second = second;
    int index = 0;

    while(it_first != last){
        if(it_first!=it_second){
            cout << "\n\nERROR: arrays not equal";
            db3(index, *it_first, *it_second)
            dbiter("arr1[]", first, distance(first, last));
            dbiter("arr2[]", second, distance(first, last));
            return false;
        }
        ++index;
        ++it_first;
        ++it_second;
    }
    return true;
}

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
template<typename T>
int64_t maxIndex(const T &arr, const int64_t &size) {
    int64_t index = 0;
    rangeup(i, 1, size) if (arr[i] > arr[index]) index = i;
    return index;
}

template<typename T>
int64_t minIndex(const T &arr, const int64_t &size) {
    int64_t index = 0;
    rangeup(i, 1, size) if (arr[i] < arr[index]) index = i;
    return index;
}

template<typename T>
bool isSorted(const T &arr, const int64_t &low, const int64_t &high, const bool &checkAscendingOrder = settings_SORT_IN_ASCENDING_ORDER) {
    if (checkAscendingOrder) {
        for (int64_t i = low; i < high; i++) {
            if (arr[i] > arr[i + 1]) return false;
        }
    } else {
        for (int64_t i = low; i < high; i++) {
            if (arr[i] < arr[i + 1]) return false;
        }
    }
    return true;
}

template<typename T, typename MyGetIndex>
bool
isSorted(const T &arr, const int64_t &low, const int64_t &high, MyGetIndex mygetindex, const bool &checkAscendingOrder = settings_SORT_IN_ASCENDING_ORDER) {
    if (checkAscendingOrder) {
        for (int64_t i = low; i < high; i++) {
            if (mygetindex(arr[i]) > mygetindex(arr[i + 1])) return false;
        }
    } else {
        for (int64_t i = low; i < high; i++) {
            if (mygetindex(arr[i]) < mygetindex(arr[i + 1])) return false;
        }
    }
    return true;
}

template<typename T>
void printArray(T &arr, int64_t low, int64_t high) {
    cout << endl;
    for (int64_t i = low; i <= high; i++) cout << arr[i] << ", ";
    cout << endl;
}

template<typename T>
void fillRandArray(T &arr, const int64_t &low, const int64_t &high, const int64_t &randStart, const int64_t &randEnd) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<ArrayIndexType> dis(randStart, randEnd);

    for (int64_t i = low; i <= high; i++) arr[i] = dis(gen);
}

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
#define startTimeOnly start = high_resolution_clock::now();
#define startTime copy(&baseArray[0], &baseArray[0] + arrayLength, &arr[0]); start = high_resolution_clock::now();
#define endTime stop = high_resolution_clock::now(); duration = duration_cast<nanoseconds>(stop - start);
#define printTimeTaken cout << endl << setw(columnWidth) << duration.count();
#define checkSortingRange_asc(_arr, _low, _high) if (!(isSortedAscending(_arr, _low, _high))) {cout << "\nERROR: array is not sorted in ascending order\n"; dbArrLimit(_arr, _low, _high);}
#define checkSortingRange_desc(_arr, _low, _high) if (!(isSortedDescending(_arr, _low, _high))) {cout << "\nERROR: array is not sorted in descending order\n"; dbArrLimit(_arr, _low, _high);}
#define time(_i) db1(_i);
#define avgTime(_i) db1((_i) / testCases);

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################

int32_t main() {
    ios_base::sync_with_stdio(false);

    int32_t setwLen = 11, setwLenRatio = 11;
    long double irSortRatio = 0, stdSortRatio = 0;
    cout << setprecision(13);

    auto start = high_resolution_clock::now(); // Get starting time
    auto stop = start;    // Get ending time
    nanoseconds duration; // Get duration. Subtract start time point to get duration. To cast it to proper unit use duration cast method

    int64_t minArrayLength = 0, maxArrayLength = 0, testCases = 0;
    cout << "Input: (minArrayLength, maxArrayLength, testCases) = ";
    cin >> minArrayLength >> maxArrayLength >> testCases;
    cout << endl;

    if (minArrayLength > maxArrayLength) {
        cout << "Enter valid minArrayLength and maxArrayLength." << endl;
        return 0;
    }

    // ARRAY
    // arr: used to check sorting time, it will get the values from the baseArray
    // baseArray: used to store the random array generated for each testcase
    vector<ArrayDataType> arr(static_cast<unsigned long>(maxArrayLength));
    vector<ArrayDataType> baseArray(static_cast<unsigned long>(maxArrayLength));

    rangeup(arrayLength, minArrayLength, maxArrayLength + 1) {
        cout << "#####################################################################";
        db1(arrayLength)
        db1(testCases)

        irSortRatio = 0, stdSortRatio = 0;

        int64_t timeArrLength = 2;
        vector<int64_t> timeArr(static_cast<unsigned long>(timeArrLength), 0);             // used to store the time taken for a particular sorting technique
        vector<int64_t> bestThreshold(static_cast<unsigned long>(timeArrLength), 0);       // used to keep track of the best THRESHOLD

        if(settings_PRINT_TIME_COMPARISON){
            cout << "\n\nTime taken in nano-seconds, Time ratio with respect to ir_sort";
            cout << endl << left << setw(setwLen) << "ir_sort" << "\t," << setw(setwLen) << "std::sort" << "\t,"
                 << setw(setwLenRatio) << "ratio ir_sort" << "\t," << setw(setwLenRatio) << "ratio std::sort";
        }

        rangeup(__, 0, testCases) {
            int64_t myTempLow = 0, myTempHigh = arrayLength - 1;

            // Fill up the array
            {
                if (settings_onlyPositiveNumbers == 0)
                    fillRandArray(baseArray, 0, arrayLength - 1, (-(1LL << myBits)) + 1, (1LL << myBits) - 1);
                else if (settings_onlyPositiveNumbers == 1)
                    fillRandArray(baseArray, 0, arrayLength - 1, 0, (1LL << myBits) - 1);
                else fillRandArray(baseArray, 0, arrayLength - 1, (-(1LL << myBits)) + 1, -1);
                // iota(&baseArray[0],&baseArray[arrayLength-1],1);
            }

            // index to insert data in timeArr
            int64_t timeArrIndex = 0;

            startTime
            ir_sort::radixSort_asc<ArrayDataType>(arr, myTempLow, myTempHigh, myTempHigh - myTempLow + 1, f1_getIndex);
            // integer_sort_small_obj(&arr[0] + myTempLow, &arr[0] + myTempHigh + 1, f1_getIndex, true);
            // integer_sort_small_obj(&arr[0] + myTempLow, &arr[0] + myTempHigh + 1, [](ArrayDataType &a) { return a.classGetIndex(); }, true);   // BEST way to sort
            // radixSort_Positive_asc<ArrayDataType>(arr, myTempLow, myTempHigh, myTempHigh - myTempLow + 1, [](ArrayDataType &a) { return a.classGetIndex(); });
            // merge_sort<ArrayDataType>(arr, myTempLow, myTempHigh, true, [](ArrayDataType &a) { return a.classGetIndex(); });
            endTime
            timeArr[timeArrIndex++] = duration.count();
            if (!isSorted(&arr[0], myTempLow, myTempHigh, f1_getIndex)) cout << endl << "ERROR: array1 not sorted :(";
            // objPrintArray(arr, myTempLow, myTempHigh);

            startTimeOnly
            ir_sort::radixSort_Positive_asc<ArrayDataType>(baseArray, myTempLow, myTempHigh, myTempHigh - myTempLow + 1, f1_getIndex);
            // radixSort_Positive_asc<ArrayDataType>(baseArray, myTempLow, myTempHigh, myTempHigh - myTempLow + 1, [](ArrayDataType &a) { return a.classGetIndex(); });
            // sort(&baseArray[0] + myTempLow, &baseArray[0] + myTempHigh + 1, f2_ascendingOrder);
            // startTime
            // sort(&arr[0] + myTempLow, &arr[0] + myTempHigh + 1, [](myClassInt &a, myClassInt &b) { return a.classGetIndex() < b.classGetIndex(); });
            // sort(&arr[0] + myTempLow, &arr[0] + myTempHigh + 1, [](myClassInt &a, myClassInt &b) { return a.classGetIndex() > b.classGetIndex(); });
            endTime
            timeArr[timeArrIndex++] = duration.count();
            // if (!isSorted(&arr[0], myTempLow, myTempHigh, f1_getIndex)) cout << endl << "ERROR: array not sorted :(";
            if (!isSorted(&baseArray[0], myTempLow, myTempHigh, f1_getIndex)) cout << endl << "ERROR: array2 not sorted :(";
            // objPrintArray(arr, myTempLow, myTempHigh);

            if(settings_PRINT_TIME_COMPARISON){
                cout << endl << left << setw(setwLen) << timeArr[0] << "\t," << setw(setwLen) << timeArr[1] << "\t,"
                     << setw(setwLenRatio) << timeArr[0] / timeArr[0] << "\t,"
                     << setw(setwLenRatio) << ((1.0 * timeArr[1]) / timeArr[0]);
            }
            irSortRatio += (timeArr[0] / timeArr[0]);
            stdSortRatio += (((1.0 * timeArr[1]) / timeArr[0]));

            if (!compareArray(begin(arr)+myTempLow, begin(arr)+myTempHigh+1, begin(baseArray))) { db1(__) };

            //#################################################
            bestThreshold[minIndex(timeArr, timeArrLength)]++;
        }

        cout << endl;
        dbArrSize(bestThreshold, timeArrLength);    // This will print the number of times ir_sort was fast and number of times std::sort was fast
        int64_t maxSpeedIndex = maxIndex(bestThreshold, timeArrLength);
        db1(bestThreshold[maxSpeedIndex])           // Number of times ir_sort was faster than std::sort out of total testCases or vice-versa

        if (maxSpeedIndex == 0) cout << "\n\nir_sort is faster than std::sort ";
        else cout << "\n\nstd::sort is faster than ir_sort ";
        cout << (bestThreshold[maxSpeedIndex] * 100.0 / testCases) << " % times, for testCases = " << testCases;

        cout << "\n\nir_sort average speed ratio = " << irSortRatio / testCases;
        cout << "\nstd::sort average speed ratio = " << stdSortRatio / testCases;

        cout << endl << endl;
    }

    return 0;
}


#pragma clang diagnostic pop