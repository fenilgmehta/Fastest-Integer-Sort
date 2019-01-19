#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfor-loop-analysis"
#pragma ide diagnostic ignored "google-explicit-constructor"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma ide diagnostic ignored "OCSimplifyInspection"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

#include <bits/stdc++.h>
#include <boost/sort/sort.hpp>

#include "../src/integer_sort.cpp"
#include "../references/NewRadixGit.h"
#include "../references/NewRadixGitBoost.h"

// #include <iostream>
// #include <cstdint>

// #include <iomanip>
// #include <algorithm>
// #include <chrono>
// #include <cstring>
// #include <array>
// #include <vector>

// #include <type_traits>


//#################################################
using namespace std;
using namespace std::chrono;

//#################################################
#define endl '\n'
#define db1(x) cerr << "\033[1;31m" << "\nDebug: " << "\033[0m" << setw(30) << left << #x << " = " << (x);
#define db2(x, y) cerr << "\033[1;31m" << "\nDebug: " << "\033[0m" << #x << " = " << (x) << ",   " << #y << " = " << (y);
#define db3(x, y, z) cerr << "\033[1;31m" << "\nDebug: " << "\033[0m" << #x << " = " << (x) << ",   " << #y << " = " << (y) << ",   " << #z << " = " << (z);
#define dblimit(arr, l, h) cerr << "\nDebug: " << #arr << " [" << (l) << " : " << (h) << "] = "; for(int64_t i = (l); i <= (h); i++) cerr << (arr)[i] << ", ";
#define dbiter(name, first, len) cerr << "\033[1;31m" << "Debug: " << "\033[0m" << name << " = "; for(auto i_temp1 = 0, len_temp1 = (len), iter_temp1 = first; i_temp1 < len_temp1; ++i_temp1) cerr<<iter_temp1[i_temp1]<<", "; cerr<<endl;

//#################################################
#define rangeup(_i, _startLimit, _endLimit) for(int64_t (_i) = (_startLimit); (_i) < (_endLimit); (_i)++)
const int32_t columnWidth = 15;

using ArrayIndexType = int64_t;

// ###################################################
// ################# SETTINGS start ##################
// ###################################################

/*
 * This macro will decide whether to print the time comparison statistics or not
 * true  : will print
 * false : will not print the stats
 *
 * */
#define settings_PRINT_TIME_COMPARISON true


/*
 * Decide whether to sort the array in ascending order or descending order
 * true  : ascending order
 * false : descending order
 *
 * */
#define settings_SORT_IN_ASCENDING_ORDER true

/*
 * This macro will decide the sign of the numbers which are used to fill the array
 *
 * global_onlyPositiveNumbers = 0;      // +ve and -ve numbers
 * global_onlyPositiveNumbers = 1;      // +ve numbers
 * global_onlyPositiveNumbers = 2;      // -ve numbers
 *
 * */
#define settings_onlyPositiveNumbers 0

/*
 * Number of bits to be used
 *
 * */
const int32_t myBits = 62;

/*
 * Select the datatype of the array to be used for testing
 *
 * */
// using ArrayDataType = int8_t;
// using ArrayDataType = int16_t;
// using ArrayDataType = int32_t;
using ArrayDataType = int64_t;

// ###################################################
// ################### SETTINGS end ##################
// ###################################################

inline int64_t f1_getIndex(const ArrayDataType &a){ return a; }
inline bool f2_ascendingOrder(const ArrayDataType &a, const ArrayDataType &b){ return a < b; }
inline bool f3_descendingOrder(const ArrayDataType &a, const ArrayDataType &b){ return a > b; }
inline bool f4_notEquals(const ArrayDataType &a, const ArrayDataType &b) { return a != b; }

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
template<typename T>
int64_t maxIndex(const T &arr, const int64_t &size) {
    int64_t index = 0;
    for(int64_t i=1; i<size; i++) if (arr[i] > arr[index]) index = i;
    return index;
}

template<typename T>
int64_t minIndex(const T &arr, const int64_t &size) {
    int64_t index = 0;
    for(int64_t i=1; i<size; i++) if (arr[i] < arr[index]) index = i;
    return index;
}

template<typename RandomAccessIterator>
bool isSorted(RandomAccessIterator first, const RandomAccessIterator last, const bool &checkAscendingOrder = settings_SORT_IN_ASCENDING_ORDER) {
    first++;
    if (checkAscendingOrder) {
        while(first != last){
            if(*next(first, -1) > *first) return false;
            first++;
        }
    } else {
        while(first != last){
            if(*next(first, -1) < *first) return false;
            first++;
        }
    }
    return true;
}

template<typename RandomAccessIterator>
bool compareArray(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator second){
    RandomAccessIterator it_first = first, it_second = second;
    int index = 0;

    while(it_first != last){
        if((*it_first)!=(*it_second)){
            cout << "\n\nERROR: arrays not equal";
            db3(index, *it_first, *it_second)
            dbiter("arr1[]", first, distance(first, last))
            dbiter("arr2[]", second, distance(first, last))
            return false;
        }
        ++index;
        ++it_first;
        ++it_second;
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
#define m_START_TIME_ONLY start = high_resolution_clock::now();
#define m_START_TIME copy(&baseArray[0], &baseArray[0] + arrayLength, &arr[0]); start = high_resolution_clock::now();
#define m_END_TIME stop = high_resolution_clock::now(); duration = duration_cast<nanoseconds>(stop - start);
#define m_PRINT_TIME_TAKEN cout << endl << setw(columnWidth) << duration.count();
#define checkSortingRange_asc(_arr, _low, _high) if (!(isSortedAscending(_arr, _low, _high))) {cout << "\nERROR: array is not sorted in ascending order\n"; dblimit(_arr, _low, _high);}
#define checkSortingRange_desc(_arr, _low, _high) if (!(isSortedDescending(_arr, _low, _high))) {cout << "\nERROR: array is not sorted in descending order\n"; dblimit(_arr, _low, _high);}
// #define m_ALL(name1) &name1[0]+myTempLow, &name1[0]+myTempHigh+1
#define m_ALL(name1) begin(name1),end(name1)

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

    for(int64_t arrayLength=minArrayLength; arrayLength<=maxArrayLength; arrayLength++){
        cout << "#####################################################################";
        db1(arrayLength)
        db1(testCases)

        irSortRatio = 0, stdSortRatio = 0;

        int64_t timeArrLength = 2;
        deque<int64_t> timeArr(static_cast<unsigned long>(timeArrLength), 0);             // used to store the time taken for a particular sorting technique
        deque<int64_t> bestThreshold(static_cast<unsigned long>(timeArrLength), 0);       // used to keep track of the best THRESHOLD

        if(settings_PRINT_TIME_COMPARISON){
            cout << "\n\nTime taken in nano-seconds, Time ratio with respect to ir_sort";
            cout << endl << left
                 << setw(setwLen) << "ir_sort" << "\t," << setw(setwLen) << "std::sort" << "\t,"
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
                else
                    fillRandArray(baseArray, 0, arrayLength - 1, (-(1LL << myBits)) + 1, -1);
                // iota(&baseArray[0],&baseArray[arrayLength-1],1);
            }

            // index to insert data in timeArr
            int64_t timeArrIndex = 0;

            m_START_TIME
            // ir_sort::integer_sort(m_ALL(arr), true);
            ir_sort::integer_sort_new(m_ALL(arr), true);
            m_END_TIME
            timeArr[timeArrIndex++] = duration.count();
            if (!isSorted(m_ALL(arr))) cerr << endl << "ERROR: array1 not sorted :(";
            // printArray(arr, myTempLow, myTempHigh);

            m_START_TIME_ONLY
            kx::radix_sort(m_ALL(baseArray)); // GREAT from 90 to 3000 // this is NOT stable sorting
            // ska_sort(m_ALL(baseArray));
            // sort(m_ALL(baseArray));
            // boost::sort::spreadsort::integer_sort(m_ALL(baseArray)); // BEST for 1000 <= size < 55000
            // boost::sort::pdqsort(m_ALL(baseArray)); // good for size < 1000
            m_END_TIME
            timeArr[timeArrIndex] = duration.count();
            // checkSortingRange_asc(baseArray, myTempLow, myTempHigh)
            // checkSortingRange_desc(baseArray, myTempLow, myTempHigh)
            // if (!isSorted(&arr[0], myTempLow, myTempHigh)) cout << endl << "ERROR: array not sorted :(";
            if (!isSorted(m_ALL(baseArray))) cerr << endl << "ERROR: array2 not sorted :(";
            // printArray(arr, myTempLow, myTempHigh);

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
        dblimit(bestThreshold,0,timeArrLength-1); // This will print the number of times ir_sort was fast and number of times std::sort was fast
        int64_t maxSpeedIndex = maxIndex(bestThreshold, timeArrLength);
        db1(bestThreshold[maxSpeedIndex])         // Number of times ir_sort was faster than std::sort out of total testCases or vice-versa

        if (maxSpeedIndex == 0) cout << "\n\nir_sort is faster than std::sort ";
        else cout << "\n\nstd::sort is faster than ir_sort ";
        cout << (bestThreshold[maxSpeedIndex] * 100.0 / testCases) << " % times, for testCases = " << testCases << endl;

        cout << "\nir_sort average speed ratio = " << irSortRatio / testCases;
        cout << "\nstd::sort average speed ratio = " << stdSortRatio / testCases;

        cout << endl << endl;
    }

    return 0;
}

