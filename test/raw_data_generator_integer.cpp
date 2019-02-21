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
#include <boost/sort/spreadsort/float_sort.hpp>

// #include <iostream>
// #include <cstdint>

// #include <iomanip>
// #include <algorithm>
// #include <chrono>
// #include <cstring>
// #include <array>
// #include <vector>

// #include <type_traits>

#include "../src/integer_sort.hpp"

//#################################################
using namespace std;
using namespace std::chrono;

//#################################################
#define endl '\n'
#define db1(x) cerr << "\033[1;31m" << "Debug: " << "\033[0m" << setw(30) << left << #x << " = " << (x) << endl;
#define db2(x, y) cerr << "\033[1;31m" << "Debug: " << "\033[0m" << #x << " = " << (x) << ",   " << #y << " = " << (y) << endl;
#define db3(x, y, z) cerr << "\033[1;31m" << "Debug: " << "\033[0m" << #x << " = " << (x) << ",   " << #y << " = " << (y) << ",   " << #z << " = " << (z) << endl;
#define dblimit(arr, l, h) cerr << "\033[1;31m" << "Debug: " << "\033[0m" << #arr << " [" << (l) << " : " << (h) << "] = "; for(int64_t i = (l); i <= (h); i++) cerr << (arr)[i] << ", "; cerr << endl;
#define dbiter(name, first, len) cerr << "\033[1;31m" << "Debug: " << "\033[0m" << name << " = "; for(auto i_temp1 = 0, len_temp1 = (len), iter_temp1 = first; i_temp1 < len_temp1; ++i_temp1) cerr<<iter_temp1[i_temp1]<<", "; cerr<<endl;

//#################################################
#define rangeup(_i, _startLimit, _endLimit) for(int64_t (_i) = (_startLimit); (_i) < (_endLimit); (_i)++)
const int32_t columnWidth = 15;


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
const int32_t myBits = 63;

/*
 * Select the datatype of the array to be used for testing
 *
 * */
// using ArrayDataType = int8_t;
// using ArrayDataType = int16_t;
// using ArrayDataType = int32_t;
// using ArrayDataType = int64_t;
using ArrayDataType = uint64_t;
using ArrayIndexType = ArrayDataType;

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
            ++first;
        }
    } else {
        while(first != last){
            if(*next(first, -1) < *first) return false;
            ++first;
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
#define m_ALL(name1) next(begin(name1),myTempLow), next(begin(name1),myTempHigh+1)

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################

int32_t main() {
/*
    ### OUTPUT format: csv file format. Use shell redirection to write to .csv file
    1. first line will contain the name of the sorting used
    2. following lines will have the time taken in nano-seconds

*/
    ios_base::sync_with_stdio(false);

    auto start = high_resolution_clock::now(); // Get starting time
    auto stop = start;    // Get ending time
    nanoseconds duration; // Get duration. Subtract start time point to get duration. To cast it to proper unit use duration cast method

    int64_t arrayLength = 0, testCases = 0;
    cerr << "Input: (arrayLength, testCases) = ";
    cin >> arrayLength >> testCases;
    cerr << endl;

    // ARRAY
    // arr: used to check sorting time, it will get the values from the baseArray
    // baseArray: used to store the random array generated for each testcase
    vector<ArrayDataType> arr(static_cast<unsigned long>(arrayLength));
    vector<ArrayDataType> baseArray(static_cast<unsigned long>(arrayLength));

    cerr << "#####################################################################" << endl;
    db1(arrayLength)
    db1(testCases)

    int64_t timeArrLength = 6;
    vector<int64_t> timeArr(static_cast<unsigned long>(timeArrLength), 0);             // used to store the time taken for a particular sorting technique
    vector<int64_t> bestThreshold(static_cast<unsigned long>(timeArrLength), 0);       // used to keep track of the best THRESHOLD
    vector<double> sortSpeedRatio(static_cast<unsigned long>(timeArrLength), 0);       // used to keep track of the best THRESHOLD

    if(settings_PRINT_TIME_COMPARISON){
        cout<<"ir_sort,std::sort,boost::sort::spreadsort::integer_sort,boost::sort::pdqsort,boost::sort::spinsort,boost::sort::flat_stable_sort"<<endl;
    }

    rangeup(__, 0, testCases) {
        int64_t myTempLow = 0, myTempHigh = arrayLength - 1;

        // Fill up the array
        {
            if (settings_onlyPositiveNumbers == 0)
                fillRandArray(baseArray, 0, arrayLength - 1, (-(1uLL << myBits)) + 1, (1uLL << myBits) - 1);
            else if (settings_onlyPositiveNumbers == 1)
                fillRandArray(baseArray, 0, arrayLength - 1, 0, (1uLL << myBits) - 1);
            else
                fillRandArray(baseArray, 0, arrayLength - 1, (-(1uLL << myBits)) + 1, -1);
        }

        // index to insert data in timeArr
        int64_t timeArrIndex = 0;

        m_START_TIME
        ir_sort::integer_sort_stable(m_ALL(arr), true);
        m_END_TIME
        timeArr[timeArrIndex++] = duration.count();
        if (!isSorted(begin(arr)+myTempLow, begin(arr)+myTempHigh)) cerr << endl << "ERROR: array \""<<timeArrIndex<<"\" not sorted :(";

        m_START_TIME
        std::sort(m_ALL(arr));
        m_END_TIME
        timeArr[timeArrIndex++] = duration.count();
        // if (!isSorted(&arr[0], myTempLow, myTempHigh)) cerr << endl << "ERROR: array \""<<timeArrIndex<<"\" not sorted :(";

        m_START_TIME
        boost::sort::spreadsort::integer_sort(m_ALL(arr));  // BEST for 1000 <= size < 55000
        m_END_TIME
        timeArr[timeArrIndex++] = duration.count();
        // if (!isSorted(&arr[0], myTempLow, myTempHigh)) cerr << endl << "ERROR: array \""<<timeArrIndex<<"\" not sorted :(";

        m_START_TIME
        boost::sort::pdqsort(m_ALL(arr));                   // good for size < 1000
        m_END_TIME
        timeArr[timeArrIndex++] = duration.count();
        // if (!isSorted(&arr[0], myTempLow, myTempHigh)) cerr << endl << "ERROR: array \""<<timeArrIndex<<"\" not sorted :(";

        m_START_TIME
        boost::sort::spinsort(m_ALL(arr));
        m_END_TIME
        timeArr[timeArrIndex++] = duration.count();
        // if (!isSorted(&arr[0], myTempLow, myTempHigh)) cerr << endl << "ERROR: array \""<<timeArrIndex<<"\" not sorted :(";

        m_START_TIME
        boost::sort::flat_stable_sort(m_ALL(arr));
        m_END_TIME
        timeArr[timeArrIndex++] = duration.count();
        // if (!isSorted(&arr[0], myTempLow, myTempHigh)) cerr << endl << "ERROR: array \""<<timeArrIndex<<"\" not sorted :(";

        if(settings_PRINT_TIME_COMPARISON){
            cout<<timeArr[0];
            for(int i = 1; i < timeArrLength; i++) cout<<","<<timeArr[i];
            cout<<endl;
        }
        for(int i = 0; i < timeArrLength ; i++) sortSpeedRatio[i] += (1.0*timeArr[i]) / timeArr[0];
        bestThreshold[minIndex(timeArr, timeArrLength)]++;
    }

    //##################################################################################################

    cerr << endl;
    dblimit(bestThreshold,0,timeArrLength-1); // This will print the number of times each sort was fast
    int64_t maxSpeedIndex = maxIndex(bestThreshold, timeArrLength);
    db1(bestThreshold[maxSpeedIndex])         // Number of times ir_sort was faster than std::sort out of total testCases or vice-versa

    cerr << "Percentage wise comparison of which sort was faster on an average:" << endl;
    for(int i = 0; i < timeArrLength; i++) cerr << ((100.0*bestThreshold[i]) / testCases) << endl;

    cerr << "Speed ratio comparison:" << endl;
    for(int i = 0; i < timeArrLength; i++) cerr << ((1.0 * sortSpeedRatio[i]) / testCases) << endl;

    return 0;
}

