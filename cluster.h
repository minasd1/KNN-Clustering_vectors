#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <vector>

#include "hash_functions.h"
#include "lsh.h"
#include "cube.h"
#include "vector_ops.h"
#include "hashTable.h"

using namespace std;


void k_means_plus_plus(int k);
void update(vector<vector<int>>& cluster_table, vector<int>& centroid_ids, int& last_known_id);
//void lloyds(int number_of_clusters, int num_of_points, fstream& output_file, bool complete_flag);
void reverse_assignment_lsh(G_Lsh g, int k);


#endif
