#include "vector_ops.h"


//SET NUMBER AND SIZE OF V-VECTORS
void v_vectors_initialization(vector<vector<int>>& v_vectors, int num_of_v_vectors, int dimensions){
    v_vectors.resize(num_of_v_vectors, vector<int>(dimensions));
}

//ASSIGN COORDINANCES TO EACH VECTOR V
//THE COORDINATES COME FROM THE GAUSSIAN DISTRIBUTION ~N(0,1)
void v_vectors_assign_coordinances(vector<vector<int>>& v_vectors, int num_of_vectors, int dimensions,
                                   std::default_random_engine& generator)
{
    int rand_int;
    normal_distribution<double> distribution(0,1);
    //INITIALIZE THE VECTORS
    v_vectors_initialization(v_vectors, num_of_vectors, dimensions);

    //ASSING VALUES USING RANDOM - INSTEAD POISSON DISTRIBUTION SHOULD BE USED
    for (int i = 0; i < v_vectors.size(); i++) {
        for (int j = 0; j < v_vectors[i].size(); j++){
            //EVERY COORDINANCE OF EACH VECTOR IS ASSIGNED TO BE BETWEEN 0 AND 101
            rand_int= distribution(generator)/1;
            v_vectors[i][j] = rand_int;
        }
    }
}

//CREATE A VECTOR t WHICH CONTAINS k RANDOM FLOATS IN RANGE [0,w)
//THE RANDOM FLOATS COME FROM THE UNIFORM DISTRIBUTION ~Unif[0,w)
void create_vector_t(vector<float>& t, int k, int w, std::default_random_engine& generator)
{
    int i;
    float rand_float;
    uniform_real_distribution<float> distribution(0.0,(float)w);

    t.resize(k);
    for (i=0; i < t.size() ; i++) {
        rand_float= distribution(generator);
        t[i]= rand_float;
    }
}

//CALCULATE DOT PRODUCT OF TWO VECTORS
int calculate_dot_product(vector <int>& point, vector <int>& d_vector){
    int product = 0;
    for(int i = 1; i < point.size(); i++){
        product = product + point[i] * d_vector[i];
    }
    return product;
}

//PRINT CONTENTS OF V-VECTORS - USED FOR CHECKING PURPOSES
void v_vectors_printdata(vector<vector<int>>& v_vectors){
    for (int i = 0; i < v_vectors.size(); i++) {
        for (int j = 0; j < v_vectors[i].size(); j++){
            cout<< v_vectors[i][j] << " ";
        }
        cout << endl;
    }
}

//PRINT CONTENTS OF t-VECTORS - USED FOR CHECKING PURPOSES
void print_vector_t(vector<float>& t){
    for (int i = 0; i < t.size(); i++) {
        cout << t[i] << " ";
    }
    cout << endl;
}

//COMPUTES THE DISTANCE BETWEEN 2 VECTORS USING THE k-NORM
float dist(vector<int>p1, vector<int>p2, int k=2)
{
    float sum= 0.0;
    int i;

    for (i=0 ; i < p1.size() ; i++) {
        sum+= pow(abs(p1[i]-p2[i]), k);
    }
    sum= pow(sum, 1.0/(float)k);

    return sum;
}
