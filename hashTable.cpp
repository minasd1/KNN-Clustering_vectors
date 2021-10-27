#include "hashTable.h"

void hashTable_initialization(int num_of_hashTables, int num_of_buckets){

    //INITIALIZE L HASHTABLES WITH HASHTABLESIZE BUCKETS AND 1 POINT (WITH ID 0) IN EACH BUCKET
    HashTables.resize(num_of_hashTables, vector<vector<int> >(num_of_buckets, vector<int>(1)));
    //IT CAN ALSO BE INITIALIZED AS ZERO
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