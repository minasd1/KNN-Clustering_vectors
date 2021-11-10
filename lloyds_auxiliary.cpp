#include <vector>
#include "vector_ops.h"

using namespace std;


//RECEIVES A POINT AND RETURNS THE NEAREST CENTROID'S INDEX IN THE TABLE OF CENTROIDS
int find_nearest_centroid(vector<int>& current_point)
{
    int i, nearest_centroid;
    float distance, min_distance;
    vector<int> other_point;

    //SET AS MINIMUM DISTANCE THE ONE BETWEEN THE POINT AND THE FIRST CENTROID
    other_point= point_vector_get_point(get_centroids_id(0));
    min_distance= calculate_distance(current_point, other_point);
    nearest_centroid= 0;
    //FOR THE REST OF THE CENTROIDS
    for (i= 1 ; i < centroids_get_size() ; i++) {
        other_point= point_vector_get_point(get_centroids_id(i));
        distance= calculate_distance(current_point, other_point);
        if (distance < min_distance) {
            min_distance= distance;
            nearest_centroid= i;
        }
    }
    return nearest_centroid;
}

//RECEIVES THE CLUSTER TABLE, AN INDEX FOR THAT TABLE AND AN ID
//RETURNS TRUE WHEN THE POINT ,IN WHICH THE ID BELONGS TO,
//IS ALREADY IN THE CLUSTER WITH THE GIVEN INDEX OF THE GIVEN CLUSTER TABLE
bool already_in_that_cluster(vector<vector<int>> cluster_table, int index, int id)
{
    int i;
    for (i= 0; i < cluster_table[index].size() ; i++) {
        if (id == cluster_table[index][i]) {
            return true;
        }
    }
    return false;
}
