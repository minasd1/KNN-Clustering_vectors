// To compile: g++ main.cpp cmd_line_args.cpp vector_ops.cpp hashTable.cpp file_functions.cpp -o lsh
//To run: ./lsh -i input_small_id -k 3 -L 9 -o output123 -N 3 (To input_small_id na einai sto idio directory)

#include <iostream>
#include <cstring>
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
#include "user_input.h"


using namespace std;


int k ;
int L ;
int N ;
float R ;
int probes;

int main(int argc, char* argv[]){
    fstream input_file;             //FILE WE READ INPUT FROM
    fstream query_file;             //FILE WE READ QUERIES FROM
    fstream output_file;            //FILE TO WRITE OUTPUT TO
    int point_id;
    string line;
    string input_file_name, query_file_name, output_file_name;
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
    //string query_file_name(argv[2]);
    int continue_execution = 1;
    //auto start_time, stop_time, time_lsh, time_brute;
    int i;
    vector<dist_id_pair> points_lsh, points_brute;
    int probes;

    int error= read_cmd_args_lsh(argc, argv, input_file_name, query_file_name,
                       k,  L, output_file_name, N, R, M, probes);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);

    open_file(&input_file, input_file_name, fstream::in);

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

    //query_file = argv[2];

    //vector<int> query_point;
    while(continue_execution == 1){

        //cout << "Query file name is " << query_file_name << endl;


        //OPEN FILE TO READ QUERY FILES FROM
        open_file(&query_file, query_file_name, fstream::in);   //argv[2] instead of query_file_name
        //query_file.open(query_file_name);
        //OPEN FILE TO WRITE RESULTS TO
        open_file(&output_file, output_file_name, fstream::out);

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

            //cout << "argv[0] is" << argv[0] << endl;

            if(strcmp(argv[0], "./lsh") == 0){
                g.hash(query_point, hash_vector, 1);

                //LSH NEAREST NEIGHBORS
                //FIND TIME LSH - APPROXIMATE NEIGHBORS
                auto start_time = std::chrono::high_resolution_clock::now();
                points_lsh= find_approximate_knn(query_point, N, g);
                auto stop_time = std::chrono::high_resolution_clock::now();
                auto time_lsh = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
                //FIND TIME BRUTE FORCE - EXACT NEIGHBORS
                auto start_time2 = std::chrono::high_resolution_clock::now();
                points_brute= find_exact_knn(query_point, N, number_of_points);
                auto stop_time2 = std::chrono::high_resolution_clock::now();
                auto time_brute = std::chrono::duration_cast<std::chrono::microseconds>(stop_time2 - start_time2);
                //PRINTING IN OUTPUT FILE
                output_file << "Query: " << query_point[0] << endl;
                for (i= 1; i <= N ; i++) {
                    output_file << "Nearest neighbor-" << i<< ": " << points_lsh[i-1].id << endl;
                    output_file << "distanceLSH: " << points_lsh[i-1].dist << endl;
                    output_file << "distanceTrue: " << points_brute[i-1].dist << endl;
                }
                output_file <<  "tLSH: " << time_lsh.count() << endl;
                output_file << "tTrue: " << time_brute.count() << endl;

                //LSH RANGE SEARCH
                vector<int> points_in_range = range_search(hash_vector, 1000, query_point);

                output_file << "R-near neighbors:" << endl;

                //PRINT IDS OF POINTS IN RANGE
                for(int i = 0; i < points_in_range.size(); i++){

                    output_file << points_in_range[i] << endl;
                }

            }
            else if(strcmp(argv[0], "./cube") == 0){
                //DO STUFF
            }


            hash_vector.clear();


        }


        read_user_input(query_file_name, &continue_execution);
        close_file(&output_file);
        close_file(&query_file);

    }

    close_file(&input_file);

    return 0;

}
