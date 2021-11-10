#include "vector_ops.h"

//INSERT A POINT TO POINT VECTOR
void point_vector_insert_point(vector<int>& point){

    point_vector.push_back(point);
}

//GET A POINT FROM POINT VECTOR
vector<int> point_vector_get_point(int point_id){

    return point_vector[point_id];
}


//SET NUMBER AND SIZE OF V-VECTORS
void v_vectors_initialization(vector<vector<int>>& v_vectors, int num_of_v_vectors, int dimensions){
    v_vectors.resize(num_of_v_vectors, vector<int>(dimensions));
}

//ASSIGN COORDINANCES TO EACH VECTOR V
//THE COORDINATES COME FROM THE GAUSSIAN DISTRIBUTION ~N(0,1)
void v_vectors_assign_coordinances(vector<vector<int>>& v_vectors, int num_of_vectors, int dimensions,
                                   std::default_random_engine& generator)
{
    int rand_int;
    normal_distribution<double> distribution(0,1);
    //INITIALIZE THE VECTORS
    v_vectors_initialization(v_vectors, num_of_vectors, dimensions);

    //ASSING VALUES USING RANDOM - INSTEAD POISSON DISTRIBUTION SHOULD BE USED
    for (int i = 0; i < v_vectors.size(); i++) {
        for (int j = 0; j < v_vectors[i].size(); j++){
            //EVERY COORDINANCE OF EACH VECTOR IS ASSIGNED TO BE BETWEEN 0 AND 101
            rand_int = distribution(generator)/1;
            v_vectors[i][j] = rand_int;
        }
    }
}

//PRINT CONTENTS OF V-VECTORS - USED FOR CHECKING PURPOSES
void v_vectors_printdata(vector<vector<int>>& v_vectors){
    for (int i = 0; i < v_vectors.size(); i++) {
        for (int j = 0; j < v_vectors[i].size(); j++){
            cout<< v_vectors[i][j] << " ";
        }
        cout << endl;
    }
}

//CREATE A VECTOR t WHICH CONTAINS k RANDOM FLOATS IN RANGE [0,w)
//THE RANDOM FLOATS COME FROM THE UNIFORM DISTRIBUTION ~Unif[0,w)
void create_vector_t(vector<float>& t, int k, int w, std::default_random_engine& generator)
{
    int i;
    float rand_float;
    uniform_real_distribution<float> distribution(0.0,(float)w);

    t.resize(k);
    for (i=0; i < t.size() ; i++) {
        rand_float= distribution(generator);
        t[i]= rand_float;
    }
}

//CREATE A VECTOR OF k RANDOM INTEGERS  IN RANGE [0,w)
//THE RANDOM INEGERS COME FROM THE UNIFORM DISTRIBUTION ~Unif[0,w)
void create_vector_int(vector<int>& ints, int k, int w, std::default_random_engine& generator)
{
    int i;
    int rand_number;
    uniform_int_distribution<> distribution(0, w);

    ints.resize(k);
    for (i=0; i < ints.size() ; i++) {
        rand_number= distribution(generator);
        ints[i]= rand_number;
    }
}

//PRINT CONTENTS OF t-VECTORS - USED FOR CHECKING PURPOSES
void print_vector_t(vector<float>& t){
    for (int i = 0; i < t.size(); i++) {
        cout << t[i] << " ";
    }
    cout << endl;
}

//INITIALIZE THE POINTS ID VECTOR
void initialize_points_ID_vector(int num_of_points, int L){
    points_ID_vector.resize(num_of_points, vector<int>(L));

}

//INSERT POINT'S ID VALUES TO ID VECTOR
//INDEX VALUE IS THE ID VALUE OF THE POINT (KEY)
void points_ID_push_back(int index_value, vector<int>& point_id_values){

    for(int i = 0; i < points_ID_vector[index_value].size(); i++){

        //points_ID_vector[index_value].push_back(point_id_values[i]);
        points_ID_vector[index_value][i] = point_id_values[i];
    }

}

//GET ID VALUE OF A POINT CORRESPONDING TO A SPECIFIC HASH TABLE
//K IS THE HASHTABLE THAT CONCERNS US - (NUMBER OF G-FUNCTION USED IN THIS HASHTABLE ({0,1,...,L})
//INDEX_VALUE IS THE ID(KEY_VALUE) OF THE POINT
int get_point_id_value(int index_value, int k){

    int id_value;
    id_value = points_ID_vector[index_value][k];

    return id_value;
}

//INSERT A POINT ID TO CENTROIDS VECTOR
void centroids_insert_point(int id){

    centroids.push_back(id);
}

//GET NUMBER OF CENTROID POINTS
int centroids_get_size(){

    return centroids.size();
}

//CALCULATE THE MINIMUM DISTANCE BETWEEN A POINT FROM INPUT AND THE CENTROIDS
float centroids_calculate_min_distance_point(vector<int>& point){


    float min_distance = numeric_limits<float>::max();
    float current_distance;

    for(int i = 0; i < centroids_get_size(); i++){

        //INITIAL CENTROIDS ARE POINTS FROM INPUT
        current_distance = calculate_distance(point_vector[centroids[i] - 1], point);

        if(current_distance < min_distance){

            min_distance = current_distance;
        }


    }

    return min_distance;

}

//CALCULATE MIN DISTANCE OF EVERY  NON CENTROID POINT TO CENTROIDS
void centroids_calculate_min_distance_input(vector<float>& points_min_distances){

    float point_min_distance;

    for(int i = 0; i < point_vector.size(); i++){

        point_min_distance = centroids_calculate_min_distance_point(point_vector[i]);

        //IF CURRENT POINT IS NOT A CENTROID
        if(point_min_distance != 0){

            points_min_distances.push_back(point_min_distance);
        }

    }
}

//PICK A POINT FROM INPUT POINTS RANDOMLY TO BE THE FIRST CENTROID
void centroids_pick_first_centroid(){

    int first_centroid_id;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);

    //WE USE UNIFORM DISTRIBUTION TO GET A POINT FROM INPUT POINTS
    uniform_int_distribution<int> p_distribution(1, point_vector.size());

    first_centroid_id = p_distribution(generator);

    //INSERT THE FIRST CENTROID ID TO CENTROIDS VECTOR
    centroids.push_back(first_centroid_id);

}

//PICK NEXT CENTROID
void centroids_pick_next_centroid(vector<float>& partial_sums){

    double x;
    int r, i;

    vector<int>::iterator it;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);

    uniform_real_distribution<float> distribution(0.0, partial_sums[partial_sums.size() - 1]);

    do{

        i = 0;
        r = 1;

        //ASSIGN TO X A VALUE IN [0, P(n-t)] USING UNIFORM DISTRIBUTION
        x = distribution(generator);

        //GET INDEX OF THE NEXT CENTROID - WE NEED AN r THAT SATISFIES THE RELATION P(r-1) < x <= P(r)
        while(x > partial_sums[i]){

            r++;
            i++;
        }

        it = find(centroids.begin(), centroids.end(), r);

    }while(it != centroids.end());  //WHILE r ALREADY EXISTS IN CENTROIDS - CALCULATE A NEW r


    //PUSH THE NEW CENTROID ID TO CENTROIDS VECTOR
    centroids.push_back(r);

}

//PRINT CENTROIDS IDS - USED FOR CHECKING PURPOSES
void centroids_print_data(){

    for(int i = 0; i < centroids.size(); i++){
        cout << centroids[i] << endl;
    }
}

int get_centroids_id(int i){
    return centroids[i];
}

void set_centroids_id(vector<int> v){
    int i;

    for(i=0 ; i < v.size() ; i++) {
        centroids[i]= v[i];
    }
}

//CALCULATE DOT PRODUCT OF TWO VECTORS
int calculate_dot_product(const vector <int>& point, vector <int>& d_vector){
    int product = 0;
    for(int i = 1; i < point.size(); i++){
        product = product + point[i] * d_vector[i-1];
    }
    return product;
}

//CALCULATES THE ADDITION OF TWO INTEGER VECTORS OF THE SAME SIZE
//EG. v1= [0, 2, 4, 8]  v2= [0, 1, -1, -5]  v1+v2= v3= [0, 3, 3, 3]
vector<int> add_vectors(vector<int>& point1, vector<int>& point2)
{
    int i;
    vector<int> point3;

    if (point1.size() != point2.size()) {
        cerr << "Error in add_vectors: Can not add vectors of different size" << endl;
        point3.assign(point1.size(), -666);
    }
    else {
        point3.assign(point1.size(), 0);
        for (i=1 ; i < point1.size(); i++) {
            point3[i]= point1[i] + point2[i];
        }
    }
    return point3;
}

//RECEIVES A VECTOR OF SUMS AND THE NUMBER OF VECTORS THAT WERE ADDED
//RETURNS A VECTOR THAT CONTAINS THE FLOOR OF THE MEAN VALUE FOR EACH COORDINATE
vector<int> find_mean_vector(vector<int> vector_of_sums, int num_of_vectors)
{
    int i;
    int mean_value;
    float float_mean_value;
    vector<int> mean_vector;

    for (i=1; i < vector_of_sums.size() ; i++) {
        float_mean_value= vector_of_sums[i]/(float)num_of_vectors;
        //ROUNDING OF THE EXACT MEAN VALUE SO THAT IT WILL BE STORED AS AN INTEGER
        if (int(float_mean_value*10) %10 >= 5) {
            mean_value= (float_mean_value+1)/1;
        }
        else {
            mean_value= float_mean_value/1;
        }
        mean_vector.push_back(mean_value);
    }
    return mean_vector;
}

//COMPUTES THE DISTANCE BETWEEN 2 VECTORS USING THE k-NORM
float calculate_distance(vector<int>& point1, const vector<int>& point2, int k)
{
    float distance = 0.0;
    int sum = 0;

    for (int i=1 ; i < point1.size() ; i++) {
        sum+= pow(abs(point1[i]-point2[i]), k);
    }
    distance = pow(sum, 1.0/(float)k);

    return distance;
}

//K-MEANS++: GET A VECTOR WITH ALL THE MINIMUM DISTANCES
//OF NON CENTROID POINTS TO CENTROIDS
//AND CALCULATE THEIR PARTIAL SUMS
//ALSO HOLD THE LAST PARTIAL SUMS
float calculate_partial_sums(vector<float>& min_distances, vector<float>& partial_sums){

    float sums = 0;
    float last_partial_sum;

    for (int i = 0; i < min_distances.size(); i++){

        sums = sums + pow(min_distances[i], 2);
        partial_sums.push_back(sums);

        if(i == min_distances.size()){
            last_partial_sum = sums;
        }
    }

    return last_partial_sum;

}
