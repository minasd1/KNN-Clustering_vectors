#include <string>
#include <cstdio>
#include <iostream>

using namespace std;

//Return codes: -1 ->Input file is missing, or wrong args
//               0 ->A query file was given in the command line
//               1 ->No query file was given in the command line
//Initializes program's variables with command line arguments
int read_cmd_args_lsh(int argc, char** argv, string& input_file, string& query_file,
                      int& k, int& l, string& output_file, int& n, float& r, int& m, int& probes)
{
    int i;
    bool i_flag, k_flag, l_flag, o_flag, q_flag,n_flag, r_flag, p_flag, m_flag;

    //Flags for given arguments (false for missing args)
    i_flag= false;
    k_flag= false;
    l_flag= false;
    o_flag= false;
    q_flag= false;
    n_flag= false;
    r_flag= false;
    p_flag= false;
    m_flag= false;

    if (argc < 5) {     //At least ./program, -i <input_file> -q <query_file> must be given
        cerr << "Not all arguments were given\nPlease run the program as below:\n\n\
        ./lsh -i <input_file> -q <query_file>" << endl;
        return -1;
    }
    for (i=1; i < argc ; i+=2) { //For every other argument
        if((string)argv[i] == "-i"){
            i_flag= true;
            input_file= argv[i+1];
        }
        else if((string)argv[i] == "-q") {
            q_flag= true;
            query_file= argv[i+1];
        }
        else if((string)argv[i] == "-k") {
            k_flag= true;
            k= stoi(argv[i+1]);
        }
        else if((string)argv[i] == "-L") {
            l_flag= true;
            l= stoi(argv[i+1]);
        }
        else if((string)argv[i] == "-o") {
            o_flag= true;
            output_file= argv[i+1];
        }
        else if((string)argv[i] == "-N") {
            n_flag= true;
            n= stoi(argv[i+1]);
        }
        else if((string)argv[i] == "-R") {
            r_flag= true;
            r= stof(argv[i+1]);
        }
        else if((string)argv[i] == "-M" && (string)argv[0] == "./cube") {
            m_flag= true;
            m= stoi(argv[i+1]);
        }
        else if((string)argv[i] == "-probes" && (string)argv[0] == "./cube") {
            p_flag= true;
            probes= stoi(argv[i+1]);
        }
        else {
            cerr << "Wrong input arguent: " << argv[i] << endl;
            return -1;
        }
    }
    if (i_flag && q_flag) {
        //Initialize missing arguments with default values
        if (!k_flag)
            k= 4;
        if (!l_flag && (string)argv[0]=="./lsh")
            l= 5;
        if(!n_flag)
            n= 1;
        if(!r_flag)
            r= 10000.0;
        if (!m_flag && (string)argv[0]=="./cube")
            m= 10;
        if (!p_flag && (string)argv[0]=="./cube")
            probes= 2;
        if (!o_flag)
            output_file= "output_file";
        return 0;
    }
    else {
        cerr << "The input file and/or the query file is missing" << endl;
        return -1;
    }
}
