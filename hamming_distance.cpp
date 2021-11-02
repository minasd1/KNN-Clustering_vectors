#include "hamming_distance.h"


int hamming_distance(string s1, string s2){

    int hamming_distance = 0;

    for(int i = 0; i < s1.size(); i++){

        if(s1[i] != s2[i]){
            hamming_distance++;
        }
    }

    return hamming_distance;
}