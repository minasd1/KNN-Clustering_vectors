#ifndef LSH_H
#define LSH_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "vector_ops.h"
#include "hash_functions.h"
#include "hashTable.h"
#include "knn_table_functions.h"

using namespace std;

vector<dist_id_pair> lsh_find_approximate_knn(vector<int> query_point, int k,  G_Lsh& g, int max_candidates=-1);
vector<dist_id_pair> find_exact_knn(vector<int> query_point, int k, int num_of_points);
vector<int> lsh_range_search(vector<int>& g, int radius, vector<int>& query_point);









#endif
