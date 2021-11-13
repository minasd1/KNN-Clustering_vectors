#ifndef CONF_FILE_H
#define CONF_FILE_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "file_functions.h"

using namespace std;

void read_configuration_file(fstream& config_file, string config_file_name, int& k_cluster, int& L, 
                                                            int& k, int& M_cube, int& k_cube, int& probes);














#endif