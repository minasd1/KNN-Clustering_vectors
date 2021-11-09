#ifndef VECTOR_OPS_NEW_H
#define VECTOR_OPS_NEW_H

#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#include <cmath>
#include <chrono>
#include <limits>

#include "hash_functions.h"
#include "lsh.h"

using namespace std;

static int last_known_id= 0; //ASSUMING THAT 1ST INPUT POINT'S IS 1 AND THE IDS ARE BEING INCREASED BY 1
//A VECTOR OF THE POINTS READ FROM INPUT FILE
static vector<vector <int> > point_vector;

//EVERY POINT HAS A COLLECTION OF L ID VALUES
static vector<vector<int>> points_ID_vector;

//VECTOR OF CENTROID POINTS - USED IN CLUSTERING
static vector<int> centroids;

/*-------------------------POINT VECTOR FUNCTIONS------------------------*/
void point_vector_insert_point(vector<int>& point);
vector<int> point_vector_get_point(int point_id);


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
void points_ID_push_back(int index_value, vector<int>& point_id_values);
int get_point_id_value(int index_value, int k);

/*-----------------------CENTROID FUNCTIONS-------------------------------*/
void centroids_insert_point(int id);
int centroids_get_size();
float centroids_calculate_min_distance_point(vector<int>& point);
void centroids_calculate_min_distance_input(vector<float>& points_min_distances);
void centroids_pick_first_centroid();
void centroids_pick_next_centroid(vector<float>& partial_sums);
void centroids_print_data();

/*--------------------OPERATIONS BETWEEN VECTORS FUNCTIONS-----------------*/
int calculate_dot_product(const vector <int>& point, vector <int>& v);
float calculate_distance(vector<int>& point1, const vector<int>& point2, int k=2);
vector<int> add_vectors(vector<int>& point1, vector<int>& point2);
vector<int> find_mean_vector(vector<int> vector_of_sums, int num_of_vectors);

/*-------------------------OTHER FUNCTIONS---------------------------------*/
void create_vector_int(vector<int>& ints, int k, int w, std::default_random_engine& generator);
float calculate_partial_sums(vector<float>& min_distances, vector<float>& partial_sums);



#endif
