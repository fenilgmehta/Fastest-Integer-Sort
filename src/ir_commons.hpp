#ifndef IR_SORT_INTEGER_SORT_HPP
#define IR_SORT_INTEGER_SORT_HPP

#include <cstdint>

//*****************************************************************************************************************************************

namespace ir_sort{
    namespace integer_sort_common{

//############################################################################
//                                                                          ##
//                           C O U N T   B I T S                            ##
//                                                                          ##
//############################################################################

        template<typename T>
        inline int_fast16_t countBits_positive(T num) {
            int_fast16_t result = 0;

            while (num > 0) {
                num >>= 1;
                result += 1;
            }

            return result;
        }

        template<typename T>
        inline int_fast16_t countBits_negative(const T &negativeNum) {
            auto num = static_cast<uint_fast64_t>(-negativeNum);
            int_fast16_t result = 0;

            while (num > 0) {
                num >>= 1;
                result += 1;
            }

            return result;
        }

        template<typename T>
        inline int_fast16_t countBits(const T &num) {
            if (num < 0) return countBits_negative(num);
            return countBits_positive(num);
        }

//############################################################################
//                                                                          ##
//                       S E T   I T E R A T I O N S                        ##
//                                                                          ##
//############################################################################

        inline void set_BitShift_Iterations(const int_fast16_t &arrMax_bits, int_fast16_t &bitsShift, int_fast16_t &iterationsRequired) {
            if (arrMax_bits <= 8) {           //  8 bits = 1 iteration
                bitsShift = arrMax_bits;
                iterationsRequired = 1;
            } else if (arrMax_bits <= 10) {
                bitsShift = 5;
                iterationsRequired = 2;
            } else if (arrMax_bits <= 12) {
                bitsShift = 6;
                iterationsRequired = 2;
            } else if (arrMax_bits <= 14) {
                bitsShift = 7;
                iterationsRequired = 2;
            } else if (arrMax_bits <= 16) {   // 16 bits = 2 iterations
                bitsShift = 8;
                iterationsRequired = 2;
            } else if (arrMax_bits <= 18) {
                bitsShift = 6;
                iterationsRequired = 3;
            } else if (arrMax_bits <= 21) {
                bitsShift = 7;
                iterationsRequired = 3;
            } else if (arrMax_bits <= 24) {   // 24 bits = 3 iterations
                bitsShift = 8;
                iterationsRequired = 3;
            } else if (arrMax_bits <= 28) {
                bitsShift = 7;
                iterationsRequired = 4;
            } else if (arrMax_bits <= 32) {   // 32 bits = 4 iterations
                bitsShift = 8;
                iterationsRequired = 4;
            } else if (arrMax_bits <= 35) {
                bitsShift = 7;
                iterationsRequired = 5;
            } else if (arrMax_bits <= 40) {   // 40 bits = 5 iterations
                bitsShift = 8;
                iterationsRequired = 5;
            } else if (arrMax_bits <= 42) {
                bitsShift = 7;
                iterationsRequired = 6;
            } else if (arrMax_bits <= 48) {   // 48 bits = 6 iterations
                bitsShift = 8;
                iterationsRequired = 6;
            } else if (arrMax_bits == 49) {
                bitsShift = 7;
                iterationsRequired = 7;
            } else if (arrMax_bits <= 56) {   // 56 bits = 7 iterations
                bitsShift = 8;
                iterationsRequired = 7;
            } else if (arrMax_bits <= 64) {   // 64 bits = 8 iterations
                bitsShift = 8;
                iterationsRequired = 8;
            } else {                          // Handle the situation where int128_t of int256_t or int512_t or int1024_t or anything else is used to create the array / array index
                bitsShift = 8;
                iterationsRequired = ((arrMax_bits+1) >> 3);
            }
        }
    } // namespace integer_sort_common
} // namespace ir_sort


//*****************************************************************************************************************************************
#endif //IR_SORT_INTEGER_SORT_HPP
