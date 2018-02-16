/* TYPEDEF */

typedef struct the_edge edge;
typedef struct the_date date;
typedef struct the_vertex vertex;
typedef struct the_graph graph;
typedef struct the_temp_path temp_path;
typedef struct the_path path;
typedef struct the_paths paths;

/* END OF TYPEDEF */


/* STRUCTURES */

/* Struktura hrany, obsahuje pouze cenu hrany ulozenou ve strukture date */
struct the_edge {
	date *value;
};

/* Struktura ceny hrany, obsahuje "datum", rok-mesic-den */
struct the_date {
	short year;
	short month;
	short day;
};

/* Struktura bodu grafu, obsahuje svùj index, indexy svých sousedù, pozice svých sousedù v poli vertexù v grafu,
 dále indexy hran v poli hran v grafu a pocet sousedù a hran*/
struct the_vertex {
	int index;
	int *neighbours;
	int *neighboursIndex;
	int *edges;
	short countNeighbours;
	short countEdges;
};

/* Struktura grafu, obsahuje pole hran a bodù a jejich pocet  */
struct the_graph {
	vertex **vertexes;
	edge **edges;
	int sizeOfArray;
	int countLines;	
};

/* Struktura pomocné cesty, obsahuje pocet bodu v poli, pole bodu pres ktere cesta prochazi, pole hran 
a pole visited, ktere nam rika, jake body jsme zatim navstivili v grafu */
struct the_temp_path {
	short sizeOfArray;
	int *vertexes;
	int *edges;
	int *visited;
};

/* Struktura cesty, oproti pomocné cestì obsahuje pouze pole hran a pole bodù, metriku cesty a velikost pole */
struct the_path {
	int sizeOfPath;
	int *vertexes;
	int *edges;
	short metric;
};

/* Struktura pole cest, obsahuje pouze pole cest a jejich poèet */
struct the_paths {
	path **pt;
	int sizeOfPaths;
};

/* END OF STRUCTURES */


/* FUNCTIONS */

int load_data(graph **g, char *filename);
void write_data(graph *g, paths *p);
void print_data(graph *g, paths *p);
void find_paths(graph *g, paths **p, int source, int destination, int maxDistance);
void dfs_search(graph *g, paths **p, temp_path **t_path, int actualVertex, int indexOfActualVertex, int destination, int maxDistance);
short calculate_metric(graph *g, path *pt);
void create_graph(graph **g, vertex **vertexes, edge **edges, int sizeOfArray, int countLines);
void add_into_arrays(vertex **vt, int indexNeighbour, int indexInArray, int indexEdge);
void create_arrays(vertex **vt, int indexNeighbour, int indexInArray, int indexEdge);
void create_date(date **dt, char *value);
void create_paths(paths **p);
void add_into_paths(paths **p, temp_path **t_path);
void print_info();

/* END OF FUNCTIONS*/


/* COMPARE FUNCTIONS */

int cmp_func_paths(const void *a, const void *b);
int cmp_func_dates(const void *a, const void *b);

/* END OF COMPARE FUNCTIONS*/


/* FREE MEMORY FUNCTIONS */

void free_temp_path(temp_path *tmp_path);
void free_path(path *p);
void free_paths(paths *p);
void free_vertex(vertex *vt);
void free_edge(edge *e);
void free_graph(graph *g);
void free_argv(int argc, char *argv[]);

/* END OF FREE MEMORY FUNCTIONS */
