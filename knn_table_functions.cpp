#include <iostream>
#include <algorithm>
#include "knn_table_functions.h"



// COMPARES TWO PAIRS ACCORDING TO DISTANCE (TRUE WHEN THE SECOND'S DISTANCE IS GREATER)
bool compare_distance(const dist_id_pair& pair1, const dist_id_pair& pair2)
{
    return (pair1.dist < pair2.dist);
}

//CHECKS IF THE ID ALREADY EXISTS IN THE VECTOR
bool already_exist(const vector<dist_id_pair>& table, int id)
{
    int i;

    for (i=0; i < table.size() ; i++) {
        if (table[i].id == id) {
            return true;
        }
    }
    return false;
}

//INSERTS A NEW PAIR IN THE CORRECT PLACE IN A SORTED TABLE OF PAIRS AND REMOVES THE LAST (k+1th) PAIR FROM THE TABLE
void insert_at_correct_place(vector<dist_id_pair>& table, dist_id_pair& pair)
{
    int i, j, last_index;
    dist_id_pair dummy_pair;

    dummy_pair.dist=0.0;
    dummy_pair.id= -1;
    i= 0;
    while (table[i].dist < pair.dist) {
        i++;
    }

    table.push_back(dummy_pair);
    for (j=table.size()-1; j >= i ; j--) {
        table[j+1].dist= table[j].dist;
        table[j+1].id= table[j].id;
    }
    table.pop_back();
    table[i].dist= pair.dist;
    table[i].id= pair.id;
}
