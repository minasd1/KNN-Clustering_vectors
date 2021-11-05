#include "hypercube.h"

//INITIALIZE HYPERCUBE
void hyperCube_initialization(int num_of_buckets){
    //HYPERCUBE INITIALLY HAS 2^D BUCKETS OF ZERO ITEMS EACH
    HyperCube.resize(num_of_buckets, vector<int>(0));
}

//PUT A POINT'S ID VALUE TO A BUCKET OF HYPERCUBE
//BUCKET IS INDICATED BY THE INT VALUE PRODUCED BY FUNCTION G
void hyperCube_push_back(unsigned int g, int key_val){
    HyperCube[g].push_back(key_val);
}

//GET KEY VALUES OF ALL THE POINTS OF A BUCKET OF HYPERCUBE
//BUCKET IS INDICATED BY THE INT VALUE PRODUCED BY FUNCTION G
vector<int> hyperCube_get_points_in_bucket(unsigned int g){

    vector<int> points_in_bucket;

    for(int i = 0; i < HyperCube[g].size(); i++){
        //PUSH THE ID OF THE POINT TO POINTS IN HYPERCUBE BUCKET
        points_in_bucket.push_back(HyperCube[g][i]);
    }

    return points_in_bucket;
}

//GET NUMBER OF ELEMENTS IN A SPECIFIC BUCKET OF THE HYPERCUBE
int hyperCube_get_bucket_size(int bucket){

    return HyperCube[bucket].size();
}

//GET A SPECIFIC POINT FROM A HYPERCUBE BUCKET
int hyperCube_get_point(int bucket, int place){

    return HyperCube[bucket][place];
}

//PRINT HYPERCUBE'S DATA - ONLY FOR CHECKING PURPOSES
void hyperCube_print_data(){

    for(int i = 0; i < HyperCube.size(); i++){
        for(int j = 0; j < HyperCube[i].size(); j++){
            cout << HyperCube[i][j];
        }
    }
}
