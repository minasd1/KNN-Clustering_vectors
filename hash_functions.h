#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#include <cmath>

using namespace std;

typedef std::default_random_engine engine;

class G {
private:
    int k; // THE NUMBER OF H FUNCTIONS USED IN G FUNCTIONS
    int h_num; // THE TOTAL NUMBER OF H FUNCTIONS TO BE CREATED, MULTIPLE OF k
    int dimension; // THE NUMBER OF COORDINATES EACH V_VECTOR AND POINT IN THE DATASET HAS
    engine generator; // THE GENERATOR OF PSEUDO-RANDOM NUMBERS
    vector<vector<int>> v_vectors; // A STRUCT TO STORE THE RANDOMLY GENERATED V_VECTORS (USED IN H FUNCTION)
    int num_of_vectors; // THE SAME WITH h_num
    vector<float> t; // A STRUCT TO STORE THE RANDOM FLOATS t (USED IN H FUNCTION)
    int w; // THE WINDOW VARIABLE (USED IN H FUNCTION)
    int m; // THE FIRST MODULO IN G FUNCTIONS
    int table_size; // THE SECOND MODULO IN G FUNCTION
    int l; // THE NUMBER OF G FUNCTIONS (OR THE NUMBER OF HASH TABLES)
    vector<vector<int>> h_functions; //A STRUCT TO STORE WHICH H FUNCTIONS ARE USED IN EACH G FUNCTION
    vector<vector<int>> r; //A STRUCT TO STORE THE r RANDOM INTEGERS OF EACH G FUNCTION
public:
    G(int k_num, int dim, engine gen, int win, int m_mod, int tab_s, int l_num);
    void id(vector<int>& point, vector<int>& id_vector, bool is_query);
    void hash(vector<int>& point, vector<int>& hash_vector, bool is_query);
    void print_hash_functions_data(void);
};

#endif
