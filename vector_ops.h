#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H


#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>

using namespace std;


//COLLECTION OF K D-VECTORS USED IN H FUNCTIONS TO BE MULTIPLIED WITH A SPECIFIC POINT
static vector<vector<int>> v_vectors;

void v_vectors_initialization(int num_of_v_vectors, int dimensions);
void v_vectors_assign_coordinances(int num_of_vectors, int dimensions);
int calculate_dot_product(vector <int>& point, vector <int>& v);
int calculate_distance(vector <int>& point1, vector <int>& point2, int metric);
void v_vectors_printdata();









#endif