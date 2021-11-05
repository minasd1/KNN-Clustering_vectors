//RECEIVES THE MAXIMUM NUMBER OF CUBE VERTICES (probes) TO BE SEARCHED FOR NEAREST NEIGHBORS
//AND THE INITIAL VERTEX
//RETURNS A TABLE OF VERTICES (HASHTABLE INDEXES) IN ASCENDING HAMMING DISTANCE
//THAT CONTAINS probes ELEMENTS
vector<unsigned int> get_relative_buckets(unsigned int current_vertex,
                                          int probes, int string_dimensions) //LAST ARG IS d'
{
    int i, j;
    int num_of_buckets;

    num_of_buckets= pow(2,string_dimensions);

    for(i=1; i <= string_dimensions ; i++) {
        for (j=0; j < num_of_buckets) {
            if ()
        }
    }
}
