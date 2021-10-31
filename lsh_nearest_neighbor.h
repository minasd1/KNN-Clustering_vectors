#ifndef LSH_NEAREST_NEIGHBOR_H
#define LSH_NEAREST_NEIGHBOR_H

#include <vector>
#include "hash_functions.h"

using namespace std;

typedef struct dist_id_pair {
    float dist; //DISTANCE BETWEEN POINT AND QUERY POINT
    int id; //ID OF THE POINT
}dist_id_pair;

vector<dist_id_pair> find_approximate_knn(vector<int> query_point, int k,  G& g, int max_candidates=-1);

#endif
