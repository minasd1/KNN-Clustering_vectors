// To compile: g++ main.cpp cmd_line_args.cpp vector_ops.cpp hashTable.cpp file_functions.cpp -o lsh
//To run: ./lsh -i input_small_id -k 3 -L 9 -o output123 -N 3 (To input_small_id na einai sto idio directory)

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <chrono>

#include "hashTable.h"
#include "hash_functions.h"
#include "vector_ops.h"
#include "file_functions.h"
#include "mod.h"
#include "cmd_line_args.h"
#include "lsh.h"


using namespace std;

const int k = 4;
const int L = 5;
const int N = 1;
const int R = 10000;

int main(int argc, char* argv[]){
    fstream input_file;             //FILE WE READ INPUT FROM
    int point_id;
    string line;
    int start;
    int finish = 0;
    bool first_iteration = true;
    string token;
    int dimensions = -1;            //DIMENSIONS OF SPACE AS GIVEN BY THE INPUT FILE
    unsigned int number_of_points = 0;
    int buckets;
    int points_divider = 16;         //USED TO GET TOTAL POINTS IN EACH HASH TABLE
    vector<int> p1, p2;             //TWO POINTS ON THE PLANE
    int M = pow(2, 31) - 5;
    vector<int> id_vector;          //IDS OF A POINT IN EACH HASHTABLE
    vector<int> hash_vector;        //INDEX OF EVERY HASHTABLE BUCKET THAT A GIVEN POINT WILL BE INSERTED

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);

    open_file(&input_file, argv[1], fstream::in);

    while(getline(input_file, line)){                       //READ FILE LINE BY LINE
        vector<int> point;
        start = 0;
        while(start < line.size()){                         //TOKENIZE EVERY LINE IN IT'S SEPERATED STRINGS
            finish = line.find_first_of(' ', start);

            if(finish == string::npos){

                finish = line.size();
            }

            if(start < line.size() - 1){
                token = line.substr(start, finish - start);
                point.push_back(stoi(token));               //CONVERT THE STRINGS TO INTEGERS AND PASS THEM TO POINT VECTOR
            }

            start = finish + 1;
            if(first_iteration == true){
                dimensions++;                               //KEEP TRACK OF THE DIMENSIONS OF GIVEN INSTANCE
            }
        }
    
        point_vector_insert_point(point);
        number_of_points++;
        first_iteration = false;
    }

    initialize_points_ID_vector(number_of_points, L);
    

    buckets = number_of_points/points_divider;

    //INITIALIZE L HASHTABLES WITH HASHTABLESIZE BUCKETS AND ZERO POINTS IN EACH BUCKET
    hashTable_initialization(L, buckets);

    //INITIALIZE G FUNCTION THAT LEADS US TO HASHTABLE BUCKETS
    G g(k, dimensions, generator, 6, M, buckets, L);

    //INSERT ALL THE POINTS FROM INPUT FILE TO HASHTABLES
    for(int i = 0; i < number_of_points; i++){

        g.hash(point_vector_get_point(i), hash_vector, 0);
    }

    hash_vector.clear();

    vector<int> query_point;
    for(int i = 0; i < dimensions; i++){

        query_point.push_back(i);
    }
    g.hash(query_point, hash_vector, 1);

    vector<int> points_in_range = range_search(hash_vector, 1000, query_point);

    cout << "Points in range:" << endl;
    cout << "Number of points in range is: " << points_in_range.size() << endl;

    //PRINT IDS OF POINTS IN RANGE
    //for(int i = 0; i < points_in_range.size(); i++){

        //cout << points_in_range[i] << endl;
    //}


    close_file(&input_file);

    return 0;

}
