#include <iostream>
#include <random>
#include <cmath>
#include "hash_functions.h"
#include "vector_ops.h"
#include "mod.h"
#include "hashTable.h"
#include "hypercube.h"

//CONSTRUCTOR OF ALL THE G FUNCTIONS
//GENERATES THE RANDOM V_VECTOR AND t FLOATS USED IN H FUNCTION
//GENERATES THE r INTEGERS
//ALL THESE 3 RANDOMLY GENERATED ELEMENTS COMBINED PRODUCES THE G HASH FUNCTIONS
G_Lsh::G_Lsh(int k_num, int dim, engine gen, int win, int m_mod, int tab_s, int l_num)
    : k(k_num), h_num(4*k_num), dimension(dim), generator(gen), w(win), m(m_mod),
      table_size(tab_s), l(l_num)
{
    int i, j, limit, rand_number;
    limit= pow(2,30); //AN INTEGER <= 32 BITS
    //THE k H FUNCTIONS AND THE k r INTEGERS WILL BE CHOSEN BY THE UNIFORM DISTRIBUTION
    uniform_real_distribution<float> h_distribution(0.0,(float)h_num); // FOR H FUNCTIONS
    uniform_real_distribution<float> r_distribution(0.0,(float)limit); // FOR r VALUES

    //INITIALIZE THE V_VECTORS AND t FLOATS
    v_vectors_assign_coordinances(v_vectors, h_num, dimension, generator);
    create_vector_t(t, h_num, w, generator);
    v_vectors_initialization(h_functions, l, k);
    v_vectors_initialization(r, l, k);

    for (i=0; i < l ; i++){  //FOR EVERY G FUNCTION
        for (j=0; j < k ; j++) { //CHOOSE RANDOMLY k OUT OF 4*k H FUNCTIONS AND k r INTEGERS TO BE USED
            rand_number= h_distribution(generator)/1;
            h_functions[i][j]= rand_number;
            rand_number= r_distribution(generator)/1;
            r[i][j]= rand_number;
        }
    }
}

//RETURNS A VECTOR THAT CONTAINS THE ID OF A GIVEN POINT FOR EVERY HASH FUNCTION
void G_Lsh::id(const vector<int>& point, vector<int>& id_vector, bool is_query)
{
    vector<int> ids;
    int i, j, sum, h, id;

    for (i=0; i < l ; i++) {  //FOR EVERY G FUNCTION
        sum= 0;
        for (j=0 ; j < k ; j++) { //ADD k r*h(p) PRODUCTS
            h= (calculate_dot_product(point, v_vectors[h_functions[i][j]]) + t[h_functions[i][j]])/w;
            sum+= r[i][j]*h;
        }
        id= mod(sum, m);
        //cout << "MOD (" << sum << " " << m << ")= " << id << endl; //CHECKING PURPOSE, TO BE REMOVED
        ids.push_back(id);
    }

    if(!is_query){

        points_ID_push_back(point[0], ids);
        //return;
    }
    else{

        id_vector = ids;

    }

}

//RETURNS A VECTOR THAT CONTAINS THE HASH VALUE OF A GIVEN POINT FOR EVERY HASH FUNCTION
void G_Lsh::hash(const vector<int>& point, vector<int>& hash_vector, bool is_query)
{
    int i;
    vector<int> hash_values;
    vector<int> ids;

    this->id(point, ids, true);
    for (i=0; i< ids.size() ; i++) {
        hash_values.push_back(mod(ids[i], table_size));
    }
    if (!is_query){
        hashTable_push_back(hash_values, point[0]);
        return;
    }
    hash_vector = hash_values;
}

//PRINTS THE V_VECTORS, t, r (THE DATA OF EACH G HASH FUNCTION)
//FOR DEBUGGING PURPOSES
void G_Lsh::print_hash_functions_data(void)
{
    int i, j;

    cout << "Total number of g hash functions: " << l << endl;
    cout << "Each g hash function uses " << k << " h functions" << endl;
    cout << "    v                                  t" << endl;
    cout << "h";
    for (i=0 ; i < h_num ; i++) {
        for (j=0; j < dimension ; j++ ){
            cout << v_vectors[i][j] << " " ;
        }
        cout << "t= " << t[i] << endl;
    }
}

//CONSTRUCTOR OF THE HYPERCUBE'S HASH FUNCTION
//GENERATES d' RANDOM h HASH FUNCTIONS
//GENERATES d' RANDOM f FUNCTIONS
//(ACTUALLY ONE f FUNCTION THAT RECEIVE d' DIFFERENT VALUES IN ONE OF ITS ARGUMENTS)
G_Hypercube::G_Hypercube(int dim, engine gen, int win, int str_dim)
    : dimension(dim), generator(gen), w(win), string_dimensions(str_dim)
{
    int limit, rand_number;

    limit= pow(2, 30); //A BIG INTEGER, THE MAX NUMBER THE UNIFORM DISTRIBUTION CAN PRODUCE (OR DI'S SECRETARY PHONE NUMBER)

    //INITIALIZE THE V_VECTORS AND t FLOATS (USED IN h FUNCTIONS)
    v_vectors_assign_coordinances(v_vectors, string_dimensions, dimension, generator);
    create_vector_t(t, string_dimensions, w, generator);
    //INITIALIZE d' RANDOM INTEGERS THAT WILL BE PASSED IN f FUNCTION
    create_vector_int(rand_ints, string_dimensions, limit, generator);
}

//MAPS A GIVEN NUMBER (h(p)) IN {0,1} UNIFORMLY
int f(int& h, int& random_int_2)
{
    int seed, random_int_1, xor_int, result;

    //SET THE UNIFORM DISTRIBUTION PARAMETERS
    seed = h;
    default_random_engine generator(seed);
    uniform_int_distribution<> distribution(0, 2107275161);

    random_int_1= distribution(generator);
    xor_int= random_int_1 ^ random_int_2;
    result= mod(xor_int, 2);
    return result;
}

//RECEIVES AND MODIFIES ARGUMENT hash_value SO THAT IT SHOWS IN WHICH VERTEX
//OF THE HYPERCUBE THE GIVEN POINT CORRESPONDS TO
//IF is_query IS false THE RECEIVED POINT point IS STORED IN THE HYPERCUBE'S HASH TABLE
void G_Hypercube::hash(const vector<int>& point, unsigned int& hash_value, bool is_query)
{
    int i, h, result;

    hash_value= 0; // 0000...000
    for (i=0; i < string_dimensions ; i++){
        h= (calculate_dot_product(point, v_vectors[i]) + t[i])/w;
        result= f(h, rand_ints[i]); //0 or 1
        hash_value <<= 1;
        hash_value+= result; // Last bit gets the value of result
    }
    if (!is_query) {
        hyperCube_push_back(hash_value, point[0]);
    }
}
