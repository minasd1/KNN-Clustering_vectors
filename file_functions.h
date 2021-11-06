#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

void open_file(fstream* file, const string& filename, ios_base::openmode mode);
void seek_in_file(fstream* file, streampos position);
void close_file(fstream* file);



#endif
