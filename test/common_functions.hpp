#include <iostream>
#include <random>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <chrono>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
using namespace std;
using namespace std::chrono;


struct {
    const std::string red = "\033[1;31m";
    const std::string green = "\033[1;32m";
    const std::string yellow = "\033[1;33m";
    const std::string cyan = "\033[1;36m";
    const std::string magenta = "\033[1;35m";
    const std::string reset = "\033[0m";
    std::string error_color = red;
} Colors;

#define endl '\n'

#define printFunction(outStream, functionName, argDelimiter, lineDelimiter) template <typename Arg, typename... Args> inline void functionName(Arg&& arg, Args&&... args) { outStream << arg; (void)(int[]){0, (void(outStream << argDelimiter << args),0)...}; outStream << lineDelimiter; }
printFunction(cerr, printErr, Colors.error_color << " , " << "\033[0m", '\n');
#undef printFunction
inline void printErr() { cerr << Colors.error_color << "stderr: -----------------" << "\033[0m" << "\n"; }

template<class T, class... U>
void dbg(const char *sdbg, T h, U... a) {
    cerr << Colors.error_color << "Debug: " << "\033[0m";
    cerr << sdbg;
    cerr << Colors.error_color << " = " << "\033[0m";
    printErr(h, a...);
}

#define db(...) dbg(#__VA_ARGS__, __VA_ARGS__)
#define dblimit(arr, l, h) {cerr << Colors.error_color << "Debug: " << "\033[0m" << #arr << " [" << (l) << " : " << (h) << "] = "; for(int64_t i = (l); i <= (h); i++) cerr << (arr)[i] << ", "; cerr << endl;}
#define dbiter(name, first, last) {cerr << Colors.error_color << "Debug: " << "\033[0m" << name << " = "; for(auto iter_temp1 = first, iter_temp2 = last; iter_temp1 != iter_temp2; ++iter_temp1) cerr<<(*iter_temp1)<<", "; cerr<<endl;}

#define urange(_i, _startLimit, _endLimit) for(int64_t (_i) = (_startLimit); (_i) < (_endLimit); ++(_i))
#define ulimit(_i, _startLimit, _endLimit) for(int64_t (_i) = (_startLimit); (_i) <= (_endLimit); ++(_i))
#define dlimit(_i, _startLimit, _endLimit) for(int64_t (_i) = (_startLimit); (_i) >= (_endLimit); --(_i))

// ------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------

inline auto get_time(){
    return high_resolution_clock::now();
}

template<typename T>
inline auto get_time_difference(T start, T stop = get_time()){
    return duration_cast<nanoseconds>(stop - start);
}

template<typename RandomAccessIterator, typename ArrayDataType>
void fillRandArray(RandomAccessIterator first, RandomAccessIterator last, const ArrayDataType randStart,
                   const ArrayDataType randEnd) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    // std::uniform_int_distribution<ArrayDataType> dis(randStart, randEnd);
    // for (; first != last; ++first) (*first) = dis(gen);

    auto generator_func = std::bind(uniform_int_distribution<ArrayDataType>(randStart, randEnd), gen);
    // auto generator_func = std::bind(uniform_int_distribution<ArrayDataType>(randStart, randEnd), default_random_engine{});

    generate(first, last, generator_func);
}

template<typename RandomAccessIterator>
bool compareArray(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator second) {
    RandomAccessIterator it_first = first, it_second = second;
    int index = 0;

    while (it_first != last) {
        if ((*it_first) != (*it_second)) {
            cout << "\n\nERROR: arrays not equal";
            db(index, *it_first, *it_second);
            dbiter("arr1[]", first, last);
            dbiter("arr2[]", second, last);
            return false;
        }
        ++index;
        ++it_first;
        ++it_second;
    }
}

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

/*
template<typename T>
void printArray(T &arr, int64_t low, int64_t high) {
    cout << endl;
    for (int64_t i = low; i <= high; i++) cout << arr[i] << ", ";
    cout << endl;
}
*/



/*int main(){
    db(1);
    vector<int> a(10);
    fillRandArray(begin(a), end(a), 1, 100);
    dbiter("a[:]", begin(a), end(a));
}*/

#pragma clang diagnostic pop