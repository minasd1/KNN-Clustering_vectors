#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H

#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#include <cmath>

using namespace std;

//COLLECTION OF K D-VECTORS USED IN H FUNCTIONS TO BE MULTIPLIED WITH A SPECIFIC POINT

void v_vectors_initialization(vector<vector<int>>& v_vectors, int num_of_v_vectors, int dimensions);
void v_vectors_assign_coordinances(vector<vector<int>>& v_vectors, int num_of_vectors, int dimensions,
                                   std::default_random_engine& generator);
void create_vector_t(vector<float>& t, int k, int w, std::default_random_engine& generator);
int calculate_dot_product(vector <int>& point, vector <int>& v);
void v_vectors_printdata(vector<vector<int>>& v_vectors);
void print_vector_t(vector<float>& t);
float dist(vector<int>p1, vector<int>p2, int k);









#endif
