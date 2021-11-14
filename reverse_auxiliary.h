#ifndef REVERSE_AUXILIARY_H
#define REVERSE_AUXILIARY_H

#include <iostream>
#include <vector>

#include "vector_ops.h"
#include "hypercube.h"
#include "hash_functions.h"
#include "knn_table_functions.h"


using namespace std;

vector<int> reverse_cube_range_search(int g, int radius, int probes, int dimensions, vector<int>& query_point);






#endif