#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "file_functions.h"

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
    
    vector<vector<int>> point_vector;

    
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
        first_iteration = false;
    }

    close_file(&input_file);
   
    return 0;
    
}
