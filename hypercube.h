#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include <iostream>
#include <vector>

using namespace std;

static vector<vector<int>> HyperCube;

void hyperCube_initialization(int num_of_buckets);
void hyperCube_push_back(int g, int key_val);
vector<int> hyperCube_get_points_in_bucket(int g);
int hyperCube_get_bucket_size(int bucket);
int hyperCube_get_point(int bucket, int place);
void hyperCube_print_data();





#endif