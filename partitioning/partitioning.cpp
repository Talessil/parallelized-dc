#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "partitioning.h"
#include <omp.h>

using namespace std;

int name[4] = {0,0,0,0};



                    ///  GRAPH FUNCTIONS  ////

void free_graph(Node *graph, int big){
    Adj *aux, *aux2;
    for(int i = 0; i < big; i++){
        aux = graph[i].adj;
        while(aux != nullptr){
            aux2 = aux->next;
            free(aux);
            aux = aux2;
        }
    }
}

/* seek and return highest node id value */
int find_biggest_value(const char *arq){
    ifstream myfile (arq);
    int a, b, bigger = 0;
    float w;
    if (myfile.is_open())
    {
      while ( myfile >> a >> b >> w)
      {
          if (a > bigger) bigger = a;
          if (b > bigger) bigger = b;
      }
      myfile.close();
    }
    else cout << "Unable to open file" << endl;
    return bigger;
}

/* create graph : read file 'arq' and set a graph structure */
void create_graph(const char * arq, Node * graph, int big, int flag){

    ifstream myfile (arq);
            // initializes
    for(int i = 0; i < big; i++){
        graph[i].value = 0;
        graph[i].adj = nullptr;
        graph[i].degree = 0;
    }

    int a, b;
    float w;
            // read file and insert nodes/edges/degree
    if (myfile.is_open())
    {
      while ( myfile >> a >> b >> w)
      {
          graph[a].value = 1;
          graph[b].value = 1;
          insert_edge(graph,a,b,w,flag);
      }
      myfile.close();
    }
    else cout << "Unable to open file" << endl;
}

/* insert new edge */
void insert_edge(Node * graph, int a, int b, float w, int flag){
            // if flag == 1, do :
            // if edge already exists: sum 2 edges weight
    if(flag == 1){
        Adj * aux = graph[b].adj;
        while( aux != nullptr ){
            if(aux->successor == a){
                aux->weight = (aux->weight + w)/2;
                return;
            }
            aux = aux->next;
        }
    }
            // create edge as a adjacence list
    Adj *edge = (Adj*) malloc(sizeof(Adj));
    edge->successor = b;
    edge->weight = w;
    edge->next = nullptr;
    graph[a].degree++;
    graph[b].degree++;
    if(graph[a].adj == nullptr){
        graph[a].adj = edge;
    }else{
        Adj * aux = graph[a].adj;
        while(aux->next!=nullptr)
        {
            aux = aux->next;
        }
        aux->next = edge;
    }
}

/* initialize mst structure */
struct Graph* createMSTGraph(int V, int E){
    struct Graph* graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edge = new Edge[E];
    return graph;
}

/* auxiliary function */
struct Graph* migrate_structure(Node * graph, int big){
            // count edges and nodes number
    int nodes_cont = 0, edges_cont = 0;
    for ( int i = 0 ; i < big ; i++ ){
        if ( graph[i].value == 1 ){
            nodes_cont ++;
            Adj * aux = graph[i].adj;
            while ( aux != nullptr ){
                edges_cont ++;
                aux = aux->next;
            }
        }
    }
            // initializes mst graph
    struct Graph* mst_graph = createMSTGraph(nodes_cont, edges_cont);
    int j = 0;
    for ( int i = 0 ; i < big ; i++ ){
        if ( graph[i].value == 1 ){
            Adj * aux = graph[i].adj;
            while ( aux != nullptr ){
                mst_graph->edge[j].src = i;
                mst_graph->edge[j].dest = aux->successor;
                mst_graph->edge[j].weight = aux->weight;
                j++;
                aux = aux->next;
            }
        }
    }

    return mst_graph;
}



                    ///  MINIMUM SPANNING TREE FUNCTIONS  ///

int find(struct subset subsets[], int i){
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

void Union(struct subset subsets[], int x, int y){
    if (subsets[x].rank < subsets[y].rank)
        subsets[x].parent = y;
    else if (subsets[x].rank > subsets[y].rank)
        subsets[y].parent = x;
    else{
        subsets[y].parent = x;
        subsets[x].rank++;
    }
}

int myComp(const void* a, const void* b){
    struct Edge* a1 = (struct Edge*)a;
    struct Edge* b1 = (struct Edge*)b;
    return a1->weight > b1->weight;
}

void quickSort(Edge *edg, int left, int right) {
      int i = left, j = right;
      Edge tmp;
      float pivot = edg[(left + right) / 2].weight;

      while (i <= j) {
            while (edg[i].weight < pivot)
                  i++;
            while (edg[j].weight > pivot)
                  j--;
            if (i <= j) {
                  tmp = edg[i];
                  edg[i] = edg[j];
                  edg[j] = tmp;
                  i++;
                  j--;
            }
      }
      if (left < j)
            quickSort(edg, left, j);
      if (i < right)
            quickSort(edg, i, right);
}

void KruskalMST(struct Graph* mst_graph, int big){
    //cout << "Running Kruskal" << endl;

    int V = mst_graph->V;
    struct Edge* result = (Edge*) malloc((V-1) * sizeof(Edge));

    quickSort(mst_graph->edge, 0, mst_graph->E - 1);

    struct subset *subsets = (struct subset*) malloc( big * sizeof(struct subset) );

    for (int v = 0; v <  big ; v++){
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    int e = 0;
    int i = 0;
    while (e < (mst_graph->V)-1 && i < mst_graph->E){

        struct Edge next_edge = mst_graph->edge[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        if (x != y)
        {
            result[e] = next_edge;
            e++;
            Union(subsets, x, y);
        }
    }

    //cout << "Generating output file (mst)" << endl;
    ofstream myfile;
    myfile.open ("mst.txt");
    for (i = 0; i < e; ++i){
        myfile << result[i].src <<" "<< result[i].dest << " "<< result[i].weight << endl;
        myfile << result[i].dest <<" "<< result[i].src << " "<< result[i].weight << endl;
    }
    myfile.close();

    free(subsets);
    free(result);

}



                    ///  CONNECTED COMPONENT FUNCTIONS  ///

/* depth-first search */
void dfsRcc(Node *g, int *cc, int i, int comp){
   cc[i] = comp;
   for (Adj * aux = g[i].adj ; aux != NULL; aux = aux->next)
      if (cc[aux->successor] == 0)
         dfsRcc(g, cc, aux->successor, comp);
}

/* count and return connected components number ; remove other components but the biggest one*/
int UGRAPHcc(Node *g, int big){
    //cout << "Defining Connected Components" << endl;
    int *cc = (int*) malloc( big * sizeof(int));
    for (int i = 0; i < big; i++)
        cc[i] = -1;

    string line;
    ifstream myfile ("mst.txt");
    int a, b;
    float w;
    if (myfile.is_open()){
      while ( myfile >> a >> b >> w){
        cc[a] = 0;
        cc[b] = 0;
      }
      myfile.close();
    }
    else cout << "Unable to open file";

    int comp = 0;
    for (int i = 0; i < big; i++){
       if (cc[i] == 0){ //if node i exists
            comp++;
            dfsRcc(g, cc, i, comp);
       }
    }
    //cout << "connected comp total: " << comp << endl;

            // set the biggest component
   int v[comp + 1], m = 0, index = -1;
   for(int j = 0; j < comp + 1; j++) v[j] = 0;
   for(int i = 0; i < big ; i++) if(cc[i] > 0) v[cc[i]]++;
   for(int j = 1; j < comp + 1; j++){
    if(v[j] > m){
        m = v[j];
        index = j;
    }
   }

    for(int i = 0; i < big; i++){
        if(cc[i] != index){
            g[i].value = 0;
            g[i].adj = NULL;
        }
    }

    free(cc);
    return m;
}



                    ///  PARTITIONING FUNCTIONS  ///

/* dfs for counting and return traveled nodes */
int dfs_count(Node *g, int sub_tree, int *v){
    v[sub_tree] = 1;
    int total = 1;
    Adj * aux = g[sub_tree].adj;
    while(aux != NULL){
        if(v[aux->successor] == 0){
            total += dfs_count(g, aux->successor, v);
        }
        aux = aux->next;
    }
    return total;
}

/* search node with high degree */
int search_high_degree(Node *g, int big){
    int index = 0;
    for(int i = 0; i < big; i++){
        if(g[i].value == 1){
            if(g[i].degree > g[index].degree)
                index = i;
        }
    }
    return index;
}

/* initialize vector */
void initializes_list(int *v, int qnt){
    for(int i = 0; i < qnt; i++) v[i] = 0;
}

int get_name(){
    int i = 0;
    while(name[i]!=0){
        i++;
    }
    name[i] = 1;
    return i;
}

/* cut between nodes 'high_degree' and it's 'sub_tree' */
/* invoke partitioning function again, if ite > 1 */
/* flag = 1 (cut 1 sub-tree) ; flag = 2 (cut more than 1 sub-tree) */
void cut(Node *g, int high_degree_index, int * vet, int big, int upper_lim, int low_lim, const char * arq, int flag, int ite, int numPart){

            // auxiliary vector
    int *aux = (int*) malloc( (big) * sizeof(int) );
    initializes_list(aux, big);

            // mark the nodes to be divided starting with node with 'high_degree_index'
    aux[high_degree_index] = 1;
    if( flag == 1){
        dfs_count(g, vet[0], aux);
    }else{
        for(int i = 0 ; vet[i] != 0 ; i++){
            dfs_count(g, vet[i], aux);
        }
    }
    aux[high_degree_index] = 0;
    free(vet);

            // dividing in 'ite' partitions 2^ite
    if(ite <= numPart){
        Node *graph1 =(Node*) malloc(sizeof(Node)*(big));
        Node *graph2 =(Node*) malloc(sizeof(Node)*(big));

                // initialize new graphs
        for(int i = 0; i < big; i++){
            graph1[i].value = 0;
            graph1[i].adj = NULL;
            graph1[i].degree = 0;

            graph2[i].value = 0;
            graph2[i].adj = NULL;
            graph2[i].degree = 0;
        }

                // populate graphs
        if(flag == 1){
            for(int i = 0; i < big; i++){
                Adj * a = g[i].adj;
                while(a != NULL){
                    if(aux[i] == 1 && aux[a->successor] == 1){
                        graph1[i].value = 1;
                        graph1[a->successor].value = 1;
                        insert_edge(graph1,i,a->successor,1,2);
                    }else if(aux[i] == 0 && aux[a->successor] == 0){
                        graph2[i].value = 1;
                        graph2[a->successor].value = 1;
                        insert_edge(graph2,i,a->successor,1,2);
                    }
                    a = a->next;
                }
            }
        }else{
            for(int i = 0; i < big; i++){
                Adj * a = g[i].adj;
                while(a != NULL){
                    if((aux[i] == 1 && aux[a->successor] == 1) || (i == high_degree_index && aux[a->successor] == 1) || (aux[i] == 1 && a->successor == high_degree_index) ){
                        graph1[i].value = 1;
                        graph1[a->successor].value = 1;
                        insert_edge(graph1,i,a->successor,1,2);
                    }else if((aux[i] == 0 && aux[a->successor] == 0) || (i == high_degree_index && aux[a->successor] == 0) || (aux[i] == 0 && a->successor == high_degree_index) ){
                        graph2[i].value = 1;
                        graph2[a->successor].value = 1;
                        insert_edge(graph2,i,a->successor,1,2);
                    }
                    a = a->next;
                }
            }
        }

        free_graph(g,big);
        free(g);
        free(aux);
        /// parallelism

                // set new boundaries and invoke 'partitioning' again
        #pragma omp task
        {
            if(ite == numPart){
                    // generate file name
                int name = get_name();
                char buffer1[320];
                snprintf(buffer1, sizeof(char) * 320, "/home/tales/.config/Neo4j Desktop/Application/neo4jDatabases/database-e440885a-6ff3-4faf-a128-ae3606036f1b/installation-3.5.14/import/file%i.csv", name);
                char buffer2[20];
                snprintf(buffer2, sizeof(char) * 20, "file%i.csv", name);

                ifstream myfile (arq);
                ofstream myfile1 (buffer1);
                ofstream myfile2 (buffer2);
                int a, b;
                float w;
                if (myfile.is_open())
                {
                  myfile1 << "author_id1" << "," << "author_id2" << "," << "count";
                  myfile2 << "author_id1" << "," << "author_id2" << "," << "count";
                  while ( myfile >> a >> b >> w)
                  {
                      if ( graph1[a].value == 1 && graph1[b].value == 1){
                        myfile1 << "\n" << a << "," << b << "," << w;
                        myfile2 << "\n" << a << "," << b << "," << w;
                    }
                  }
                  myfile.close();
                  myfile1.close();
                  myfile2.close();
                }
            }

            int k = 0;
            for(int i = 0; i < big; i++){
                if(graph1[i].value == 1) k++;
            }
            upper_lim = (k/2.0) + (k/20.0);
            low_lim = (k/2.0) - (k/20.0);

            partitioning(graph1,big,upper_lim,low_lim,arq,ite,numPart);
        }

        #pragma omp task
        {
            if(ite == numPart){
                    // generate file name
                int name = get_name();
                char buffer3[320];
                snprintf(buffer3, sizeof(char) * 320, "/home/tales/.config/Neo4j Desktop/Application/neo4jDatabases/database-e440885a-6ff3-4faf-a128-ae3606036f1b/installation-3.5.14/import/file%i.csv", name);
                char buffer4[20];
                snprintf(buffer4, sizeof(char) * 20, "file%i.csv", name);

                ifstream myfile (arq);
                ofstream myfile3 (buffer3);
                ofstream myfile4 (buffer4);
                int a, b;
                float w;
                if (myfile.is_open())
                {
                  myfile3 << "author_id1" << "," << "author_id2" << "," << "count";
                  myfile4 << "author_id1" << "," << "author_id2" << "," << "count";
                  while ( myfile >> a >> b >> w)
                  {
                      if ( graph2[a].value == 1 && graph2[b].value == 1){
                        myfile3 << "\n" << a << "," << b << "," << w ;
                        myfile4 << "\n" << a << "," << b << "," << w ;
                    }
                  }
                  myfile.close();
                  myfile3.close();
                  myfile4.close();
                }
            }

            int k = 0;
            for(int i = 0; i < big; i ++){
                if(graph2[i].value == 1) k++;
            }

            upper_lim = (k/2.0) + (k/20.0);
            low_lim = (k/2.0) - (k/20.0);
            partitioning(graph2,big,upper_lim,low_lim,arq,ite,numPart);
        }
    }
}

/* MAIN PARTITIONING FUNCTION */
void partitioning(Node *g, int big, int upper_lim, int low_lim, const char * arq, int ite, int numPart){

    //printf("\nParticionando");
    if(ite < numPart){

        int done = 0, sum[2];
        initializes_list(sum, 2);
                //search high degree node ( i = high_degree_index )
        int i = search_high_degree(g, big);
        int high_degree = g[i].degree;

                // 2 scheduling vectors
        int index1 = 0, index2 = 0;
        int *vet1 = (int*) malloc( high_degree * sizeof(int) );
        initializes_list(vet1, high_degree);
        int *vet2 = (int*) malloc( high_degree * sizeof(int) );
        initializes_list(vet2, high_degree);
                // vector to set if node was visited
        int *vet = (int*) malloc( (big) * sizeof(int) );
        initializes_list(vet, big);

                // take first sub-tree (index)
        int sub_tree = g[i].adj->successor;

                // while not partitioned
        while(done == 0){

                    // set node visited
            vet[i] = 1;
                    // return number of nodes (sub-tree) and mark nodes as visited
            int cont = dfs_count(g, sub_tree, vet);

            if(cont >= low_lim && cont <= upper_lim){ // sub-tree within the boundaries
                vet1[0] = sub_tree;
                        // cut sub-tree ( i = high_degree_index )
                done = 1;
                free(vet);
                cut(g, i, vet1, big, upper_lim, low_lim, arq, 1, ite + ite, numPart); ///* CUTTING *
                free(vet2);
            }else if(cont > upper_lim){ // found bigger sub-tree (go down)
                        // initializes all
                index1 = 0;
                index2 = 0;
                initializes_list(vet1, high_degree);
                initializes_list(vet2, high_degree);
                initializes_list(vet, big);
                initializes_list(sum, 2);
                i = sub_tree;
                sub_tree = g[sub_tree].adj->successor;
            }else if(sum[0] + cont  >= low_lim && sum[0] + cont <= upper_lim){ // scheduling within the boundaries : cutting
                vet1[index1++] = sub_tree;
                done = 1;
                free(vet);
                cut(g, i, vet1, big, upper_lim, low_lim, arq, 2, ite + ite, numPart); ///* CUTTING *
                free(vet2);
            }else if(sum[1] + cont >= low_lim && sum[1] + cont <= upper_lim){ // scheduling within the boundaries : cutting
                vet2[index2++] = sub_tree;
                done = 1;
                free(vet);
                cut(g, i, vet2, big, upper_lim, low_lim, arq, 2, ite + ite, numPart); ///* CUTTING *
                free(vet1);
            }else if(sum[0] + cont < low_lim){ // sub-tree under the low_lim : scheduling
                sum[0] += cont ;
                vet1[index1++] = sub_tree;
                Adj * aux = g[i].adj;
                while( aux->successor != sub_tree ){
                    aux = aux->next;
                }
                if(aux->next != NULL){
                    sub_tree = aux->next->successor;
                }
            }else if(sum[1] + cont < low_lim){ // sub-tree under the low_lim : scheduling
                sum[1] += cont;
                vet2[index2++] = sub_tree;
                Adj * aux = g[i].adj;
                while( aux->successor != sub_tree ){
                    aux = aux->next;
                }
                if(aux->next != NULL){
                    sub_tree = aux->next->successor;
                }
            }else {
                if(sum[0] >= sum[1]){ // last option
                    vet2[index2++] = sub_tree;
                    done = 1;
                    free(vet);
                    cut(g, i, vet2, big, upper_lim, low_lim, arq, 2, ite + ite, numPart); ///* CUTTING *
                    free(vet1);
                }else{
                    vet1[index1++] = sub_tree;
                    done = 1;
                    free(vet);
                    cut(g, i, vet1, big, upper_lim, low_lim, arq, 2, ite + ite, numPart); ///* CUTTING *
                    free(vet2);
                }
            }
        }
    }
    else{
        free_graph(g,big);
        free(g);
    }
}

//

