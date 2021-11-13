#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <vector>
#include <utility>

#include "hash_functions.h"
#include "lsh.h"
#include "cube.h"
#include "vector_ops.h"
#include "hashTable.h"

using namespace std;


void k_means_plus_plus(int k);
void lloyds(int number_of_clusters, fstream& output_file, bool complete_flag);
void reverse_assignment_lloyds(vector<vector<int>>& cluster_table, int number_of_clusters, int last_id);
void reverse_assignment_lsh(G_Lsh g, int k);
void reverse_assignment_cube(G_Lsh g, int k, int probes);
void update(vector<vector<int>>& cluster_table, int& last_known_id);


#endif
