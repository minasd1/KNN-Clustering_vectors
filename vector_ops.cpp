#include "vector_ops.h"

//INSERT A POINT TO POINT VECTOR
void point_vector_insert_point(vector<int>& point){

    point_vector.push_back(point);
}

//GET A POINT FROM POINT VECTOR
vector<int> point_vector_get_point(int point_id){

    return point_vector[point_id];
}


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
            rand_int = distribution(generator)/1;
            v_vectors[i][j] = rand_int;
        }
    }
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

//PRINT CONTENTS OF t-VECTORS - USED FOR CHECKING PURPOSES
void print_vector_t(vector<float>& t){
    for (int i = 0; i < t.size(); i++) {
        cout << t[i] << " ";
    }
    cout << endl;
}

//INITIALIZE THE POINTS ID VECTOR
void initialize_points_ID_vector(int num_of_points, int L){
    points_ID_vector.resize(num_of_points, vector<int>(L));
    
}

//INSERT POINT'S ID VALUES TO ID VECTOR
//INDEX VALUE IS THE ID VALUE OF THE POINT (KEY)
void points_ID_push_back(int index_value, vector<int>& point_id_values){
    
    for(int i = 0; i < points_ID_vector[index_value].size(); i++){
       
        //points_ID_vector[index_value].push_back(point_id_values[i]);
        points_ID_vector[index_value][i] = point_id_values[i];
    }
    
}

//GET ID VALUE OF A POINT CORRESPONDING TO A SPECIFIC HASH TABLE
//K IS THE HASHTABLE THAT CONCERNS US - (NUMBER OF G-FUNCTION USED IN THIS HASHTABLE ({0,1,...,L})
//INDEX_VALUE IS THE ID(KEY_VALUE) OF THE POINT
int get_point_id_value(int index_value, int k){

    int id_value;
    id_value = points_ID_vector[index_value][k];

    return id_value;
}

//CALCULATE DOT PRODUCT OF TWO VECTORS
int calculate_dot_product(const vector <int>& point, vector <int>& d_vector){
    int product = 0;
    for(int i = 1; i < point.size(); i++){
        product = product + point[i] * d_vector[i-1];
    }
    return product;
}

//COMPUTES THE DISTANCE BETWEEN 2 VECTORS USING THE k-NORM
float calculate_distance(vector<int>& point1, const vector<int>& point2, int k)
{
    float distance = 0.0;
    int sum = 0;

    for (int i=1 ; i < point1.size() ; i++) {
        sum+= pow(abs(point1[i]-point2[i]), k);
    }
    distance = pow(sum, 1.0/(float)k);

    return distance;
}
