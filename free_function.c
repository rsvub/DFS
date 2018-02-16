#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

/*
 * Uvoln�n� pomocn� cesty z pam�ti
 * @param tmp_path: pomocn� cesta
 */
void free_temp_path(temp_path *tmp_path) {
	free(tmp_path -> visited);
	free(tmp_path -> vertexes);
	free(tmp_path -> edges);
	free(tmp_path);
}

/*
 * Uvoln�n� cesty z pam�ti
 * @param p: cesta
 */
void free_path(path *p) {
	free(p -> vertexes);
	free(p -> edges);
	free(p);
}

/*
 * Uvoln�n� v�ech cest z pam�ti
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
 * Uvoln�n� bodu z pam�ti
 * @param p: bod
 */
void free_vertex(vertex *vt) {
	free(vt -> neighbours);
	free(vt -> edges);
	free(vt -> neighboursIndex);
	free(vt);
}

/*
 * Uvoln�n� hrany z pam�ti
 * @param p: hrana
 */
void free_edge(edge *e) {
	free(e -> value);
	free(e);
}

/*
 * Uvoln�n� grafu z pam�ti
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
