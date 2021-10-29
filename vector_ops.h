#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H

#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#include <cmath>

using namespace std;

//COLLECTION OF K D-VECTORS USED IN H FUNCTIONS TO BE MULTIPLIED WITH A SPECIFIC POINT
static vector<vector<int>> v_vectors;

//t VALUES OF H FUNCTION
static vector<float> t;               

void v_vectors_initialization(int num_of_v_vectors, int dimensions);
void v_vectors_assign_coordinances(int num_of_vectors, int dimensions,
                                   std::default_random_engine& generator);
void create_vector_t(int k, int w, std::default_random_engine& generator);
int calculate_dot_product(vector <int>& point, vector <int>& v);
void v_vectors_printdata();
void print_vector_t();
float calculate_distance(vector<int>& point1, vector<int>& point2, int k);









#endif
