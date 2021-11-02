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
int iii = 0;

int main(int argc, char* argv[]){
    fstream input_file;             //FILE WE READ INPUT FROM
    fstream query_file;             //FILE WE READ QUERIES FROM
    fstream output_file;            //FILE TO WRITE OUTPUT TO
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

    //vector<int> query_point;
    //OPEN FILE TO READ QUERY FILES FROM
    open_file(&query_file, argv[2], fstream::in);

    //OPEN FILE TO WRITE RESULTS TO
    open_file(&output_file, argv[3], fstream::out);

    finish = 0;

    while(getline(query_file, line)){                       //READ QUERY FILE LINE BY LINE

        vector<int> query_point;
        start = 0;
        while(start < line.size()){                         //TOKENIZE EVERY LINE IN IT'S SEPERATED STRINGS
            finish = line.find_first_of(' ', start);

            if(finish == string::npos){

                finish = line.size();
            }

            if(start < line.size() - 1){
                token = line.substr(start, finish - start);
                query_point.push_back(stoi(token));               //CONVERT THE STRINGS TO INTEGERS AND PASS THEM TO QUERY POINT VECTOR
            }

            start = finish + 1;

        }

        g.hash(query_point, hash_vector, 1);

        //APOSTOLOS----------
        vector<dist_id_pair> points_nn;


        points_nn= find_approximate_knn(query_point, 1, g);
        cout << "BEST NEIGHBOR FOR QUERY  " << iii << "is point with ID " << points_nn[0].id << " with distance: " << points_nn[0].dist << endl;
        points_nn= find_approximate_knn(query_point, 3, g);
        cout << "BEST 3 NEIGHBORS FOR QUERY  " <<iii << "is (1st) point with ID " << points_nn[0].id << " with distance: " << points_nn[0].dist << endl;
        cout <<  "(2nd) point with ID " << points_nn[1].id << " with distance: " << points_nn[1].dist << endl;
        cout <<  "(3rd) point with ID " << points_nn[2].id << " with distance: " << points_nn[2].dist << endl;
        iii++;

        //-------------------


        vector<int> points_in_range = range_search(hash_vector, 1000, query_point);

        output_file << "Query: " << query_point[0] << endl;
        output_file << "R-near neighbors:" << endl;

        //PRINT IDS OF POINTS IN RANGE
        for(int i = 0; i < points_in_range.size(); i++){

            output_file << points_in_range[i] << endl;
        }

        hash_vector.clear();
    }


    close_file(&output_file);
    close_file(&query_file);
    close_file(&input_file);

    return 0;

}
