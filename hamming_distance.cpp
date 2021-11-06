#include "hamming_distance.h"

//RETURNS THE HAMMING DISTANCE OF TWO STRINGS (STORED AS INTEGERS, E.G.: 17 -> 000...010001)
int hamming_distance(unsigned int s1, unsigned int s2)
{
    int i, hamming_distance, dividend, remainder, quotient;

    hamming_distance = 0;
    dividend= s1^s2;
    quotient= dividend/2;
    remainder= dividend%2;
    if (remainder == 1) {
        hamming_distance++;
    }
    dividend= quotient;
    while (quotient != 0) {
        quotient= dividend/2;
        remainder= dividend%2;
        if (remainder == 1) {
            hamming_distance++;
        }
        dividend= quotient;
    }
    return hamming_distance;
}
