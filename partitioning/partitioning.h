#ifndef PARTITIONING_H_INCLUDED
#define PARTITIONING_H_INCLUDED


/* struct for graph partitioning */

struct Adj{
    int successor;
    float weight;
    struct Adj *next;
};

struct Node{
    int value;
    int degree;
    Adj *adj;
};

/* struct for mst manipulate */

struct Edge{
    int src, dest;
    float weight;
};

struct Graph{
    int V, E;
    struct Edge* edge;
};

struct subset{
    int parent;
    int rank;
};

/// /// /// /// /// /// /// /// /// /// /// ///


        ///  GRAPH FUNCTIONS  ////
void free_graph(Node *graph, int big);
int find_biggest_value(const char *arq);
void create_graph(const char * arq, Node * graph, int big, int flag);
void insert_edge(Node * graph, int a, int b, float w, int flag);
struct Graph* migrate_structure(Node * graph, int big);
struct Graph* createMSTGraph(int V, int E);

        ///  MINIMUM SPANNING TREE FUNCTIONS  ///
void Union(struct subset subsets[], int x, int y);
int myComp(const void* a, const void* b);
void quickSort(Edge *edg, int left, int right);
void KruskalMST(struct Graph* mst_graph, int big);

        ///  CONNECTED COMPONENT FUNCTIONS  ///
int UGRAPHcc(Node *g, int big);
void dfsRcc(Node *g, int *cc, int i, int comp);

        ///  PARTITIONING FUNCTIONS  ///
void initializes_list(int *v, int qnt);
int dfs_count(Node *g, int sub_tree, int *v);
int search_high_degree(Node *g, int big);
int get_name();
void cut(Node *g, int high_degree, int *vet, int big, const char * arq, int flag, int numPart);
void partitioning(Node *g, int big, int upper_lim, int low_lim, const char * arq, int ite, int numPart);



#endif // PARTITIONING_H_INCLUDED
