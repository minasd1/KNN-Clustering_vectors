#ifndef VECTOR_OPS_NEW_H
#define VECTOR_OPS_NEW_H

#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#include <cmath>

using namespace std;

//EVERY POINT HAS A COLLECTION OF L ID VALUES
static vector<vector<int>> points_ID_vector;

/*-------------------------V_VECTOR FUNCTIONS----------------------------*/
void v_vectors_initialization(vector<vector<int>>& v_vectors, int num_of_v_vectors, int dimensions);
void v_vectors_assign_coordinances(vector<vector<int>>& v_vectors, int num_of_vectors, int dimensions,
                                   std::default_random_engine& generator);
void v_vectors_printdata(vector<vector<int>>& v_vectors);

/*------------------------T-VECTOR FUNCTIONS------------------------------*/
void create_vector_t(vector<float>& t, int k, int w, std::default_random_engine& generator);
void print_vector_t(vector<float>& t);

/*--------------------POINTS ID VECTOR FUNCTIONS--------------------------*/
void initialize_points_ID_vector(int num_of_points, int L);
void points_ID_push_back(int index_value, vector<int> point_id_values);
int get_point_id_value(int index_value, int k);

/*--------------------OPERATIONS BETWEEN VECTORS FUNCTIONS-----------------*/
int calculate_dot_product(vector <int>& point, vector <int>& v);
float calculate_distance(vector<int>& point1, vector<int>& point2, int k);


#endif
