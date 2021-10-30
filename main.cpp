// To compile: g++ main.cpp cmd_line_args.cpp vector_ops.cpp hashTable.cpp file_functions.cpp -o lsh
//To run: ./lsh -i input_small_id -k 3 -L 9 -o output123 -N 3 (To input_small_id na einai sto idio directory)

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

//#include "hashTable.h"
//#include "vector_ops.h"
//#include "file_functions.h"
//#include "cmd_line_args.h"
#include "hash_functions.h"
#include "hashTable.h"
#include "vector_ops_new.h"

using namespace std;

const int k = 4;
const int L = 10;
const int N = 1;
const int R = 10000;
const int M = 1111123337;
int main(int argc, char* argv[]){

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);

    initialize_points_ID_vector(100, L);
    hashTable_initialization(L, 4);

    G g(k, 9, generator, 6, M, 4, L);

    vector<int> point;
    vector<int> v;
    vector<int> ids;

    point.push_back(0);
    point.push_back(3);
    point.push_back(7);
    point.push_back(4);
    point.push_back(11);
    point.push_back(13);
    point.push_back(32);
    point.push_back(42);
    point.push_back(8);
    point.push_back(16);

    cout << "3333\n" << endl;
    g.id(point, ids, false);
    cout << "4444\n" << endl;
    hashTable_print_data();

    g.id(point, ids, true);

    cout << "v= ";
    for (int i=0 ; i <ids.size() ; i++) {
        cout << ids[i] << " " ;
    }
    cout << endl;

    g.hash(point, ids, true);

    cout << "v= ";
    for (int i=0 ; i <ids.size() ; i++) {
        cout << ids[i] << " " ;
    }
    cout << endl;

    //g.print_hash_functions_data();

    return 0;

}
