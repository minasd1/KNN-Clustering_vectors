#include <fstream>
#include <limits>
#include <chrono>
#include "cluster.h"
#include "lloyds_auxiliary.h"
#include "silhouette.h"
#include "vector_ops.h"

using namespace std;

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
void update(vector<vector<int>>& cluster_table, vector<int>& centroids_ids, int& last_known_id)
{
    int row, column;  //ITERATORS
    int dimensions; //THE NUMBER OF COORDINATES AN INPUT POINT HAS
    vector<int> coordinates_sum;  //A TABLE OF THE SUMS OF THE 1ST, 2ND, ..., NTH COORDINATE OF THE POINTS IN THE SAME CLUSTER
    vector<int> current_point, mean_vector;
    vector<vector<int>> new_centroids;

    dimensions= point_vector_get_point(1).size();
    //CLEAR THE CENTROIDS VECTOR SO THE NEW CENTROIDS CAN BE PUSHED BACK IN IT
    centroids_ids.clear();

    for (row=0 ; row < cluster_table.size() ; row++) { //FOR EACH CLUSTER
        coordinates_sum.assign(dimensions, 0); //INITIALIZE ALL SUMS (ONE SUM FOR EACH COORDINATE) WITH 0
        for (column=0 ; column < cluster_table[row].size(); column++) { //FOR EVERY POINT IN THE CLUSTER
            current_point= point_vector_get_point((cluster_table[row][column])-1);
            coordinates_sum= add_vectors(current_point, coordinates_sum);
        }
        mean_vector= find_mean_vector(coordinates_sum, cluster_table[row].size());
        mean_vector[0]= ++last_known_id;
        point_vector_insert_point(mean_vector);
        centroids_ids.push_back(mean_vector[0]);
    }
    set_centroids_id(centroids_ids);
}

/*//IMPLEMENTATION OF THE LLOYDS ALGORITHM
void lloyds(int number_of_clusters, int num_of_points, fstream& output_file, bool complete_flag)
{
    int i, dimensions, j;
    int nearest_centroid; //NEAREST CENTROID'S INDEX IN THE centroid TABLE
    int changes_made= 0; //HOW MANY POINTS CHANGED CLUSTER IN A NEW ASSIGNMENT
    vector<int> current_point;
    vector<vector<int>> new_cluster_table, previous_cluster_table;
    float change_rate; //THE RATE OF POINTS THAT CHANGED CLUSTER TO THE TOTAL NUMBER OF POINTS
    int last_known_id= num_of_points-1;

    dimensions= point_vector_get_point(1).size();
    previous_cluster_table.resize(number_of_clusters);
    //START COUNTING TIME
    auto start_time = std::chrono::high_resolution_clock::now();
    //INITIALIZE THE CENDROID POINTS
    k_means_plus_plus(number_of_clusters);
    //ASSIGN POINTS IN CLUSTERS FOR THE FIRST TIME
    for (i=0 ; i < num_of_points ; i++) { //FOR EVERY POINT
        current_point= point_vector_get_point(i);
        nearest_centroid= find_nearest_centroid(current_point);
        previous_cluster_table[nearest_centroid].push_back(current_point[0]);
        changes_made++;
    }
    change_rate= float(changes_made)/float(num_of_points);
    //LOOP UNTIL A SMALL PERCENTAGE OF POINTS CHANGE CLUSTER
    int kl=0;
    while (change_rate > 0.1) {
        cout << "ENTER " << kl++ << endl;
        //UPDATE THE CENTROIDS
        cout << "Before update :" << endl;
        centroids_print_data();
        update(previous_cluster_table, centroids, last_known_id);
        cout << "After update :" << endl;
        centroids_print_data();
        changes_made= 0;
        new_cluster_table.clear();
        new_cluster_table.resize(number_of_clusters);
        //MAKE A NEW ASSIGNMENT FOR ALL THE POINTS
        for (i=0 ; i < num_of_points ; i++) { //FOR EVERY POINT
            current_point= point_vector_get_point(i);
            nearest_centroid= find_nearest_centroid(current_point);
            if (!already_in_that_cluster(previous_cluster_table, nearest_centroid, current_point[0])) {
                changes_made++;
            }
            new_cluster_table[nearest_centroid].push_back(current_point[0]);
        }
        previous_cluster_table= new_cluster_table;

        change_rate= float(changes_made)/float(num_of_points);
        cout << "Change rate= " << change_rate << endl;
    }
    //STOP COUNTING TIME
    auto stop_time = std::chrono::high_resolution_clock::now();
    //PRINT THE RESULTS
    output_file << "Algorithm: Lloyds" << endl;
    for (i= 0 ; i < number_of_clusters ; i++) {
        output_file << "CLUSTER-" << i+1 << " {size: " << previous_cluster_table[i].size();
        output_file << " centroid: ";
        current_point= point_vector_get_point(get_centroids_id(i));
        for (j=1 ; j < dimensions ; j++) {
            output_file <<  current_point [j]<< " ";
        }
        output_file << "]" << endl;
    }
    auto time_passed = std::chrono::duration_cast<std::chrono::seconds>(stop_time - start_time);
    output_file << "clustering_time: " << time_passed.count() << " seconds" << endl;
    output_file << "Silhouette: ";
    print_silhouette(previous_cluster_table, output_file);
    if (complete_flag) {
        for(i=0 ; i < number_of_clusters ; i++) {
            output_file << "CLUSTER-" << i+1 << " {size: " << previous_cluster_table[i].size();
            output_file << " centroid: ";
            for (j=1 ; j < dimensions ; j++) {
                output_file << previous_cluster_table[i][j] << " ";
            }
            output_file << ", ";
            for (j= 0; j < previous_cluster_table[i].size(); j++) {
                output_file << previous_cluster_table[i][j] << " ";
            }
            output_file << "}" << endl;
        }
    }
}*/
