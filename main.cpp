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

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);

    vector<vector <int> > point_vector;

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
        point_vector.push_back(point);
        number_of_points++;
        first_iteration = false;
    }

    initialize_points_ID_vector(number_of_points, L);
    

    buckets = number_of_points/points_divider;

    //INITIALIZE L HASHTABLES WITH HASHTABLESIZE BUCKETS AND ZERO POINTS IN EACH BUCKET
    hashTable_initialization(L, buckets);

    G g(k, dimensions, generator, 6, M, buckets, L);

    for(int i = 0; i < dimensions; i++){
        p1.push_back(i);
    }

    vector <int> id_vec;

    g.id(p1, id_vec, 0);
    cout << "Printing id vector of p1" << endl;

    for(int i = 0; i < id_vec.size(); i++){
        cout << id_vec[i] << endl;
    }
    

    //JUST FOR TESTING PURPOSES
    

    //INTERACT WITH VECTOR-OPS LIBRARY
//    v_vectors_assign_coordinances(k, dimensions);
//    v_vectors_printdata();

    g.hash(p1, id_vec, 0);

    cout << "v= ";
    for (int i=0 ; i <id_vec.size() ; i++) {
        cout << id_vec[i] << " " ;
    }
    cout << endl;

    hashTable_print_data();

    //FOR DEMONSTRATION PORPUSES:
    //MAKE TWO POINTS P1(1,1) AND P2(9,3)
    /*cout << "\n\n" << endl;
    p1.push_back(1);
    p1.push_back(1);
    p2.push_back(9);
    p2.push_back(3);*/

    //MAKE 10 V_VECTORS WITH 20 GAUSSIAN RANDOM COORDINATES
    //AND 10 t INTEGERS WITH UNIFORM DISTRIBUTION ~UNIF[0,6)
    //v_vectors_assign_coordinances(10, 20, generator);
    //v_vectors_printdata();
    //cout << endl;
    //create_vector_t(10, 6, generator);
    //print_vector_t();

    //PRINT THE EUCLEDIAN AND MANHATTAN DISTANCE OF P1, P2
    /*float dista;
    dista= calculate_distance(p1, p2, 2);
    cout << "EUCLEDIAN DISTANCE p1(" << p1[0] << "," << p1[1] << "), p2(" << p2[0] << "," << p2[1] << ")" << ": "
         << dista << endl;
    dista= calculate_distance(p1, p2, 1);
    cout << "MANHATTAN DISTANCE p1(" << p1[0] << "," << p1[1] << "), p2(" << p2[0] << "," << p2[1] << ")" << ": "
         << dista << endl;*/


    close_file(&input_file);

    return 0;

}
