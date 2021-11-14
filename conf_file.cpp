#include "conf_file.h"

void read_configuration_file(fstream& config_file, string config_file_name, int& k_cluster, int& L,
                                                            int& k, int& M_cube, int& k_cube, int& probes){

    int start;
    int finish = 0;
    int count = 0;
    string line;
    string token_text, token_number;

    //OPEN CLUSTER CONFIGURATION FILE
    open_file(&config_file, config_file_name, fstream::in);

    while(getline(config_file, line)){

        start = 0;
        while(start < line.size()){

            start = line.find_first_of(' ', start);

            finish = line.size();

            //CAPTURE THE TOKENS IN EVERY LINE OF THE CONFIGURATION FILE
            token_text= line.substr(0, start-1);
            token_number = line.substr(start, finish - start);
            //INITIALIZE THE CORRESPONDING VARIABLE ACCORDING TO THE TOKENS CAPTURED
            if(token_text == "number_of_clusters"){
                k_cluster = stoi(token_number);
            }
            else if(token_text == "number_of_vector_hash_tables"){
                L = stoi(token_number);
            }
            else if(token_text == "number_of_vector_hash_functions"){
                k = stoi(token_number);
            }
            else if(token_text == "max_number_M_hypercube"){
                M_cube = stoi(token_number);
            }
            else if(token_text == "number_of_hypercube_dimensions"){
                k_cube = stoi(token_number);
            }
            else if(token_text == "number_of_probes"){
                probes = stoi(token_number);
            }
            else {
                cerr << "Invalid argument: "<< token_text <<" in configuration file" << endl;
            }

            start = finish;
        }


    }

    close_file(&config_file);

}
