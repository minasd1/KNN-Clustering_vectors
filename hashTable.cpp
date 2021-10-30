#include "hashTable.h"

void hashTable_initialization(int num_of_hashTables, int num_of_buckets){

    //INITIALIZE L HASHTABLES WITH HASHTABLESIZE BUCKETS AND 1 POINT (WITH ID 0) IN EACH BUCKET
    HashTables.resize(num_of_hashTables, vector<vector<int> >(num_of_buckets, vector<int>(0)));
    //IT CAN ALSO BE INITIALIZED AS ZERO
}

//PUSH A POINT'S ID VALUE TO A BUCKET OF EACH OF THE HASHTABLES USING 
//EACH BUCKET IS INDICATED BY THE INT VALUES PRODUCED BY FUNCTION G
void hashTable_push_back(vector<int>& g,int key_val){

    for(int i = 0; i < HashTables.size(); i++){
        HashTables[i][g[i]].push_back(key_val);
        //HashTables[i][g[i]].end() = key_val;
    }
}

//GET KEY VALUES OF ALL THE POINTS OF A BUCKET OF EACH HASH TABLE
//EACH BUCKET IS INDICATED BY THE INT VALUES PRODUCED BY FUNCTION G 
vector<int> hashTable_get_points_in_buckets(vector<int>& g){

    vector<int> points_in_buckets;

    for(int i = 0; i < HashTables.size(); i++){
        for(int j = 0; j < HashTables[i][g[i]].size(); j++){
            points_in_buckets.push_back(HashTables[i][g[i]][j]);
        }
    }

    return points_in_buckets;
}

void hashTable_print_data(){
    for (int i = 0; i < HashTables.size(); i++) {
        for (int j = 0; j < HashTables[i].size(); j++){
            for(int k = 0; k < HashTables[i][j].size(); k++){
                cout<< HashTables[i][j][k] << " ";
            }
        }
        cout << endl;
    }
}