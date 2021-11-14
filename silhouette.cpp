#include <fstream>
#include <iostream>
#include <vector>
#include "vector_ops.h"
#include "lloyds_auxiliary.h"

using namespace std;

//PRINTS THE SILHOUETTE'S DATA
void print_silhouette(vector<vector<int>> cluster_table, fstream& output_file)
{
    int i, j, k, second_nearest;
    double sum_a, sum_b, a, b, silhouette, cluster_mean_silhouette, dataset_mean_silhouette, cluster_sum, dataset_sum;
    vector<int> current_point, other_point;

    dataset_sum= 0.0;
    output_file << "[ ";

    for (i= 0; i < cluster_table.size() ; i++) { //FOR EVERY CLUSTER
        cluster_sum= 0.0;
        for (j= 0; j < cluster_table[i].size() ; j++) { //FOR EVERY POINT IN THIS CLUSTER
            sum_a= 0.0;
            current_point= point_vector_get_point((cluster_table[i][j])-1);
            for (k= 0; k < cluster_table[i].size(); k++) { //FOR EVERY POINT IN THE SAME CLUSTER
                other_point= point_vector_get_point((cluster_table[i][k])-1);
                sum_a+= calculate_distance(current_point, other_point);
            }
            a= sum_a/double(cluster_table[i].size()-1);
            //cout << "Sum_a = " << sum_a << " and a= "<< a<<  endl;
            second_nearest= find_second_nearest_centroid(current_point);
            //cout << "second_nearest= " << second_nearest << endl;
            sum_b= 0.0;
            for (k= 0; k < cluster_table[second_nearest].size(); k++) { //FOR EVERY POINT IN THE SECOND NEAREST CLUSTER
                other_point= point_vector_get_point((cluster_table[second_nearest][k])-1);
                sum_b+= calculate_distance(current_point, other_point);
            }
            b= sum_b/double(cluster_table[second_nearest].size());
            //cout << "Sum_b= " << sum_b << " and b= " << b << endl;
            silhouette= (b-a)/max_double(a,b);
            cluster_sum+= silhouette;
        }
        cluster_mean_silhouette= cluster_sum/double(cluster_table[i].size());
        output_file << cluster_mean_silhouette << ", ";
        dataset_sum+= cluster_mean_silhouette;
    }
    dataset_mean_silhouette= dataset_sum/double(cluster_table.size());
    output_file << dataset_mean_silhouette << "]" << endl;
}
