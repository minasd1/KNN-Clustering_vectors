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
#include "hypercube.h"
#include "cube.h"
#include "cluster.h"


using namespace std;


int main(int argc, char* argv[]){
                                    /*COMMAND LINE ARGUMENTS */
    int k ;                         //NUMBER OF H FUNCTIONS USED IN FUNCTION G
    int L ;                         //NUMBER OF HASHTABLES(LSH)
    int N ;                         //NUMBER OF NEAREST MEIGHBORS
    float R ;                       //SEARCH RADIUS
    int probes ;                    //MAX NUMBER OF HYPERCUBE VERTICES TO BE CHECKED
    int k_cube ;                    //D'
    int M_cube ;                    //MAX NUMBER OF CANDIDATE POINTS TO BE CHECKED
    int window= 100;
    int k_cluster ;

    fstream input_file;             //FILE WE READ INPUT FROM
    fstream query_file;             //FILE WE READ QUERIES FROM
    fstream config_file;            //CLUSTER CONFIGURATION FILE
    fstream output_file;            //FILE TO WRITE OUTPUT TO
    int point_id;
    string line;
    string input_file_name, query_file_name, output_file_name, config_file_name, method;
    int start;
    int finish = 0;
    bool first_iteration = true;
    string token;
    int dimensions = -1;            //DIMENSIONS OF SPACE AS GIVEN BY THE INPUT FILE
    unsigned int number_of_points = 0;
    int buckets;
    int points_divider = 2;         //USED TO GET TOTAL POINTS IN EACH HASH TABLE
    vector<int> p1, p2;             //TWO POINTS ON THE PLANE
    int M = pow(2, 31) - 5;
    vector<int> id_vector;          //IDS OF A POINT IN EACH HASHTABLE
    vector<int> hash_vector;        //INDEX OF EVERY HASHTABLE BUCKET THAT A GIVEN POINT WILL BE INSERTED
    unsigned int hash_value;
    int continue_execution = 1;

    int i;
    vector<dist_id_pair> points_lsh, points_cube, points_brute;

    int error= read_cmd_args(argc, argv, input_file_name, query_file_name, k, k_cube, L,
                             output_file_name, N, R, M_cube, probes, config_file_name, method);
    if (error) {
        return -1;
    }

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

    //MARK ALL THE INPUT POINTS AS UNASSIGNED
    is_assigned_initialize();

    if(strcmp(argv[0], "./cluster") == 0){
        
        //OPEN CLUSTER CONFIGURATION FILE
        open_file(&config_file, config_file_name, fstream::in);
        
        finish = 0;
        int count = 0;

        while(getline(config_file, line)){
            
            start = 0;
            while(start < line.size()){
                
                start = line.find_first_of(' ', start);
                
                finish = line.size();
                
                    
                token = line.substr(start, finish - start);
                if(count == 0){
                    k_cluster = stoi(token);
                }
                else if(count == 1){
                    L = stoi(token);
                }
                else if(count == 2){
                    k = stoi(token);
                }
                else if(count == 3){
                    M_cube = stoi(token);
                }
                else if(count == 4){
                    k_cube = stoi(token);
                }
                else if(count == 5){
                    probes = stoi(token);
                }

                start = finish;
                
                count++;
            }

             
        }

        close_file(&config_file);
    }
    cout << "k_cluster: " << k_cluster << endl;
    cout << "L: " << L << endl;
    cout << "k: " << k << endl;
    cout << "M_cube: " << M_cube << endl;
    cout << "k_cube" << k_cube << endl;
    cout << "probes: " << probes << endl;

    initialize_points_ID_vector(number_of_points, L);

    buckets = number_of_points/points_divider;

    //INITIALIZE G FUNCTION THAT LEADS US TO HASHTABLE BUCKETS
    G_Lsh g_lsh(k, dimensions, generator, window, M, buckets, L);

    //INITIALIZE G FUNCTION THAT LEADS US TO HYPERCUBE BUCKETS
    G_Hypercube g_cube(dimensions, generator, window, k_cube);

    if(strcmp(argv[0], "./lsh") == 0 || strcmp(argv[0], "./cluster") == 0){
        cout << "HASHTABLE INITIALIZED" << endl;
        //INITIALIZE L HASHTABLES WITH HASHTABLESIZE BUCKETS AND ZERO POINTS IN EACH BUCKET
        hashTable_initialization(L, buckets);

        //INSERT ALL THE POINTS FROM INPUT FILE TO HASHTABLES
        for(int i = 0; i < number_of_points; i++){

            g_lsh.hash(point_vector_get_point(i), hash_vector, 0);
        }

        hash_vector.clear();
    }
    
    if(strcmp(argv[0], "./cube") == 0 || strcmp(argv[0], "./cluster") == 0){
        cout << "HYPERCUBE IS INITIALIZED" << endl;
        //INITIALIZE A HYPERCUBE WITH 2^D' BUCKETS AND ZERO POINTS IN EACH BUCKET
        hyperCube_initialization(pow(2, k_cube));

        //INSERT ALL THE POINTS FROM INPUT FILE TO HYPERCUBE BUCKETS
        for(int i = 0; i < number_of_points; i++){

            g_cube.hash(point_vector_get_point(i), hash_value, 0);
        }

    }

    while(continue_execution == 1){

        if(strcmp(argv[0], "./lsh") == 0 || strcmp(argv[0], "./cube") == 0){

            //OPEN FILE TO READ QUERY FILES FROM
            open_file(&query_file, query_file_name, fstream::in);

            //OPEN FILE TO WRITE RESULTS TO
            open_file(&output_file, output_file_name, fstream::out);

            finish = 0;

            //FOR EVERY QUERY POINT
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

                if(strcmp(argv[0], "./lsh") == 0){
                    g_lsh.hash(query_point, hash_vector, 1);

                    //LSH NEAREST NEIGHBORS
                    //FIND TIME LSH - APPROXIMATE NEIGHBORS
                    auto start_time = std::chrono::high_resolution_clock::now();
                    points_lsh= lsh_find_approximate_knn(query_point, N, g_lsh);
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
                        if (i > points_lsh.size()) {
                            output_file << "Nearest neighbor-" << i<< ": Not enough points in buckets (Consider to decrease hash table size or window)" << endl;
                            output_file << "distanceLSH: Not enough points in buckets (Consider to decrease hash table size or window)" << endl;
                            continue;
                        }
                        output_file << "Nearest neighbor-" << i<< ": " << points_lsh[i-1].id << endl;
                        output_file << "distanceLSH: " << points_lsh[i-1].dist << endl;
                        output_file << "distanceTrue: " << points_brute[i-1].dist << endl;
                    }
                    output_file <<  "tLSH: " << time_lsh.count() << " microseconds" << endl;
                    output_file << "tTrue: " << time_brute.count() << " microseconds" << endl;

                    //LSH RANGE SEARCH
                    vector<int> points_in_range = lsh_range_search(hash_vector, 1000, query_point);

                    output_file << "R-near neighbors:" << endl;

                    //PRINT IDS OF POINTS IN RANGE
                    for(int i = 0; i < points_in_range.size(); i++){

                        output_file << points_in_range[i] << endl;
                    }

                }
                else if(strcmp(argv[0], "./cube") == 0){
                    //DO STUFF
                    g_cube.hash(query_point, hash_value, 1);

                    output_file << "Query: " << query_point[0] << endl;

                    //--------------
                    //CUBE NEAREST NEIGHBORS
                    //FIND TIME CUBE - APPROXIMATE NEIGHBORS
                    auto start_time = std::chrono::high_resolution_clock::now();
                    points_cube= cube_find_approximate_knn(query_point, N, g_cube, probes, k_cube, M_cube);
                    auto stop_time = std::chrono::high_resolution_clock::now();
                    auto time_cube = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
                    //FIND TIME BRUTE FORCE - EXACT NEIGHBORS
                    auto start_time2 = std::chrono::high_resolution_clock::now();
                    points_brute= find_exact_knn(query_point, N, number_of_points);
                    auto stop_time2 = std::chrono::high_resolution_clock::now();
                    auto time_brute = std::chrono::duration_cast<std::chrono::microseconds>(stop_time2 - start_time2);
                    // //PRINTING IN OUTPUT FILE
                    for (i= 1; i <= N ; i++) {
                        //IF NO POINTS FOUND
                        if(points_cube[i-1].id == -1){
                            output_file << "No points found in query's bucket or it's relative buckets" << endl;
                        }
                        else{
                            output_file << "Nearest neighbor-" << i<< ": " << points_cube[i-1].id << endl;
                            output_file << "distanceCUBE: " << points_cube[i-1].dist << endl;
                        }

                        output_file << "distanceTrue: " << points_brute[i-1].dist << endl;

                    }
                    output_file <<  "tCUBE: " << time_cube.count() << " microseconds" << endl;
                    output_file << "tTrue: " << time_brute.count() << " microseconds" << endl;
                    //-----------------
                    //HYPERCUBE RANGE SEARCH
                    vector<int> points_in_range = cube_range_search(hash_value, 1000, probes, k_cube, query_point);
                    output_file << "R-near neighbors:" << endl;

                    //PRINT IDS OF POINTS IN RANGE
                    for(int i = 0; i < points_in_range.size(); i++){

                        output_file << points_in_range[i] << endl;
                    }
                }


                hash_vector.clear();


            }


            read_user_input(query_file_name, &continue_execution);
            close_file(&output_file);
            close_file(&query_file);
        }
        // else if(strcmp(argv[0], "./cube") == 0){

        //     //OPEN CLUSTER CONFIGURATION FILE
        //     open_file(&config_file, argv[3], fstream::in);

        //     finish = 0;

        //     while(getline(config_file, line)){

        //         start = 0;
        //         while(start < line.size()){
        //             finish = line.find_first_of(' ', start);
        //         }

        //         if(finish == string::npos){

        //             finish = line.size();
        //         }

        //         token = line.substr(start, finish - start);
        //         start = finish + 1;
        //         cout << "token is " << token << endl; 
        //     }
        // }
        else if(strcmp(argv[0], "./cluster") == 0){

            // //OPEN FILE TO WRITE RESULTS TO
            // open_file(&output_file, output_file_name, fstream::out);

            continue_execution = 0;
        }

        // read_user_input(query_file_name, &continue_execution);
        // close_file(&output_file);


    }

    cout << "k_cluster is " << k_cluster << endl;
    
    open_file(&output_file, output_file_name, fstream::out);
    //lloyds(20, output_file, false);
    //reverse_assignment_lsh(g_lsh, k_cluster);
    cout << "got in here" << endl;
    reverse_assignment_cube(g_cube, k_cluster, probes);
    cout << "but not here" << endl;
    close_file(&output_file);


    close_file(&input_file);

    return 0;

}
