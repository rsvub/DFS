#include <stdio.h>
#include <string.h>
#include "structures.h"
#include <stdlib.h>

#define SIZEOFSTRING 10
#define SIZEOFLINE 100

/*
 * Vytvoreni noveho grafu a naplneni pozadovanyma hodnotama
 * @param g			 : vytvareny graf
 * @param vertexes	 : pole vertexu v grafu
 * @param edges		 : pole hran v grafu
 * @param sizeOfArray: pocet bodu v poli vertexu
 * @param countLines : pocet hran v poli hran
 */
void create_graph(graph **g, vertex **vertexes, edge **edges, int sizeOfArray, int countLines) {
	(*g) = calloc(1, sizeof(graph));
	(*g) -> vertexes = vertexes;
	(*g) -> edges = edges;
	(*g) -> sizeOfArray = sizeOfArray;	
	(*g) -> countLines = countLines;
}

/*
 * Prida pozadovane hodnoty do poli zadaneho vertexu
 * @param vt			: pozadovany vertex
 * @param indexNeighbour: index souseda v poli vertexu
 * @param indexInArray	: index pozice souseda v poli vertexu
 * @param indexEdge		: index hrany v poli hran
 */
void add_into_arrays(vertex **vt, int indexNeighbour, int indexInArray, int indexEdge) {	
	(*vt) -> countNeighbours++;	
	(*vt) -> neighbours = realloc((*vt) -> neighbours, sizeof(int*) * (*vt) -> countNeighbours);
	(*vt) -> neighbours[(*vt) -> countNeighbours - 1] = indexNeighbour;
	
	(*vt) -> neighboursIndex = realloc((*vt) -> neighboursIndex, sizeof(int*) * (*vt) -> countNeighbours);
	(*vt) -> neighboursIndex[(*vt) -> countNeighbours - 1] = indexInArray;
				
	(*vt) -> countEdges++;
	(*vt) -> edges = realloc((*vt) -> edges, sizeof(int*) * (*vt) -> countEdges);
	(*vt) -> edges[(*vt) -> countEdges - 1] = indexEdge;
}

/*
 * Vytvoreni poli pro pozadovany vertex a naplneni prvnima hodnotama
 * @param vt			: pozadovany vertex
 * @param indexNeighbour: prvni index souseda v poli vertexu
 * @param indexInArray	: index souseda v grafovem poli vertexu
 * @param indexEdge		: prvni index hrany v poli hran
 */
void create_arrays(vertex **vt, int indexNeighbour, int indexInArray, int indexEdge) {
	
	(*vt) -> countNeighbours = 1;
	(*vt) -> countEdges = 1;
				
	(*vt) -> neighbours = calloc(1, sizeof(int*));
	(*vt) -> neighbours[0] = indexNeighbour;
	
	(*vt) -> neighboursIndex = calloc(1, sizeof(int*));
	(*vt) -> neighboursIndex[0] = indexInArray;
				
	(*vt) -> edges = calloc(1, sizeof(int*));
	(*vt) -> edges[0] = indexEdge;	
}

/*
 * String prevedeme na strukturu date, ktera je cenou hrany
 * @param dt   : cena hrany
 * @param value: string s cenou hrany
 */
void create_date(date **dt, char *value) {
	
	char *tok = strtok(value, "-");
	short year = atoi(tok);
	
	tok = strtok(NULL, "-");
	short month = atoi(tok);
		
	tok = strtok(NULL, "-");
	/* Obèas naète znak navíc, tøeba ho vyfiltrovat */
	char *tmpDay = calloc(1, 2);
	strncpy(tmpDay, tok, 2);
	short day = atoi(tmpDay);	

	free(tmpDay);
		
	(*dt) -> year = year;
	(*dt) -> month = month;
	(*dt) -> day = day;
}

/*
 * Vytvoreni struktury cest
 * @param p: struktura poli cest
 */
void create_paths(paths **p) {
	(*p) = (paths*)calloc(1, sizeof(paths));
	(*p) -> pt = (path**)calloc(1, sizeof(path*));
	(*p) -> sizeOfPaths = 0;
	return;
}

/*
 * Prida do pole cest dalsi cestu, kterou jsme prave nasli
 * @param p		: struktura poli cest
 * @param t_path: aktualne nalezena cesta
 */
void add_into_paths(paths **p, temp_path **t_path) {
	(*p) -> sizeOfPaths++;
	(*p) -> pt = realloc((*p) -> pt, sizeof(path*) * (*p) -> sizeOfPaths);
	(*p) -> pt[(*p) -> sizeOfPaths - 1] = calloc(1, sizeof(path));
	(*p) -> pt[(*p) -> sizeOfPaths - 1] -> vertexes = calloc((*t_path) -> sizeOfArray, sizeof(int*));
	(*p) -> pt[(*p) -> sizeOfPaths - 1] -> sizeOfPath = (*t_path) -> sizeOfArray;
	(*p) -> pt[(*p) -> sizeOfPaths - 1] -> edges = calloc((*t_path) -> sizeOfArray - 1, sizeof(int*));
	(*p) -> pt[(*p) -> sizeOfPaths - 1] -> metric = 0;
	int i;
	for (i = 0; i < (*t_path) -> sizeOfArray; i++) {
		(*p) -> pt[(*p) -> sizeOfPaths - 1] -> vertexes[i] = (*t_path) -> vertexes[i];
		if (i < i < (*t_path) -> sizeOfArray - 1) {
			(*p) -> pt[(*p) -> sizeOfPaths - 1] -> edges[i] = (*t_path) -> edges[i];
		}
	}
}
