#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

/*
 * Uvolnìní pomocné cesty z pamìti
 * @param tmp_path: pomocná cesta
 */
void free_temp_path(temp_path *tmp_path) {
	free(tmp_path -> visited);
	free(tmp_path -> vertexes);
	free(tmp_path -> edges);
	free(tmp_path);
}

/*
 * Uvolnìní cesty z pamìti
 * @param p: cesta
 */
void free_path(path *p) {
	free(p -> vertexes);
	free(p -> edges);
	free(p);
}

/*
 * Uvolnìní všech cest z pamìti
 * @param p: Struktura pole cest
 */
void free_paths(paths *p) {
	int i;
	for (i = 0; i < p -> sizeOfPaths; i++) {
		free_path(p -> pt[i]);
	}
	free(p -> pt);
	free(p);
}

/*
 * Uvolnìní bodu z pamìti
 * @param p: bod
 */
void free_vertex(vertex *vt) {
	free(vt -> neighbours);
	free(vt -> edges);
	free(vt -> neighboursIndex);
	free(vt);
}

/*
 * Uvolnìní hrany z pamìti
 * @param p: hrana
 */
void free_edge(edge *e) {
	free(e -> value);
	free(e);
}

/*
 * Uvolnìní grafu z pamìti
 * @param p: graf
 */
void free_graph(graph *g) {
	int i;
	for (i = 0; i < g -> sizeOfArray; i++) {
		free_vertex(g -> vertexes[i]);
	}
	free(g -> vertexes);
	
	for (i = 0; i < g -> countLines; i++) {
		free_edge(g -> edges[i]);
	}
	free(g -> edges);
	free(g);
}
