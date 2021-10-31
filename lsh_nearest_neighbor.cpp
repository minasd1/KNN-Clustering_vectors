#include <iostream>
#include <vector>
#include <algorithm>
#include "vector_ops.h"
#include "hash_functions.h"
#include "hashTable.h"
#include "lsh_nearest_neighbor.h"

using namespace std;

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

//RECEIVES A QUERY POINT AND RETURNS THE FIRST k NEAREST NEIGHBORS IN ASCENDING DISTANCE ORDER
//THE LAST ARGUMENT IS OPTIONAL AND SHOWS THE MAXIMUM NUMBER OF POINTS TO BE EXAMINED AS POSSIBLE NEAREST NEIGHBORS
//IF NO FORTH ARGUMENT IS GIVEN THEN ALL THE POINTS IN THE SAME BUCKETS WITH query_point ARE EXAMINED
vector<dist_id_pair> find_approximate_knn(vector<int> query_point, int k, G& g, int max_candidates)// THIRD ARG IS THE OBJECT OF CLASS G DECLEARED IN MAIN FUNCTION
{
    int points_in_table_counter= 0; //A COUNTER OF THE ELEMENTS INSIDE THE nn_table
    vector<dist_id_pair> nn_table; //A TABLE IN WHICH THE PAIRS OF {DISTANCE, ID} OF THE NEAREST NEIGHBORING POINTS ARE STORED IN ASCENDIND DISTANCE ORDER
    vector<int> candidate_points; //A TABLE OF ALL THE POINTS THAT BELONG TO THE SAME BUCKET WITH THE query_point IN ANY OF THE L HASHTABLES
                                  //PERHAPS ONE POINT EXISTS MORE THAN ONCE IN THE candidate_points TABLE BECAUSE IT APPEARS IN MORE THAN ONE HASHTABLES
    vector<int> buckets_indexes; //THE L BUCKETS' INDEXES WHERE THE QUERY POINT WOULD BELONG IF IT WAS HASHED BY THE L G_HASH_FUNCTIONS
    float max_distance; //THE DISTANCE BETWEEN THE QUERY POINT AND IT'S k-th NEAREST NEIGHBORS
    float distance;
    int candidates_counter; //A COUNTER OF THE POINTS THAT HAVE BEEN COMPARED
    vector<int> current_candidate;
    dist_id_pair current_pair;

    //HASH THE QUERY POINT TO FIND THE BUCKETS WHERE IT WOULD BELONG
    g.hash(query_point, buckets_indexes, true);
    candidate_points= hashTable_get_points_in_buckets(buckets_indexes);
    for (candidates_counter=0; candidates_counter < candidate_points.size(); candidates_counter++) { //FOR EVERY CANDIDATE POINT
        if (candidates_counter == max_candidates) { //DONT COMPARE MORE THAN max_candidates POINTS
            break;
        }
        //GET THE CURRENT CANDIDATE POINT'S COORDINATES AND IT's DISTANCE FROM QUERY POINT
        current_candidate= point_vector_get_point(candidate_points[candidates_counter]-1);//-1 BECAUSE POINT_VECTOR STARTS FROM 0 WHILE POINTS' IDS START FROM 1
        distance= calculate_distance(query_point, current_candidate);
        //CREATE A PAIR WITH THESE TWO VALUES
        current_pair.dist= distance;
        current_pair.id= current_candidate[0];

        if (points_in_table_counter < k) { //THE FIRST k CANDIDATES TO COME ARE DIRECTLY PUSHED INTO THE NEAREST NEIGHBORS VECTOR
            if (!already_exist(nn_table, current_pair.id)) {
                nn_table.push_back(current_pair);
                points_in_table_counter++;
            }
            candidates_counter++;
        }
        else if (points_in_table_counter == k) { //WHEN THE nn_table HAS BEEN FILLED FULLY FOR THE FIRST TIME
            sort(nn_table.begin(), nn_table.end(), compare_distance); //SORT THE FIRST k PAIRS IN ASCENDING DISTANCE ORDER
            max_distance= nn_table[k-1].dist; //THE LAST ELEMENT OF THE nn_table HAS THE MAXIMUM DISTANCE FROM QUERY POINT
            if (current_pair.dist < max_distance) {
                insert_at_correct_place(nn_table, current_pair);
            }
            candidates_counter++;
        }
        else { //IF THE nn_table ALREADY HAS k ID-DISTANCE PAIRS
            if (current_pair.dist < max_distance) {
                insert_at_correct_place(nn_table, current_pair);
            }
            candidates_counter++;
        }
    }
    if (nn_table.size() < k) { //JUST IN CASE THERE ARE LESS THAN k CANDIDATE POINTS IN THE BUCKETS
        sort(nn_table.begin(), nn_table.end(), compare_distance);
    }
    return nn_table;
}
