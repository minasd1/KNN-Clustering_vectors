#include <vector>
#include <iostream>
#include "hypercube.h"
#include "hash_functions.h"

using namespace std;

//RECEIVES THE MAXIMUM NUMBER OF CUBE VERTICES (probes) TO BE SEARCHED FOR NEAREST NEIGHBORS
//AND THE INITIAL VERTEX
//RETURNS A TABLE OF VERTICES (HASHTABLE INDEXES) IN ASCENDING HAMMING DISTANCE
//THAT CONTAINS probes ELEMENTS
vector<unsigned int> get_relative_buckets(unsigned int current_vertex,
                                          int probes, int string_dimensions) //LAST ARG IS d'
{
    int i, j;
    int num_of_buckets;

    num_of_buckets= pow(2,string_dimensions);

    for(i=1; i <= string_dimensions ; i++) {
        for (j=0; j < num_of_buckets) {
            if ()
        }
    }
}

//RECEIVES A QUERY POINT AND RETURNS THE FIRST k NEAREST NEIGHBORS IN ASCENDING DISTANCE ORDER
//THE LAST ARGUMENT IS OPTIONAL AND SHOWS THE MAXIMUM NUMBER OF POINTS TO BE EXAMINED AS POSSIBLE NEAREST NEIGHBORS
//IF NO FORTH ARGUMENT IS GIVEN THEN ALL THE POINTS IN THE SAME BUCKETS WITH query_point ARE EXAMINED
vector<dist_id_pair> find_approximate_knn(vector<int> query_point, int k, G_Hypercube& g,
                                          int probes, int string_dimensions, int max_candidates)
{
    int i;
    unsigned int querys_bucket;
    vector<int> candidate_points;
    vector<int> points_from_relative_buckets;
    vector<unsigned int> relative_buckets_indexes;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //GET THE HASH VALUE (HYPERCUBE'S HASH TABLE INDEX) OF THE QUERY POINT
    g.hash(query_point, querys_bucket, true);
    //BRING ALL THE POINTS THAT THE QUERY'S BUCKET CONTAINS IN A TABLE
    candidate_points= hyperCube_get_points_in_bucket(querys_bucket);

    //EXPAND THAT TABLE TO ALSO CONTAIN ALL THE POINTS
    //FROM THE RELATIVE BUCKETS (WITH LOW HAMMING DISTANCE)
    relative_buckets_indexes= get_relative_buckets(querys_bucket, probes, string_dimensions);
    for (i=0; i < relative_buckets_indexes.size(); i++) {//FOR EVERY RELATIVE BUCKET
        //GET THE POINTS IT CONTAINS
        points_from_relative_buckets= hyperCube_get_points_in_bucket(querys_bucket);
        //AND PUT THEM INTO THE candidate_points TABLE
        candidate_points.insert(candidate_points.end(), points_from_relative_buckets.begin(),
                                points_from_relative_buckets.end());
    }
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //
    //FOR ALL THE CANDIDATE POINTS
    for (i=0; i < candidate_points.size() ; i+=2) { //FOR EVERY CANDIDATE POINT
        if (candidates_counter == max_candidates) { //DONT COMPARE MORE THAN max_candidates POINTS
            break;
        }
        //GET THE CURRENT CANDIDATE POINT'S COORDINATES AND IT's DISTANCE FROM QUERY POINT
        current_candidate= point_vector_get_point(candidate_points[i+1]-1);//+1 BECAUSE candidate_points IS [HASHTABLE, POINT_INDEX, HASHTABLE, ...],
                                                                                        //-1 BECAUSE POINT_VECTOR STARTS FROM 0 WHILE POINTS' IDS START FROM 1
        g.id(current_candidate, current_candidate_ids, true); //ALTHOUGH current_candidate IS AN INPUT POINT WE USE 'true' SO THAT THE TABLE WITH INPUT POINTS IDS WONT HAVE DUBLICATES

        //IF QUERY_POINT'S ID != CANDIDATE_POINT'S ID, BUT EXIST AT LEAST ONE CANDIDATE WITH SAME ID
        if (current_candidate_ids[candidate_points[i]] != query_points_ids[candidate_points[i]]
            && at_least_one_candidate_has_same_id) {
            candidates_counter++;
            continue; //DONT CHECK POINTS WITH DIFFRENT ID UNLESS NONE OF THEM HAS THE SAME ID WITH QUERY_POINT
        }
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


}
