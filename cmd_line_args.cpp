#include <string>
#include <cstdio>
#include <iostream>

using namespace std;

//Return codes: -1 ->Input file is missing, or wrong args
//               0 ->A query file was given in the command line
//               1 ->No query file was given in the command line
//Initializes program's variables with command line arguments
int read_cmd_args(int argc, char** argv, string& input_file, string& query_file,
                      int& k_lsh, int& k_cube, int& l, string& output_file, int& n,
                      float& r, int& m, int& probes, string& config_file, string& method)
{
    int i;
    bool i_flag, k_lsh_flag, k_cube_flag, l_flag, o_flag, q_flag,n_flag, r_flag,
         p_flag, m_flag, c_flag, method_flag, complete_flag;

    //Flags for given arguments (false for missing args)
    i_flag= false;
    k_lsh_flag= false;
    k_cube_flag= false;
    l_flag= false;
    o_flag= false;
    q_flag= false;
    n_flag= false;
    r_flag= false;
    p_flag= false;
    m_flag= false;
    c_flag= false;
    method_flag= false;
    complete_flag= false;

    if (argc < 5 && ((string)argv[0] == "./lsh"  || (string)argv[0] == "./cube")) {     //At least ./program, -i <input_file> -q <query_file> must be given
        cerr << "Not all arguments were given\nPlease run the program as below:\n" << endl;
        if ((string)argv[0] == "./lsh"){
            cerr << "./lsh -i <input_file> -q <query_file>" << endl;
        }
        else if ((string)argv[0] == "./cube") {
            cerr << "./cube -i <input_file> -q <query_file>" << endl;
        }
        return -1;
    }
    if (argc < 7 && (string)argv[0] == "./cluster") {
        cerr << "Not all arguments were given\nPlease run the program as below:\n" << endl;
        cerr << "./cluster -i <input_file> -c <configuration_file> -m <Classic OR LSH or Hypercube>" << endl;
        return -1;
    }
    for (i=1; i < argc ; i+=2) { //For every other argument
        if((string)argv[i] == "-i"){
            i_flag= true;
            input_file= argv[i+1];
        }
        else if((string)argv[i] == "-q" && ((string)argv[0] == "./lsh"  || (string)argv[0] == "./cube")) {
            q_flag= true;
            query_file= argv[i+1];
        }
        else if((string)argv[i] == "-k" && (string)argv[0] == "./lsh") {
            k_lsh_flag= true;
            k_lsh= stoi(argv[i+1]);
        }
        else if((string)argv[i] == "-L" && (string)argv[0] == "./lsh") {
            l_flag= true;
            l= stoi(argv[i+1]);
        }
        else if((string)argv[i] == "-o") {
            o_flag= true;
            output_file= argv[i+1];
        }
        else if((string)argv[i] == "-N" && ((string)argv[0] == "./lsh"  || (string)argv[0] == "./cube")) {
            n_flag= true;
            n= stoi(argv[i+1]);
        }
        else if((string)argv[i] == "-R" && ((string)argv[0] == "./lsh"  || (string)argv[0] == "./cube")) {
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
        else if((string)argv[i] == "-k" && (string)argv[0] == "./cube") {
            k_cube_flag= true;
            k_cube= stoi(argv[i+1]);
        }
        else if ((string)argv[i] == "-c" && (string)argv[0] == "./cluster") {
            c_flag= true;
            config_file= argv[i+1];
        }
        else if ((string)argv[i] == "-complete" && (string)argv[0] == "./cluster") {
            complete_flag= true;
        }
        else if ((string)argv[i] == "-m" && (string)argv[0] == "./cluster") {
            if ((string)argv[i+1] == "Classic" || (string)argv[i+1] == "classic"
                || (string)argv[i+1] == "CLASSIC"){
                    method_flag= true;
                    method= "classic";
            }
            else if ((string)argv[i+1] == "Lsh" || (string)argv[i+1] == "lsh"
                || (string)argv[i+1] == "LSH") {
                    method_flag= true;
                    method= "lsh";
            }
            else if ((string)argv[i+1] == "Hypercube" || (string)argv[i+1] == "hypercube"
                || (string)argv[i+1] == "HYPERCUBE") {
                    method_flag= true;
                    method= "hypercube";
            }
            else {
                cerr << "Unknown method \"" << argv[i+1] << "\". " << "Please run the program with one of the acceptable methods:" << endl;
                cerr << "(Classic, Lsh, Hypercube)" << endl;
                return -1;
            }
        }
        else {
            cerr << "Wrong input arguent: " << argv[i] << endl;
            return -1;
        }
    }
    if ((string)argv[0] == "./lsh"  || (string)argv[0] == "./cube" ) {
        if (i_flag && q_flag) {
            //Initialize missing arguments with default values
            if (!k_lsh_flag)
                k_lsh= 4;
            if  (!k_cube_flag)
                k_cube= 14;
            if (!l_flag)
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
    else if ((string)argv[0] == "./cluster"){
        if (!i_flag || !method_flag || !c_flag) {
            cerr << "At least one of the following is missing: ";
            cerr << "(Input file, Configuration file, Method)" << endl;
            cerr << "Please run the program as below:\n" << endl;
            cerr << "./cluster -i <input_file> -c <configuration_file> -m <Classic OR LSH or Hypercube>" << endl;
            return -1;
        }
        else {
            //INITIALIZE LSH AND HYPERCUBE ARGUMENTS WITH THE DEFAULT VALUES
            l= 3;
            k_lsh= 4;
            k_cube= 3;
            m= 10;
            probes= 2;
            n= 1;
            if (!o_flag) {
                output_file= "output_file";
            }
        }
    }
    return 0;
}
