#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fonction.h"

int main(){
    graphe_t graphe;
    float temps;
    clock_t t1, t2;
    char *depart, *arriver, *c;
    char * unused __attribute__((unused));

    depart = (char*)malloc(MAX_STATION_CHAR * sizeof(char));
    arriver = (char*)malloc(MAX_STATION_CHAR * sizeof(char));

    printf("Les noms des stations ont été tirés du site Wikipédia.\n");
    printf("Entrer le nom de la station de départ : ");
    unused = fgets(depart, MAX_STATION_CHAR, stdin);
    if ((c = strrchr(depart, '\n')) != NULL) {
        *c = '\0';
    }

    printf("Entrer le nom de la station d'arrivée : ");
    unused = fgets (arriver, MAX_STATION_CHAR, stdin);
    if ((c = strrchr(arriver, '\n')) != NULL) {
        *c = '\0';
    }

    if(strcmp(arriver, depart) == 0){
        printf("\nLa station de départ et la station d'arriver ne doivent pas être la même.\n");
        exit(0);
    }
    t1 = clock();
    graphe = initGraphe("metro.txt", NBR_STATION);
    dijkstra(graphe, depart,arriver);
    t2 = clock();
    
    temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    printf("temps de calcul = %f\n", temps);
    freeGraphe(graphe);
    free(depart);
    free(arriver);

    return 0;
}
