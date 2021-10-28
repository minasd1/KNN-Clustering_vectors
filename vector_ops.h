#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H


#include <iostream>
#include <vector>
#include <time.h>

using namespace std;


//COLLECTION OF K D-VECTORS USED IN H FUNCTIONS TO BE MULTIPLIED WITH A SPECIFIC POINT
static vector<vector<int>> v_vectors;

void v_vectors_initialization(int num_of_v_vectors, int dimensions);
void v_vectors_assign_coordinances(int num_of_vectors, int dimensions);
int calculate_dot_product(vector <int>& point, vector <int>& v);
void v_vectors_printdata();









#endif