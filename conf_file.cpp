#include "conf_file.h"

void read_configuration_file(fstream& config_file, string config_file_name, int& k_cluster, int& L, 
                                                            int& k, int& M_cube, int& k_cube, int& probes){

    int start;
    int finish = 0;
    int count = 0;
    string line;
    string token;

    //OPEN CLUSTER CONFIGURATION FILE
    open_file(&config_file, config_file_name, fstream::in);

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