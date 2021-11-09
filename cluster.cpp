#include "cluster.h"
#include <limits>


void k_means_plus_plus(int k){

    int t = 1;                              //CURRENT NUMBER OF CENTROIDS

    vector<float> points_min_distances;     //EVERY NON CENTROID POINT MIN DISTANCE TO CENTROIDS
    vector <float> partial_sums;

    centroids_pick_first_centroid();        //INSERT FIRST CENTROID TO CENTROIDS VECTOR

    //CALCULATE DISTANCE OF ALL THE NON INPUT POINTS TO THE FIRST CENTROID
    centroids_calculate_min_distance_input(points_min_distances);

    //CALCULATE ALL THE PARTIAL SUMS NEEDED TO CHOOSE THE NEXT CENTROID
    calculate_partial_sums(points_min_distances, partial_sums);

    while(t < k){

        centroids_pick_next_centroid(partial_sums);
        t++;
    }

    centroids_print_data();

}

//RECEIVES A TABLE OF THE CLUSTERS. EACH ROW CORRESPENDS TO A CLUSTER
//IN EACH ROW ARE STORED THE IDS OF THE INPUT POINTS THAT BELONG TO THAT CLUSTER
//ALSO RECEIVES THE DIMENSION OF THE POINT VECTORS (HOW MANY COORDINATES EACH POINT HAS)
//RETURNS A TABLE WITH EACH CLUSTER'S NEW CENTROID
vector<vector<int>> update(vector<vector<int>> cluster_table, int dimensions)
{
    int row, column;  //ITERATORS
    vector<int> coordinates_sum;  //A TABLE OF THE SUMS OF THE 1ST, 2ND, ..., NTH COORDINATE OF THE POINTS IN THE SAME CLUSTER
    vector<int> current_point, mean_vector;
    vector<vector<int>> new_centroids;

    for (row=0 ; row < cluster_table.size() ; row++) { //FOR EACH CLUSTER
        coordinates_sum.assign(dimensions,0); //INITIALIZE ALL SUMS (ONE SUM FOR EACH COORDINATE) WITH 0
        for (column=0 ; column < cluster_table[row].size(); column++) { //FOR EVERY POINT IN THE CLUSTER
            current_point= point_vector_get_point((cluster_table[row][column])-1);
            coordinates_sum= add_vectors(current_point, coordinates_sum);
        }
        mean_vector= find_mean_vector(coordinates_sum, cluster_table[row].size());
        new_centroids.push_back(mean_vector);
    }
    return new_centroids;
}
