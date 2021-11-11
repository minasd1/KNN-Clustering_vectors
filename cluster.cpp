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

//IMPLEMENTATION OF THE LLOYDS ALGORITHM
void lloyds(int number_of_clusters, int num_of_points, fstream& output_file, bool complete_flag)
{
    int i, dimensions, j;
    int nearest_centroid; //NEAREST CENTROID'S INDEX IN THE centroid TABLE
    int changes_made= 0; //HOW MANY POINTS CHANGED CLUSTER IN A NEW ASSIGNMENT
    vector<int> current_point;
    vector<vector<int>> new_cluster_table, previous_cluster_table;
    float change_rate; //THE RATE OF POINTS THAT CHANGED CLUSTER TO THE TOTAL NUMBER OF POINTS
    int last_known_id= num_of_points-1;
    int max_updates= 20;

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
    change_rate= float(changes_made)/float(num_of_points); //INITIALLY change_rate WILL BE 1 (100%)

    //LOOP UNTIL A SMALL PERCENTAGE OF POINTS CHANGE CLUSTER
    //OR THE MAXIMUM NUMBER OF ITERATIONS HAS BEEN REACHED
    while (change_rate > 0.1 && --max_updates > 0) {
        //UPDATE THE CENTROIDS
        update(previous_cluster_table, last_known_id);

        //PREPARE THE NEW CLUSTER TABLE FOR THE NEW CENTROIDS ASSIGNMENT
        changes_made= 0;
        new_cluster_table.clear();
        new_cluster_table.resize(number_of_clusters);

        //MAKE A NEW ASSIGNMENT FOR ALL THE POINTS
        for (i=0 ; i < num_of_points ; i++) { //FOR EVERY POINT
            current_point= point_vector_get_point(i);
            nearest_centroid= find_nearest_centroid(current_point);
            //IF A POINT IS BEING ASSIGNED IN A DIFFERENT CLUSTER THAN THE ONE IT WAS ASSIGNED IN THE PREVIOUS ASSIGNMENT
            if (!already_in_that_cluster(previous_cluster_table, nearest_centroid, current_point[0])) {
                changes_made++;
            }
            new_cluster_table[nearest_centroid].push_back(current_point[0]);
        }
        previous_cluster_table= new_cluster_table;
        change_rate= float(changes_made)/float(num_of_points);
 //       cout << "Change rate= " << change_rate << endl;
    }
    //WHEN THE CLUSTERS HAVE BEEN DEFINITIVELY FORMED STOP COUNTING TIME
    auto stop_time = std::chrono::high_resolution_clock::now();

    //PRINT THE RESULTS
    output_file << "Algorithm: Lloyds" << endl;
    for (i= 0 ; i < number_of_clusters ; i++) {
        output_file << "CLUSTER-" << i+1 << " {size: " << previous_cluster_table[i].size();
        output_file << " centroid: ";
        current_point= point_vector_get_point(get_centroids_id(i));
        if (previous_cluster_table[i].size() == 0){
            output_file << "- }" << endl;
            continue;
        }
        for (j=1 ; j < dimensions ; j++) {
            output_file <<  current_point [j]<< " ";
        }
        output_file << "}" << endl;
    }
    auto time_passed = std::chrono::duration_cast<std::chrono::seconds>(stop_time - start_time);
    output_file << "clustering_time: " << time_passed.count() << " seconds" << endl;
    output_file << "Silhouette: ";
    print_silhouette(previous_cluster_table, output_file);
    if (complete_flag) {
        for(i=0 ; i < number_of_clusters ; i++) {
            output_file << "CLUSTER-" << i+1 << " {size: " << previous_cluster_table[i].size();
            output_file << " centroid: [";
            current_point= point_vector_get_point(get_centroids_id(i));
            for (j=1 ; j < dimensions ; j++) {
                output_file << current_point[j] << " ";
            }
            output_file << "]";
            output_file << ", ";
            for (j= 0; j < previous_cluster_table[i].size(); j++) {
                output_file << previous_cluster_table[i][j] << " ";
            }
            output_file << "}" << endl;
        }
    }
}

void reverse_assignment_lsh(G_Lsh g, int k){

    k_means_plus_plus(k);                //INITIALIZE K CENTROIDS USING K-MEANS++ ALGORITHM
    vector<int> centroid;                //HERE CENTROIDS ARE THE QUERY POINTS
    vector<int> hash_vector;             //THE HASHES PRODUCED BY G THAT LEADS US TO LSH HASHTABLE BUCKETS
    vector<int> appending_points;
    vector<pair<vector<int>,int>> points_in_range; //ALL THE POINTS THAT ARE INSIDE THE GIVEN RADIUS FROM EVERY CENTROID
    int radius = centroids_get_radii();  //SET MINIMUM DISTANCE BETWEEN CENTROIDS DIVIDED BY 2 AS FIRST RADIUS
    int iterations = 10000;

    //is_assigned_initialize();           //INITIALIZE ALL INPUT POINTS AS UNASSIGNED

    //FOR EVERY CENTROID
    for(int i = 0; i < centroids_get_size(); i++){
        //GET CENTROID'S COORDINANCES BY ACCESSING THE POINT VECTOR DATA
        centroid = point_vector_get_point(centroids_get_centroid(i) - 1);
        //GET THE HASHTABLE BUCKETS WHICH THE CURRENT CENTROID SHOULD ACCESS IN EACH HASHTABLE
        g.hash(centroid, hash_vector, 1);
        //AND PERFORM RANGE SEARCH - 0 IS THE FIRST INDEX OF AN UNASSIGNED POINT AT START
        points_in_range.push_back(make_pair(lsh_range_search(hash_vector, radius, centroid), 0));
        //PARTITION POINTS IN RANGE OF CENTROID i TO ALREADY ASSIGNED AND NOT ASSIGNED
        partition_assigned_unassigned(points_in_range[i]);

        hash_vector.clear();    //WE MUST CLEAR THE HASH VECTOR AFTER EVERY ITERATION
    }

    centroids_duplicates_assign_to_nearest_centroid(points_in_range);

    for(int j = 0; j < iterations; j++){
        radius = radius*2;
        for(int i = 0; i < centroids_get_size(); i++){

            //GET CENTROID'S COORDINANCES BY ACCESSING THE POINT VECTOR DATA
            centroid = point_vector_get_point(centroids_get_centroid(i) - 1);
            //GET THE HASHTABLE BUCKETS WHICH THE CURRENT CENTROID SHOULD ACCESS IN EACH HASHTABLE
            g.hash(centroid, hash_vector, 1);
            //AND PERFORM RANGE SEARCH - 0 IS THE FIRST INDEX OF AN UNASSIGNED POINT AT START
            appending_points = lsh_range_search(hash_vector, radius, centroid);

            points_in_range[i].first.insert(points_in_range[i].first.end(), appending_points.begin(),
                                                                                    appending_points.end());
            //PARTITION POINTS IN RANGE OF CENTROID i TO ALREADY ASSIGNED AND NOT ASSIGNED
            partition_assigned_unassigned(points_in_range[i]);


            appending_points.clear();
            hash_vector.clear();    //WE MUST CLEAR THE HASH VECTOR AFTER EVERY ITERATION

        }

        centroids_duplicates_assign_to_nearest_centroid(points_in_range);
    }


}

//RECEIVES A TABLE OF THE CLUSTERS. EACH ROW CORRESPENDS TO A CLUSTER
//IN EACH ROW ARE STORED THE IDS OF THE INPUT POINTS THAT BELONG TO THAT CLUSTER
//ALSO RECEIVES THE DIMENSION OF THE POINT VECTORS (HOW MANY COORDINATES EACH POINT HAS)
//RETURNS A TABLE WITH EACH CLUSTER'S NEW CENTROID
void update(vector<vector<int>>& cluster_table, int& last_known_id)
{
    int row, column;  //ITERATORS
    int dimensions; //THE NUMBER OF COORDINATES AN INPUT POINT HAS
    vector<int> coordinates_sum;  //A TABLE OF THE SUMS OF THE 1ST, 2ND, ..., NTH COORDINATE OF THE POINTS IN THE SAME CLUSTER
    vector<int> current_point, mean_vector;
    vector<vector<int>> new_centroids;

    dimensions= point_vector_get_point(1).size();
    //CLEAR THE CENTROIDS VECTOR SO THE NEW CENTROIDS CAN BE PUSHED BACK IN IT
    //centroids_ids.clear();
    centroids_clear();

    for (row=0 ; row < cluster_table.size() ; row++) { //FOR EACH CLUSTER
        coordinates_sum.assign(dimensions, 0); //INITIALIZE ALL SUMS (ONE SUM FOR EACH COORDINATE) WITH 0
        for (column=0 ; column < cluster_table[row].size(); column++) { //FOR EVERY POINT IN THE CLUSTER
            current_point= point_vector_get_point((cluster_table[row][column])-1);
            coordinates_sum= add_vectors(current_point, coordinates_sum);
        }
        mean_vector= find_mean_vector(coordinates_sum, cluster_table[row].size());
        mean_vector[0]= ++last_known_id;
        point_vector_insert_point(mean_vector);
        centroids_insert_point(mean_vector[0]);
    }
}
