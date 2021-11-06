#include "file_functions.h"

void open_file(fstream* file, const string& filename, ios_base::openmode mode){

    try{
        (*file).open(filename, mode);
        ios_base::iostate exceptionMask = (*file).exceptions() | ios_base::failbit;
        (*file).exceptions(exceptionMask);

    }catch(ios_base::failure& e){
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    (*file).exceptions(ios_base::goodbit);

}

void seek_in_file(fstream* file, streampos position){

    try{
        (*file).seekg(position);
        ios_base::iostate exceptionMask = (*file).exceptions() | ios_base::failbit | ios_base::badbit | ios_base::eofbit;
        (*file).exceptions(exceptionMask);
    }catch(ios_base::failure& e){
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    (*file).exceptions(ios_base::goodbit);
}



void close_file(fstream* file){

    (*file).clear();

    try{
        (*file).close();
        ios_base::iostate exceptionMask = (*file).exceptions() | ios_base::failbit;
        (*file).exceptions(exceptionMask);
    }catch(ios_base::failure& e){
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }

}
