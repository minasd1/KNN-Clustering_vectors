#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static vector<vector <vector <int> > > HashTables;  //DECLARE A VECTOR OF HASHTABLES

void hashTable_initialization(int num_of_hashTables, int num_of_buckets);
void hashTable_push_back(vector<int>& g, int key_val);
vector<int> hashTable_get_points_in_buckets(vector<int>& g);
int hashTable_get_num_of_htables();
int hashTable_get_bucket_size(int htable_num, int bucket);
int hashTable_get_point(int htable_num, int bucket, int place);
void hashTable_print_data();




















#endif