#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "partitioning.h"
#include <omp.h>
#include "time.h"
#include "unistd.h"

using namespace std;

///     ALGORITHM STEPS
///     1 - file reading and graph structure creating
///     2 - minimum spanning tree (mst) creating
///     3 - largest connected component searching
///     4 - parallelism

int main(int argc, char** argv)
{
    // get 'int' input value
    long arg = strtol(argv[1], NULL, 10);
    // starts time couting
    double start_time = omp_get_wtime();
    // original graph file
    const char* arq = "dblp.txt";
    // mst file
    const char* arq2 = "mst.txt";


            ///  1 - GRAPH CREATION  ///
            // retrieve highest id value ( explanation: + 1 because we need this id as a vector position )
    int big = find_biggest_value(arq) + 1;
            // space allocation
    Node *graph = (Node*) malloc(sizeof(Node)*(big));

            ///  2 - if mst file doens't exists, create it  ///
    if( access( "mst.txt", F_OK ) == -1 ) {
                // create graph and merge bi-directed edges weight ( explanation: we deal with no-directed graph )
        create_graph(arq, graph, big, 1);
                // change structure for mst calculus
        struct Graph* mst_graph = migrate_structure(graph,big);
                // kruskal algorithm
        KruskalMST(mst_graph, big);
                // memory deallocation
        delete[] mst_graph->edge;
        delete(mst_graph);
        free_graph(graph,big);
    }

            ///  3 - largest connected component searching  ///

            // the graph struct is an array with size = 'big'
    create_graph(arq2, graph, big, 0);
            // retrive only the biggest component and it's size
    int bigger_comp = UGRAPHcc(graph, big);
    printf("bigger comp: %d", bigger_comp);

            ///  4 - parallelism zone  ///

            // set upper and lower bounds
    int upper_lim = (bigger_comp/2.0) + (bigger_comp/20.0);
    int low_lim = (bigger_comp/2.0) - (bigger_comp/20.0);

    omp_set_dynamic(0);
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        #pragma omp single
        partitioning(graph, big, upper_lim, low_lim, arq, 1, arg);
    }



            /// 5 - print execution time
    double time = omp_get_wtime() - start_time;
    printf("\n\nTIME: %lf", time);

    remove("mst.txt");
    return 0;
}
