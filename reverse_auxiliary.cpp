#include "reverse_auxiliary.h"


//RETURN ALL POINTS THAT ARE INSIDE THE GIVEN RANGE OF QUERY
vector<int> reverse_cube_range_search(int g, int radius, int probes, int dimensions, vector<int>& query_point){

    int retrieved_items = 0;
    int max_retrieved_items = 100;

    //A VECTOR WITH ALL THE RELATIVE BUCKETS OF QUERY POINT IN HYPERCUBE
    vector <unsigned int> relative_buckets_indexes;
    relative_buckets_indexes = get_relative_buckets(g, probes, dimensions);

    //POINTS THAT ARE IN THE GIVEN RADIUS
    vector<int> points_in_range;
    
    //FOR ALL THE ELEMENTS IN QUERY POINT'S BUCKET (INDICATED BY G)
    for(int i = 0; i < hyperCube_get_bucket_size(g); i++){
        //IF INPUT POINT IS NOT THE SAME AS THE QUERY POINT AND DOES NOT ALREADY EXIST IN POINTS IN RANGE
        if((query_point[0] != hyperCube_get_point(g, i)) 
        && (!already_exists(points_in_range, hyperCube_get_point(g, i))) 
        && (!already_assigned(hyperCube_get_point(g, i)-1))){ //ALSO CHECK THAT IT IS NOT ASSIGNED ALREADY - CLUSTERING

            //IF DISTANCE BETWEEN QUERY POINT AND A POINT IN BUCKET IS IN THE GIVEN RADIUS
            if(calculate_distance(query_point, point_vector_get_point(hyperCube_get_point(g, i) - 1), 2) < radius){
                //ADD THE POINT TO POINTS IN RANGE
                points_in_range.push_back(hyperCube_get_point(g, i));
                retrieved_items++;
    
                if(retrieved_items == max_retrieved_items){

                    return points_in_range;
                }
            }
        }
        
    
    }
    
    //FOR ALL THE RELATIVE BUCKETS OF QUERY POINT
    for(int i = 0; i < relative_buckets_indexes.size(); i++){
        //FOR ALL THEIR ELEMENTS
        for(int j = 0; j < hyperCube_get_bucket_size(relative_buckets_indexes[i]); j++){
            //IF INPUT POINT IS NOT THE SAME AS THE QUERY POINT AND DOES NOT ALREADY EXIST IN POINTS IN RANGE
            if((query_point[0] != hyperCube_get_point(g, i))
            && (!already_exists(points_in_range, hyperCube_get_point(relative_buckets_indexes[i], j))) 
            && (!already_assigned(hyperCube_get_point(relative_buckets_indexes[i], j)-1))){ //ALSO CHECK THAT IT IS NOT ASSIGNED ALREADY - CLUSTERING
                //IF DISTANCE BETWEEN QUERY POINT AND A POINT IN RELATIVE BUCKET IS IN THE RADIUS
                if(calculate_distance(query_point, point_vector_get_point(hyperCube_get_point(relative_buckets_indexes[i], j) - 1), 2) < radius){
                    //ADD IT TO POINTS IN RANGE
                    points_in_range.push_back(hyperCube_get_point(relative_buckets_indexes[i], j));
                    retrieved_items++;

                    if(retrieved_items == max_retrieved_items){

                        return points_in_range;
                    }
                }
            }
        }
    }

    return points_in_range;
}
