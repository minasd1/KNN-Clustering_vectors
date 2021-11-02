#include "lsh.h"

// COMPARES TWO PAIRS ACCORDING TO DISTANCE (TRUE WHEN THE SECOND'S DISTANCE IS GREATER)
bool compare_distance(const dist_id_pair& pair1, const dist_id_pair& pair2)
{
    return (pair1.dist < pair2.dist);
}

//CHECKS IF THE ID ALREADY EXISTS IN THE VECTOR
bool already_exist(const vector<dist_id_pair>& table, int id)
{
    int i;

    for (i=0; i < table.size() ; i++) {
        if (table[i].id == id) {
            return true;
        }
    }
    return false;
}

//INSERTS A NEW PAIR IN THE CORRECT PLACE IN A SORTED TABLE OF PAIRS AND REMOVES THE LAST (k+1th) PAIR FROM THE TABLE
void insert_at_correct_place(vector<dist_id_pair>& table, dist_id_pair& pair)
{
    int i, j, last_index;
    dist_id_pair dummy_pair;

    dummy_pair.dist=0.0;
    dummy_pair.id= -1;
    i= 0;
    while (table[i].dist < pair.dist) {
        i++;
    }

    table.push_back(dummy_pair);
    for (j=table.size()-1; j >= i ; j--) {
        table[j+1].dist= table[j].dist;
        table[j+1].id= table[j].id;
    }
    table.pop_back();
    table[i].dist= pair.dist;
    table[i].id= pair.id;
}

//RECEIVES THE QUERY_POINT'S IDS AND THE CANDIDATE_POINTS' IDS
//RETURNS IF THERE IS AT LEAST ONE POINT IN THE CANDIDATES SET WITH THE SAME ID WITH THE QUERY_POINT
bool is_there_someone_with_same_id(vector<int>& query_points_ids, vector<int>& candidate_points, G& g,
                                   int& same_id_counter, vector<int>& points_with_same_id) {
    int i;
    vector<int> current_candidate_ids; //THE IDs OF A CANDIDATE POINT
    vector<int> current_candidate;
    bool is_there_at_least_one= false;

    same_id_counter= 0;
    for (i=0; i < candidate_points.size() ; i+=2) {
        current_candidate= point_vector_get_point(candidate_points[i+1]-1);
        g.id(current_candidate, current_candidate_ids, true);

        if (query_points_ids[candidate_points[i]] == current_candidate_ids[candidate_points[i]] ) {
            points_with_same_id.push_back(candidate_points[i]); //IN WHICH HASHTABLE WAS THE POINT FOUND
            points_with_same_id.push_back(candidate_points[i+1]); //WHICH POINT IS IT
            is_there_at_least_one= true;
            same_id_counter++;
        }
    }
    return is_there_at_least_one;
}

//RECEIVES A QUERY POINT AND RETURNS THE FIRST k NEAREST NEIGHBORS IN ASCENDING DISTANCE ORDER
//THE LAST ARGUMENT IS OPTIONAL AND SHOWS THE MAXIMUM NUMBER OF POINTS TO BE EXAMINED AS POSSIBLE NEAREST NEIGHBORS
//IF NO FORTH ARGUMENT IS GIVEN THEN ALL THE POINTS IN THE SAME BUCKETS WITH query_point ARE EXAMINED
vector<dist_id_pair> find_approximate_knn(vector<int> query_point, int k, G& g, int max_candidates)// THIRD ARG IS THE OBJECT OF CLASS G DECLEARED IN MAIN FUNCTION
{
    int points_in_table_counter= 0; //A COUNTER OF THE ELEMENTS INSIDE THE nn_table
    vector<dist_id_pair> nn_table; //A TABLE IN WHICH THE PAIRS OF {DISTANCE, ID} OF THE NEAREST NEIGHBORING POINTS ARE STORED IN ASCENDIND DISTANCE ORDER
    vector<int> candidate_points; //A TABLE OF ALL THE {HASHTABLE_INDEX, POINT_ID} PAIRS, WHERE POINT_ID BELONGS TO A POINT THAT BELONG TO THE SAME BUCKET WITH THE query_point IN ANY OF THE L HASHTABLES
                                  //PERHAPS ONE POINT EXISTS MORE THAN ONCE IN THE candidate_points TABLE BECAUSE IT APPEARS IN MORE THAN ONE HASHTABLES
    vector<int> points_with_same_id; //A TABLE OF PAIRS WHERE THE CANDIDATES WITH THE SAME ID WITH QUERY_POINT'S ID ARE STORED
    vector<int> buckets_indexes; //THE L BUCKETS' INDEXES WHERE THE QUERY POINT WOULD BELONG IF IT WAS HASHED BY THE L G_HASH_FUNCTIONS
    vector<int> query_points_ids;//A TABLE OF L QUERY_POINT'S IDs, ONE FOR EACH HASH TABLE
    float max_distance; //THE DISTANCE BETWEEN THE QUERY POINT AND IT'S k-th NEAREST NEIGHBORS
    float distance;
    int candidates_counter; //A COUNTER OF THE POINTS THAT HAVE BEEN COMPARED
    vector<int> current_candidate;
    vector<int> current_candidate_ids; //THE IDs OF A CANDIDATE POINT
    dist_id_pair current_pair;
    int i;
    bool at_least_one_candidate_has_same_id;
    int same_id_counter;

    //HASH THE QUERY POINT TO FIND THE BUCKETS WHERE IT WOULD BELONG
    g.hash(query_point, buckets_indexes, true);
    //FIND QUERY POINT'S ID FOR EACH HASH TABLE
    g.id(query_point, query_points_ids, true);
    //GET ALL THE CANDIDATE POINTS IDS AND THE HASHTABLE WHERE EACH OF THE WAS FOUND
    candidate_points= hashTable_get_points_in_buckets(buckets_indexes);
    //SEARCH IF THERE IS AT LEAST ONE POINT IN CANDIDATES SET WITH SAME ID WITH THE QUERY POINT
    at_least_one_candidate_has_same_id= is_there_someone_with_same_id(query_points_ids,
    candidate_points, g, same_id_counter, points_with_same_id);
    //IF THE BOOLEAN ABOVE IS TRUE ALL THE CANDIDATE POINTS WITH DIFFRENT ID THAN THE QUERY WILL BE SKIPED
    //IF ITS FALSE THEN ALL THE CANDIDATES WILL BE EXAMINED AS POSSIBLE NEAREST NEIGHBORS

    candidates_counter= 0;
    //FIRST CHECK THE CANDIDATE_POINTS WITH THE SAME ID WITH QUERY POINT
    for (i= 0; i < points_with_same_id.size() ; i+=2) {
        if (candidates_counter == max_candidates) { //DONT COMPARE MORE THAN max_candidates POINTS
            break;
        }
        //GET THE CURRENT CANDIDATE POINT'S COORDINATES AND IT's DISTANCE FROM QUERY POINT
        current_candidate= point_vector_get_point(points_with_same_id[i+1]-1);//+1 BECAUSE candidate_points IS [HASHTABLE, POINT_INDEX, HASHTABLE, ...],
                                                                                        //-1 BECAUSE POINT_VECTOR STARTS FROM 0 WHILE POINTS' IDS START FROM 1
        distance= calculate_distance(query_point, current_candidate);
        //CREATE A PAIR WITH THESE TWO VALUES
        current_pair.dist= distance;
        current_pair.id= current_candidate[0];

        if (points_in_table_counter < k) { //THE FIRST k CANDIDATES TO COME ARE DIRECTLY PUSHED INTO THE NEAREST NEIGHBORS VECTOR
            if (!already_exist(nn_table, current_pair.id)) {
                nn_table.push_back(current_pair);
                points_in_table_counter++;
            }
            candidates_counter++;
        }
        else if (points_in_table_counter == k) { //WHEN THE nn_table HAS BEEN FILLED FULLY FOR THE FIRST TIME
            sort(nn_table.begin(), nn_table.end(), compare_distance); //SORT THE FIRST k PAIRS IN ASCENDING DISTANCE ORDER
            max_distance= nn_table[k-1].dist; //THE LAST ELEMENT OF THE nn_table HAS THE MAXIMUM DISTANCE FROM QUERY POINT
            if (current_pair.dist < max_distance) {
                insert_at_correct_place(nn_table, current_pair);
            }
            candidates_counter++;
        }
        else { //IF THE nn_table ALREADY HAS k ID-DISTANCE PAIRS
            if (current_pair.dist < max_distance) {
                insert_at_correct_place(nn_table, current_pair);
            }
            candidates_counter++;
        }
    }

    //IF CANDIDATE POINTS WITH SAME ID WITH QUERY_POINT ARE NOT ENOUGH
    if (points_in_table_counter < k) {
        //SEARCH FOR THE REST BEST NEIGHBORS AMONG THE WHOLE CANDIDATE_POINTS DATASET

        //FOR ALL THE CANDIDATE POINTS
        for (i=0; i < candidate_points.size() ; i+=2) { //FOR EVERY CANDIDATE POINT
            if (candidates_counter == max_candidates) { //DONT COMPARE MORE THAN max_candidates POINTS
                break;
            }
            //GET THE CURRENT CANDIDATE POINT'S COORDINATES AND IT's DISTANCE FROM QUERY POINT
            current_candidate= point_vector_get_point(candidate_points[i+1]-1);//+1 BECAUSE candidate_points IS [HASHTABLE, POINT_INDEX, HASHTABLE, ...],
                                                                                            //-1 BECAUSE POINT_VECTOR STARTS FROM 0 WHILE POINTS' IDS START FROM 1
            g.id(current_candidate, current_candidate_ids, true); //ALTHOUGH current_candidate IS AN INPUT POINT WE USE 'true' SO THAT THE TABLE WITH INPUT POINTS IDS WONT HAVE DUBLICATES

            //IF QUERY_POINT'S ID != CANDIDATE_POINT'S ID, BUT EXIST AT LEAST ONE CANDIDATE WITH SAME ID
            if (current_candidate_ids[candidate_points[i]] != query_points_ids[candidate_points[i]]
                && at_least_one_candidate_has_same_id) {
                candidates_counter++;
                continue; //DONT CHECK POINTS WITH DIFFRENT ID UNLESS NONE OF THEM HAS THE SAME ID WITH QUERY_POINT
            }
            distance= calculate_distance(query_point, current_candidate);
            //CREATE A PAIR WITH THESE TWO VALUES
            current_pair.dist= distance;
            current_pair.id= current_candidate[0];

            if (points_in_table_counter < k) { //THE FIRST k CANDIDATES TO COME ARE DIRECTLY PUSHED INTO THE NEAREST NEIGHBORS VECTOR
                if (!already_exist(nn_table, current_pair.id)) {
                    nn_table.push_back(current_pair);
                    points_in_table_counter++;
                }
                candidates_counter++;
            }
            else if (points_in_table_counter == k) { //WHEN THE nn_table HAS BEEN FILLED FULLY FOR THE FIRST TIME
                sort(nn_table.begin(), nn_table.end(), compare_distance); //SORT THE FIRST k PAIRS IN ASCENDING DISTANCE ORDER
                max_distance= nn_table[k-1].dist; //THE LAST ELEMENT OF THE nn_table HAS THE MAXIMUM DISTANCE FROM QUERY POINT
                if (current_pair.dist < max_distance) {
                    insert_at_correct_place(nn_table, current_pair);
                }
                candidates_counter++;
            }
            else { //IF THE nn_table ALREADY HAS k ID-DISTANCE PAIRS
                if (current_pair.dist < max_distance) {
                    insert_at_correct_place(nn_table, current_pair);
                }
                candidates_counter++;
            }
        }
        if (nn_table.size() < k) { //JUST IN CASE THERE ARE LESS THAN k CANDIDATE POINTS IN THE BUCKETS
            sort(nn_table.begin(), nn_table.end(), compare_distance);
        }
    }
    return nn_table;
}

//RECEIVES A QUERY POINT AND RETURNS THE FIRST k EXACT NEAREST NEIGHBORS
//IN ASCENDING DISTANCE ORDER USING BRUTE FORCE
vector<dist_id_pair> find_exact_knn(vector<int> query_point, int k, int num_of_points)
{
    int i;
    vector<dist_id_pair> nn_table; //A TABLE IN WHICH THE PAIRS OF {DISTANCE, ID} OF THE NEAREST //NEIGHBORING POINTS ARE STORED IN ASCENDIND DISTANCE ORDER
    vector<int> current_candidate;
    dist_id_pair current_pair;
    int points_in_table_counter= 0;
    float max_distance, distance;

    //FOR ALL THE CANDIDATE POINTS
    for (i=0; i < num_of_points ; i++) {
        current_candidate= point_vector_get_point(i);
        distance= calculate_distance(query_point, current_candidate);
        //CREATE A PAIR WITH THESE TWO VALUES
        current_pair.dist= distance;
        current_pair.id= current_candidate[0];

        if (points_in_table_counter < k) { //THE FIRST k CANDIDATES TO COME ARE DIRECTLY PUSHED INTO THE NEAREST NEIGHBORS VECTOR
            if (!already_exist(nn_table, current_pair.id)) {
                nn_table.push_back(current_pair);
                points_in_table_counter++;
            }
        }
        else if (points_in_table_counter == k) { //WHEN THE nn_table HAS BEEN FILLED FULLY FOR THE FIRST TIME
            sort(nn_table.begin(), nn_table.end(), compare_distance); //SORT THE FIRST k PAIRS IN ASCENDING DISTANCE ORDER
            max_distance= nn_table[k-1].dist; //THE LAST ELEMENT OF THE nn_table HAS THE MAXIMUM DISTANCE FROM QUERY POINT
            if (current_pair.dist < max_distance) {
                insert_at_correct_place(nn_table, current_pair);
            }
        }
        else { //IF THE nn_table ALREADY HAS k ID-DISTANCE PAIRS
            if (current_pair.dist < max_distance) {
                insert_at_correct_place(nn_table, current_pair);
            }
        }
    }
    if (nn_table.size() < k) { //JUST IN CASE THERE ARE LESS THAN k CANDIDATE POINTS IN THE BUCKETS
        sort(nn_table.begin(), nn_table.end(), compare_distance);
    }
    return nn_table;
}


//IMPLEMENT APPROXIMATE RANGE SEARCH ALGORITHM
vector<int> range_search(vector<int>& g, int radius, vector<int>& query_point){

    int retrieved_items = 0;
    int max_retrieved_items = 20* hashTable_get_num_of_htables();
    vector<int> points_in_range;

    //ACCESS TO I-TH HASHTABLE
    for(int i = 0; i < hashTable_get_num_of_htables(); i++){

        //ACCESS TO THE SPECIFIC BUCKET THAT I-TH G FUNCTION INDICATES
        for(int j = 0; j < hashTable_get_bucket_size(i, g[i]); j++){

            //IF DISTANCE OF J-TH POINT IN THIS BUCKET IS IN THE GIVEN RADIUS
            if((calculate_distance(query_point, point_vector_get_point(hashTable_get_point(i, g[i], j) - 1), 2)) < radius){
                //THEN ADD IT'S ID TO POINTS_IN_RANGE VECTOR
                points_in_range.push_back(hashTable_get_point(i, g[i], j));
                retrieved_items++;

                if(retrieved_items > max_retrieved_items){

                    return points_in_range;
                }
            }
        }
    }

    return points_in_range;
}
