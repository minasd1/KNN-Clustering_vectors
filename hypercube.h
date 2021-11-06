#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include <iostream>
#include <vector>
#include <cmath>

#include "hamming_distance.h"

using namespace std;

static vector<vector<int>> HyperCube;

void hyperCube_initialization(int num_of_buckets);
void hyperCube_push_back(unsigned int g, int key_val);
int hyperCube_get_size();
vector<int> hyperCube_get_points_in_bucket(unsigned int g);
int hyperCube_get_bucket_size(int bucket);
int hyperCube_get_point(int bucket, int place);
vector<unsigned int> get_relative_buckets(unsigned int current_vertex, int probes, int string_dimensions);
void hyperCube_print_data();





#endif
