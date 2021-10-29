#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static vector<vector <vector <int> > > HashTables;  //DECLARE A VECTOR OF HASHTABLES

void hashTable_initialization(int num_of_hashTables, int num_of_buckets);
void hashTable_push_back(vector<int>& g,int key_val);
vector<int> hashTable_get_points_in_buckets(vector<int>& g);
void hashTable_print_data();




















#endif