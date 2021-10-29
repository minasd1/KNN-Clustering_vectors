#ifndef CMD_LINE_ARGS_H
#define CMD_LINE_ARGS_H
#include <string>

using namespace std;

int read_cmd_args_lsh(int argc, char** argv, string& input_file, string& query_file,
                      int& k, int& l, string& output_file, int& n, float& r, int& m, int& probes);

#endif
