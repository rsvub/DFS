#include <stdio.h>
#include <string.h>
#include "structures.h"
#include <stdlib.h>
#include <ctype.h>

#define SIZEOFSTRING 10
#define SIZEOFLINE 100

#define ARGV_FILENAME 1
#define ARGV_SOURCE 2
#define ARGV_DESTINATION 3
#define ARGV_MAXDISTANCE 4
#define TYPE_FORMAT_CSV ".csv"

/*
 * Main funkce, nacte hodnoty z konzole a zkontroluje vsechny parametry
 * @param argc: pocet zadanych parametru v konzoli
 * @param argv: pole zadanych parametru
 */
int main(int argc, char *argv[]) {
	
	/* Kontrola zadaných parametrù - source, destination a maxDistance, zda jsou to èísla nebo ne 
	 * Pokud alespoò jedno ze zadaných parametrù není èíslo, nastaví se promìnná na hodnotu 1
	 */
	int x, y, is = 0;
	for (x = 2; x < argc; x++) {
		for (y = 0; y < strlen(argv[x]); y++) {
			if (!isdigit(argv[x][y])) {
				is = 1;
				break;
			}
		}
	}

	/* Kontrola zda poèet zadaných parametrù je správnì, zda je nastavený správný typ souboru,
	 * zda source/destination/maxDistance je vìtší než hodnota nula
	 */
	if (argc == 5 && strstr (argv[ARGV_FILENAME], TYPE_FORMAT_CSV) && atoi(argv[ARGV_SOURCE]) > 0  && 
	    atoi(argv[ARGV_DESTINATION]) > 0 && atoi(argv[ARGV_MAXDISTANCE]) > 0 && is == 0) {
		char *filename = argv[ARGV_FILENAME];	
		int source = atoi(argv[ARGV_SOURCE]);
		int destination = atoi(argv[ARGV_DESTINATION]);
		short maxDistance = atoi(argv[ARGV_MAXDISTANCE]);
	
		graph *g;
		short val = load_data(&g, filename);
		/* Kontrola source a destination, zda vùbec tyto body existují v grafu */
		int i; 
		short isSource = 0, isDestination = 0;
		if (val == 1) {
			for (i = 0; i < g -> sizeOfArray; i++) {
				if (g -> vertexes[i] -> index == source) {
					isSource = 1;
				}
			
				if (g -> vertexes[i] -> index == destination) {
					isDestination = 1;
				}
			}	
		}

		
		if (val == 1 && isSource == 1 && isDestination == 1) {
			//printf("Data successfully loaded\n");
			
			paths *p;
			find_paths(g, &p, source, destination, maxDistance);
			
			int l;
			for (l = 0; l < p -> sizeOfPaths; l++) {
				p -> pt[l] -> metric = calculate_metric(g, p -> pt[l]);
			}
			qsort(p -> pt, p -> sizeOfPaths, sizeof(path*), cmp_func_paths);
			//printf("All paths found\n");
			
			print_data(g, p);
			//write_data(g, p);
			//printf("Data saved to results.txt\nEND\n");
			printf("\n%d", p -> sizeOfPaths);
			free_paths(p);
			free_graph(g);	
			
			return EXIT_SUCCESS;			
		}
		else if (val == 0) {
			printf("Spatne zadane parametry - spatny soubor\n");
			return EXIT_FAILURE;			
		}
		else {
			free_graph(g);
			printf("Spatne zadane parametry - spatne vybrane body\n");
			return EXIT_FAILURE;
		}
	}
	else {
		print_info();
		return EXIT_FAILURE;
	}
}


/*
 * Nacteni dat ze souboru a vytvoreni grafu
 * @param g 	  : graf
 * @param filename: nazev souboru
 */
int load_data(graph **g, char *filename) {
	
	FILE *f;	
	char line[SIZEOFLINE], *tok;	
	int tmpIndex1 = 0, tmpIndex2 = 0, ret = 1, is1 = 0, is2 = 0, countLines = 0, j = 0, i = 0, sizeOfArray = 0, lineIndex = 0;
	
	f = fopen(filename, "r");
	if (f == NULL) {
		ret = 0;
		printf("Soubor neexistuje / nelze otevrit\n");
		return ret;
	}
	else {	
		while (fgets(line, sizeof(line), f) != NULL) {	
			countLines++;
		}
	}
	fclose(f);
	
	vertex **vertexes = (vertex**)calloc(1, sizeof(vertex*));
	edge **edges = (edge**)calloc(countLines, sizeof(edge*));
	
	f = fopen(filename, "r");
	while (fgets(line, sizeof(line), f) != NULL) {		
		edges[lineIndex] = calloc(1, sizeof(edge));
		edges[lineIndex] -> value = calloc(1, sizeof(date));
			
		tok = strtok(line, ";");	
		int index1 = atoi(tok);
		
		tok = strtok(NULL, ";");
		int index2 = atoi(tok);
			
		tok = strtok(NULL, ";");
		char value[SIZEOFSTRING];
		strncpy(value, tok, SIZEOFSTRING);
		create_date(&edges[lineIndex] -> value, value);
			
		int j = 0;
		is1 = 0;
		is2 = 0;
		tmpIndex1 = 0;
		tmpIndex2 = 0;
		/* Kontrola, zda tento index se nachází v poli vertexù nebo ne */
		while (j < sizeOfArray) {
			if (vertexes[j] -> index == index1) {				
				tmpIndex1 = j;
				is1 = 1;
			}
				
			if (vertexes[j] -> index == index2) {
				tmpIndex2 = j;
				is2 = 1;
			}
				
			if (is1 == 1 && is2 == 1) {
				break;
			}
		
			j++;
		}
			
		if (is1 == 0) {
			tmpIndex1 = sizeOfArray;
			sizeOfArray++;
				
			vertexes = (vertex**)realloc(vertexes, sizeOfArray * sizeof(vertex*));
			vertexes[sizeOfArray - 1] = calloc(1, sizeof(vertex));
			vertexes[sizeOfArray - 1] -> index = index1;				
		}
			
		if (is2 == 0) {
			tmpIndex2 = sizeOfArray;
			sizeOfArray++;
				
			vertexes = (vertex**)realloc(vertexes, sizeOfArray * sizeof(vertex*));
			vertexes[sizeOfArray - 1] = calloc(1, sizeof(vertex));
			vertexes[sizeOfArray - 1] -> index = index2;	
		}
		
		/* Uložení a vytvoøení jednotlivých polí pro body až na konci, protože nám není dopøedu známo, jaké indexy budeme pøidávat */
		if (is1 == 1) {
			add_into_arrays(&vertexes[tmpIndex1], vertexes[tmpIndex2] -> index, tmpIndex2, lineIndex);
		}
		else if (is1 == 0) {
			create_arrays(&vertexes[tmpIndex1], index2, tmpIndex2, lineIndex);	
		}
		if (is2 == 1) {
			add_into_arrays(&vertexes[tmpIndex2], vertexes[tmpIndex1] -> index, tmpIndex1, lineIndex);			
		}			
		else if (is2 == 0) {
			create_arrays(&vertexes[tmpIndex2], index1, tmpIndex1, lineIndex);				
		}
		lineIndex++;
		
	}
	fclose(f);			
	create_graph(g, vertexes, edges, sizeOfArray, countLines);
	
	return ret;
}


/*
 * Zapsani vsech nalezenych dat do souboru
 * @param g: graf
 * @param p: pole cest
 */
void write_data(graph *g, paths *p) {
	FILE *f;
	
	f = fopen("results.txt", "w");
	int l, y;
	for (l = 0; l < p -> sizeOfPaths; l++) {		
		
		for (y = 0; y < p -> pt[l] -> sizeOfPath; y++) {
			fprintf(f, "%d", p -> pt[l] -> vertexes[y]);
			if (y < p -> pt[l] -> sizeOfPath - 1) {
				fprintf(f, "-");
			}
		}
		
		fprintf(f, ";");
		
		for (y = 0; y < p -> pt[l] -> sizeOfPath - 1; y++) {
			short year = g -> edges[p -> pt[l] -> edges[y]] -> value -> year;
			short month = g -> edges[p -> pt[l] -> edges[y]] -> value -> month;
			short day = g -> edges[p -> pt[l] -> edges[y]] -> value -> day;
			if (month < 10 && day < 10) fprintf(f, "%hu-0%hu-0%hu", year, month, day);
			else if (month < 10 && day > 10) fprintf(f, "%hu-0%hu-%hu", year, month, day);
			else if (month > 10 && day < 10) fprintf(f, "%hu-%hu-0%hu", year, month, day);
			else fprintf(f, "%hu-%hu-%hu", year, month, day);
			if (y < p -> pt[l] -> sizeOfPath - 2) fprintf(f, ",");
		}
		
		fprintf(f, ";");
		fprintf(f, "%hu", p -> pt[l] -> metric);
		fprintf(f, "\n");
	}
	fclose(f);
}

/*
 * Vypíše všechny nalezené cesty do console
 * @param g: graf
 * @param p: pole nalezených cest
 */
 void print_data(graph *g, paths *p) {
	int l, y;
	for (l = 0; l < p -> sizeOfPaths; l++) {		
		
		for (y = 0; y < p -> pt[l] -> sizeOfPath; y++) {
			printf("%d", p -> pt[l] -> vertexes[y]);
			if (y < p -> pt[l] -> sizeOfPath - 1) {
				printf("-");
			}
		}
		
		printf(";");
		
		for (y = 0; y < p -> pt[l] -> sizeOfPath - 1; y++) {
			short year = g -> edges[p -> pt[l] -> edges[y]] -> value -> year;
			short month = g -> edges[p -> pt[l] -> edges[y]] -> value -> month;
			short day = g -> edges[p -> pt[l] -> edges[y]] -> value -> day;
			if (month < 10 && day < 10) printf("%hu-0%hu-0%hu", year, month, day);
			else if (month < 10 && day >= 10) printf("%hu-0%hu-%hu", year, month, day);
			else if (month >= 10 && day < 10) printf("%hu-%hu-0%hu", year, month, day);
			else printf("%hu-%hu-%hu", year, month, day);
			if (y < p -> pt[l] -> sizeOfPath - 2) printf(",");
		}
		
		printf(";");
		printf("%hu", p -> pt[l] -> metric);
		printf("\n");
	}	
}


/*
 * Pomocna funkce pro pripravu pole cest a pomocne cesty
 * @param g			 : graf
 * @param p			 : pole cest
 * @param source	 : zacatecni bod cesty
 * @param destination: koncovy bod cesty
 * @param maxDistance: maximalni delka cesty
 */
void find_paths(graph *g, paths **p, int source, int destination, int maxDistance) {
	
	create_paths(p);
	temp_path *t_path = calloc(1, sizeof(temp_path));
	t_path -> vertexes = calloc(maxDistance + 1, sizeof(int*));
	t_path -> edges = calloc(maxDistance, sizeof(int*));
	t_path -> visited = calloc(g -> sizeOfArray, sizeof(int));
	t_path -> sizeOfArray = 0;
	
	int i, indexOfSource;
	for (i = 0; i < g -> sizeOfArray; i++) {
		t_path -> visited[i] = 0;
		if (source == g -> vertexes[i] -> index) {
			indexOfSource = i;
		}
	}
	dfs_search(g, p, &t_path, source, indexOfSource, destination, maxDistance);	
	free_temp_path(t_path);
}


/*
 * Zjisteni vsech cest ze zadaneho pocatecniho bodu do konecneho, vyhledavani Depth-first search
 * @param g 				 : graf
 * @param p					 : pole cest
 * @param t_path			 : pomocna cesta pro hledani vsech cest
 * @param actualVertex		 : aktualni index vertexu, ktery pridavame do cesty
 * @param indexOfActualVertex: pozice aktualne pridavaneho vertexu v poli vertexu v grafu
 * @param destination		 : pozadovany cil
 * @param maxDistance		 : maximalni delka cesty
 */
void dfs_search(graph *g, paths **p, temp_path **t_path, int actualVertex, int indexOfActualVertex, int destination, int maxDistance) {
	int j = 0;
	(*t_path) -> sizeOfArray++;	
	(*t_path) -> visited[indexOfActualVertex] = 1;	
	(*t_path) -> vertexes[(*t_path) -> sizeOfArray - 1] = actualVertex;	
	
	if (actualVertex == destination) {
		add_into_paths(p, t_path);
	}
	else {
		for (j = 0; j < g -> vertexes[indexOfActualVertex] -> countNeighbours; j++) {
			if ((*t_path) -> sizeOfArray <= maxDistance && 
				(*t_path) -> visited[g -> vertexes[indexOfActualVertex] -> neighboursIndex[j]] == 0) {
				
				(*t_path) -> edges[(*t_path) -> sizeOfArray - 1] = g -> vertexes[indexOfActualVertex] -> edges[j];
				
				dfs_search(g, p, t_path, g -> vertexes[indexOfActualVertex] -> neighbours[j]
				, g -> vertexes[indexOfActualVertex] -> neighboursIndex[j], destination, maxDistance);
			}
			else if ((*t_path) -> sizeOfArray > maxDistance) {
				break;
			}
		}
	}
	(*t_path) -> sizeOfArray--;
	(*t_path) -> visited[indexOfActualVertex] = 0;
}


/*
 * Vytvoreni metriky pro zadanou cestu
 * @param g : graf
 * @param pt: cesta, pro kterou chceme vypocitat metriku
 * @return vypocitana metrika cesty
 */
short calculate_metric(graph *g, path *pt) {
	int i;
	short metric = 0;
	date **dat = calloc(pt -> sizeOfPath - 1, sizeof(date*));
	
	for (i = 0; i < pt -> sizeOfPath - 1; i++) {
		dat[i] = g -> edges[pt -> edges[i]] -> value;
	}		
	
	if (pt -> sizeOfPath - 1 > 1) {
		qsort(dat, pt -> sizeOfPath-1, sizeof(date*), cmp_func_dates);
		
		date *first = dat[0];
		date *last = dat[pt -> sizeOfPath - 2];
		
		short year = first -> year;
		short month = first -> month;
		short day = first -> day;
		metric -= day; /* pro zjednodušení poèítání metriky, aby se nemuselo dopoèítávat kolik dní zbývá do konce mìsíce */
		while (1) {
			if ((year != last -> year) || (month != last -> month)) {
				if (month == 13) {
					year++;
					month = 1;
				}
				else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
					metric += 31;
					month++;
				}
				else if (month == 4 || month == 6 || month == 9 || month == 11) {
					metric += 30;
					month++;
				}
				else if (month == 2) {
					if (year % 100 == 0) {
						if (year % 400 == 0) {
							metric += 29;
							month++;
						}
						else {
							metric += 28;
							month++;
						}
					}
					else if (year % 4 == 0) {
						metric += 29;
						month++;
					}
					else {
						metric += 28;
						month++;
					}
				}
			}
			else {
				metric += last -> day;
				break;
			}
		}	
	}
	free(dat);
	return metric;
}

/*
 * Funkce pro vypis info pro uzivatele, pokud zada neplatny vstup
 */
void print_info() {
	printf("\nNavod pro spusteni programu:\n\n");
	printf("Prvni parametr(soubor): nazev_souboru.csv\n");
	printf("Druhy parametr(source): ciselna hodnota v rozsahu minIndex - maxIndex\n");
	printf("Treti parametr(destination): ciselna hodnota v rozsahu minIndex - maxIndex\n");
	printf("Ctvrty parametr(maxDistance): ciselna hodnota, urcuje jake maximalni delky maji byt nalezene cesty (maxDistance > 0)\n");
	printf("Priklad: dfs data.csv 1 2 4\n");	
	return;
}

/*
 * Funkce pro serazeni cest podle jejich velikosti, pote podle metriky
 */
int cmp_func_paths(const void *a, const void *b) {
	path **x = (path**)a;
	path **y = (path**)b;
	
	int value = (*x) -> sizeOfPath - (*y) -> sizeOfPath;
	if (value == 0) {
		value = (*x) -> metric - (*y) -> metric;
	}
	return value;
}

/*
 * Funkce pro serazeni datumu podle roku-mìsíce-dne
 */
int cmp_func_dates(const void *a, const void *b)
{
	date **x = (date**)a;
	date **y = (date**)b;
		
	int value = (*x) -> year - (*y) -> year;
	if (value == 0) {
		value = (*x) -> month - (*y) -> month;
		if (value == 0) {
			value = (*x) -> day - (*y) -> day;
		}
	}
	return value;
}
