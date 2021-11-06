#ifndef KNN_TABLE_FUNCTIONS_H
#define KNN_TABLE_FUNCTIONS_H

#include <vector>

using namespace std;

typedef struct dist_id_pair {
    float dist; //DISTANCE BETWEEN POINT AND QUERY POINT
    int id; //ID OF THE POINT
}dist_id_pair;

bool compare_distance(const dist_id_pair& pair1, const dist_id_pair& pair2);
bool already_exist(const vector<dist_id_pair>& table, int id);
void insert_at_correct_place(vector<dist_id_pair>& table, dist_id_pair& pair);

#endif
