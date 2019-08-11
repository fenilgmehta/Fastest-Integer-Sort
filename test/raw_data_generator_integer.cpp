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

#include "../references/ska_sort.hpp"
#include "../src/integer_sort.hpp"

#include <iostream>
#include <iomanip>
#include "common_functions.hpp"

//#################################################
using namespace std;
using namespace std::chrono;

//#################################################
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
 * This macro will decide the sign of the numbers which are used to fill the array
 *
 * global_onlyPositiveNumbers = 0;      // +ve and -ve numbers
 * global_onlyPositiveNumbers = 1;      // +ve numbers
 * global_onlyPositiveNumbers = 2;      // -ve numbers
 *
 * */
// #define settings_onlyPositiveNumbers 1

/*
 * Number of bits to be used
 *
 * */
// const int32_t myBits = 63;

/*
 * Select the datatype of the array to be used for testing
 *
 * */
// using ArrayDataType = int8_t;
// using ArrayDataType = int16_t;
// using ArrayDataType = int32_t;
// using ArrayDataType = int64_t;
using ArrayDataType = uint64_t;

// ###################################################
// ################### SETTINGS end ##################
// ###################################################

inline int64_t f1_getIndex(const ArrayDataType &a) { return a; }

inline bool f2_ascendingOrder(const ArrayDataType &a, const ArrayDataType &b) { return a < b; }

inline bool f3_descendingOrder(const ArrayDataType &a, const ArrayDataType &b) { return a > b; }

inline bool f4_notEquals(const ArrayDataType &a, const ArrayDataType &b) { return a != b; }

//#########################################################################################################################################
//#########################################################################################################################################
//#########################################################################################################################################
#define m_END_TIME stop = high_resolution_clock::now(); duration = duration_cast<nanoseconds>(stop - start);
#define m_PRINT_TIME_TAKEN cout << endl << setw(columnWidth) << duration.count();
#define checkSortingRange_asc(_arr, _low, _high) if (!(isSortedAscending(_arr, _low, _high))) {cout << "\nERROR: array is not sorted in ascending order\n"; dblimit(_arr, _low, _high);}
#define checkSortingRange_desc(_arr, _low, _high) if (!(isSortedDescending(_arr, _low, _high))) {cout << "\nERROR: array is not sorted in descending order\n"; dblimit(_arr, _low, _high);}
#define m_ALL(name1) begin(name1), begin(name1)

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

    int64_t arrayLength, testCases;
    cerr << "Input: (arrayLength, testCases) = ";
    cin >> arrayLength >> testCases;
    cerr << endl;

    // ARRAY
    // arr: used to check sorting time, it will get the values from the baseArray
    // baseArray: used to store the random array generated for each testcase
    vector<ArrayDataType> arr(static_cast<unsigned long>(arrayLength));
    vector<ArrayDataType> baseArray(static_cast<unsigned long>(arrayLength));

    cerr << "#####################################################################" << endl;
    db(arrayLength);
    db(testCases);

    const vector<string> sorting_functions = {"ir_sort","std::sort","spreadsort","pdqsort","spinsort"/*,"flat_stable_sort"*/,"ska_sort"};

    int64_t timeArrLength = sorting_functions.size();

    // used to keep track of the best THRESHOLD
    vector<int64_t> bestThreshold(static_cast<unsigned long>(timeArrLength), 0);
    // used to keep track of the best THRESHOLD
    vector<double> sortSpeedRatio(static_cast<unsigned long>(timeArrLength), 0);

    if (settings_PRINT_TIME_COMPARISON) {
        cout << sorting_functions[0];
        for (int32_t i = 1; i < sorting_functions.size(); ++i) cout << "," << sorting_functions[i];
        cout<< endl;
    }

    urange(_no_use_, 0, testCases) {
        fillRandArray(begin(baseArray) + 0, begin(baseArray) + arrayLength - 1,
                      numeric_limits<ArrayDataType>::min(),
                      numeric_limits<ArrayDataType>::max());
        // sort(m_ALL(baseArray));
        // sort(m_ALL(arr));
        ir_sort::integer_sort_stable(m_ALL(arr));
        ir_sort::integer_sort_stable(m_ALL(baseArray));


        // used to store the time taken for a particular sorting technique
        vector<int64_t> timeArr;

        copy(begin(baseArray), end(baseArray), begin(arr));
        auto start = get_time();                             // Get starting time
        ir_sort::integer_sort_stable(m_ALL(arr));
        auto duration = get_time_difference(start);          // Get execution time.
        timeArr.push_back(duration.count());
        if (!is_sorted(m_ALL(arr))) cerr << endl << "ERROR: array \"" << timeArr.size() - 1 << "\" not sorted :(";

        copy(begin(baseArray), end(baseArray), begin(arr));
        start = get_time();
        std::sort(m_ALL(arr));
        duration = get_time_difference(start);
        timeArr.push_back(duration.count());
        if (!is_sorted(m_ALL(arr))) cerr << endl << "ERROR: array \"" << timeArr.size() - 1 << "\" not sorted :(";

        copy(begin(baseArray), end(baseArray), begin(arr));
        start = get_time();
        boost::sort::spreadsort::integer_sort(m_ALL(arr));  // BEST for 1000 <= size < 55000
        duration = get_time_difference(start);
        timeArr.push_back(duration.count());
        if (!is_sorted(m_ALL(arr))) cerr << endl << "ERROR: array \"" << timeArr.size() - 1 << "\" not sorted :(";

        copy(begin(baseArray), end(baseArray), begin(arr));
        start = get_time();
        boost::sort::pdqsort(m_ALL(arr));                   // good for size < 1000
        duration = get_time_difference(start);
        timeArr.push_back(duration.count());
        if (!is_sorted(m_ALL(arr))) cerr << endl << "ERROR: array \"" << timeArr.size() - 1 << "\" not sorted :(";

        copy(begin(baseArray), end(baseArray), begin(arr));
        start = get_time();
        boost::sort::spinsort(m_ALL(arr));
        duration = get_time_difference(start);
        timeArr.push_back(duration.count());
        if (!is_sorted(m_ALL(arr))) cerr << endl << "ERROR: array \"" << timeArr.size() - 1 << "\" not sorted :(";

        // copy(begin(baseArray), end(baseArray), begin(arr));
        // start = get_time();
        // boost::sort::flat_stable_sort(m_ALL(arr));
        // duration = get_time_difference(start);
        // timeArr.push_back(duration.count());
        // if (!is_sorted(m_ALL(arr))) cerr << endl << "ERROR: array \"" << timeArr.size() - 1 << "\" not sorted :(";

        copy(begin(baseArray), end(baseArray), begin(arr));
        start = get_time();
        ska_sort(m_ALL(arr));
        duration = get_time_difference(start);
        timeArr.push_back(duration.count());
        if (!is_sorted(m_ALL(arr))) cerr << endl << "ERROR: array \"" << timeArr.size() - 1 << "\" not sorted :(";

        if (settings_PRINT_TIME_COMPARISON) {
            cout << timeArr[0];
            for (int i = 1; i < timeArrLength; i++) cout << "," << timeArr[i];
            cout << endl;
        }
        for (int i = 0; i < timeArrLength; i++) sortSpeedRatio[i] += (1.0 * timeArr[i]) / timeArr[1];
        bestThreshold[minIndex(timeArr, timeArrLength)]++;
    }

    //##################################################################################################

    cerr << endl;
    dbiter("bestThreshold", begin(bestThreshold), end(bestThreshold)); // This will print the number of times each sort was fast
    int64_t maxSpeedIndex = maxIndex(bestThreshold, timeArrLength);
    db(bestThreshold[maxSpeedIndex]);             // Number of times ir_sort was faster than std::sort out of total testCases or vice-versa

    cerr << endl;
    cerr << "Percentage wise comparison of which sort was faster on an average:" << endl;
    cerr << "Speed ratio comparison:" << endl;

    int32_t summary_width = 20;
    for (const auto & sorting_function : sorting_functions) cerr << setw(summary_width) << sorting_function << ",";
    cerr << endl;
    for (int i = 0; i < timeArrLength; i++) cerr <<setw(summary_width-1)<< ((100.0 * bestThreshold[i]) / testCases) << "%,";
    cerr << endl;
    for (int i = 0; i < timeArrLength; i++) cerr <<setw(summary_width)<<setprecision(3) << ((1.0 * sortSpeedRatio[i]) / testCases) << ",";
    cerr << endl;

    cerr << endl;
    return 0;
}

