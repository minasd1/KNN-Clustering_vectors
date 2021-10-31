#include "lsh.h"



//IMPLEMENT APPROXIMATE RANGE SEARCH ALGORITHM
vector<int> range_search(vector<int>& g, int radius, vector<int>& query_point){
    
    int retrieved_items = 0;
    int max_retrieved_items = 20* hashTable_get_num_of_htables();
    vector<int> points_in_range;
    
    //ACCESS TO I-TH HASHTABLE
    for(int i = 0; i < hashTable_get_num_of_htables(); i++){

        //ACCESS TO THE SPECIFIC BUCKET THAT I-TH G FUNCTION INDICATES
        for(int j = 0; j < hashTable_get_bucket_size(i, g[i]); j++){
            
            //IF DISTANCE OF J-TH POINT IN THIS BUCKET IS IN THE GIVEN RADIUS
            if((calculate_distance(query_point, point_vector_get_point(hashTable_get_point(i, g[i], j)), 2)) < radius){
                //THEN ADD IT'S ID TO POINTS_IN_RANGE VECTOR
                points_in_range.push_back(hashTable_get_point(i, g[i], j));
                retrieved_items++;

                if(retrieved_items > max_retrieved_items){

                    return points_in_range;
                }
            }
        }
    }

    return points_in_range;
}