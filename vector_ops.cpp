#include "vector_ops.h"

//SET NUMBER AND SIZE OF V-VECTORS
void v_vectors_initialization(int num_of_v_vectors, int dimensions){
    v_vectors.resize(num_of_v_vectors, vector<int>(dimensions));
}

//ASSIGN COORDINANCES TO EACH VECTOR V
//THIS FUNCTION SHOULD BE REPLACED BY USING GAUSSIAN DISTRIBUTION
void v_vectors_assign_coordinances(int num_of_vectors, int dimensions){
    //INITIALIZE THE VECTORS
    v_vectors_initialization(num_of_vectors, dimensions);

    //INITIALIZE RANDOM SEED
    srand(time(NULL));

    //ASSING VALUES USING RANDOM - INSTEAD POISSON DISTRIBUTION SHOULD BE USED
    for (int i = 0; i < v_vectors.size(); i++) {
        for (int j = 0; j < v_vectors[i].size(); j++){
            //EVERY COORDINANCE OF EACH VECTOR IS ASSIGNED TO BE BETWEEN 0 AND 101
            v_vectors[i][j] = rand()%101;
            
        }
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

//CALCULATE EUCLIDEAN DISTANCE BETWEEN TWO POINTS
int calculate_distance(vector <int>& point1, vector <int>& point2, int metric){
    int distance = 0;
    int sum = 0;

    for(int i = 1; i < point1.size(); i++){
        //METRIC = 2 FOR EUCLIDEAN DISTANCE
        sum = sum + pow(point1[i] - point2[i], metric);
    }

    distance = pow(sum, 1.0/metric);

    return distance;
}

//PRINT CONTENTS OF V-VECTORS - USED FOR CHECKING PURPOSES
void v_vectors_printdata(){
    for (int i = 0; i < v_vectors.size(); i++) {
        for (int j = 0; j < v_vectors[i].size(); j++){
            
            cout<< v_vectors[i][j] << " ";
        }
        cout << endl;
    }
}