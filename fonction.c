#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "fonction.h"

int myRand(int min, int max){
    return rand()%(max-min)+min;
}

void freeGraphe(graphe_t graphe){
    int i;

    if(!graphe){
        return;
    }
    for(i=0;i<graphe->nbrSucc;i++){    
        freeGraphe(graphe->vecSucc[i]);
    }
    free(graphe);
}

void afficheGraphe(graphe_t graphe){
    int i;

    if(!graphe){
        return;
    }
    printf("%s poid %d\n", graphe->station, graphe->poid);
    for(i=0;i<graphe->nbrSucc;i++){    
        afficheGraphe(graphe->vecSucc[i]);
    }
}

int checkIfExist(char * stationName, tabremonte_t * nodeVisited, int sizeTab ){
    int i, resultat;

    resultat = -1;

    for(i=0; i< sizeTab; i++){ 
        if(strcmp(nodeVisited[i]->station, "NONE")== 0){
            continue;
        }
        if((strcmp(stationName, nodeVisited[i]->station)) == 0){
            resultat = 0;
        }
    }
    return resultat;
}

graphe_t getMinWeight(graphe_t graphe, tabremonte_t * nodeVisited, int * distMin){
    int i, minWeight, indice;
    graphe_t ChildNode;

    minWeight = 99999;
    indice = -1;
    for(i = 0;i < graphe->nbrSucc; i++){
        ChildNode = graphe->vecSucc[i];
        if(checkIfExist(ChildNode->station, nodeVisited, graphe->nbrSucc) < 0){
            if(distMin[i] < minWeight){
                minWeight = distMin[i];
                indice = i;
            }
        }
    }
    return graphe->vecSucc[indice];
}

graphe_t getStationByName(char * stationName, graphe_t graphe){
    int i;
    graphe_t result;

    result = NULL;

    for(i=0; i < graphe->nbrSucc; i++){
        if(!graphe->vecSucc[i]){
            continue;
        }
        if(strcmp(graphe->vecSucc[i]->station, stationName) == 0){
            result = graphe->vecSucc[i];
        }
    }

    if(!result){
        printf("%s station not found\n", stationName);
        exit(0);
    }
    return result;
}

graphe_t createGraphe(int nbrFils){
    int i;
    graphe_t graphe;
    
    graphe = (graphe_t)malloc(sizeof(node_t));

    if(nbrFils != 0 ){
        graphe->vecSucc = (graphe_t *)malloc(nbrFils * sizeof(node_t));
        for(i=0; i < nbrFils; i++){
            graphe->vecSucc[i]= NULL;
        }
    }
    else {
        graphe->nbrSucc = 0;
        graphe->vecSucc = NULL;
    }

    return graphe;
}

graphe_t initGraphe(char * file, int nbrStation){
    
    /// Pour fscanf /// 
    FILE* fichier = NULL;
    char scanStation[MAX_STATION_CHAR];
    int scanNbrSucc,i, j;
    char ** scanSucc;
    int unused __attribute__((unused));

    /// Pour graphe ///
    graphe_t graphe;

    fichier = fopen(file, "r");

    if (!nbrStation){
      printf("erreur : Precisez le nombre station\n");
      exit(0);
    }
    if(fichier == NULL) {
        printf("erreur : Verifier le fichier d'entrer\n");
        exit(0);
    }
    
    graphe = createGraphe(nbrStation);
    strcpy(graphe->station, "Graphe principale");
    graphe->nbrSucc = nbrStation;
    for(i=0; i < graphe->nbrSucc; i++){
        
        /// Init param fscanf ///
        scanSucc = (char**)malloc(6*sizeof(char*));
        for(int k = 0; k < 6; k++){
            scanSucc[k] = (char*)malloc(MAX_STATION_CHAR * sizeof(char));
        }

        unused = fscanf(fichier, "%s %d %s %s %s %s %s %s",
                            scanStation, &scanNbrSucc, scanSucc[0],
                            scanSucc[1], scanSucc[2], scanSucc[3],
                            scanSucc[4], scanSucc[5]);

        graphe->vecSucc[i] = createGraphe(scanNbrSucc);
        graphe->vecSucc[i]->indice = i;
        strcpy(graphe->vecSucc[i]->station, scanStation);
        graphe->vecSucc[i]->nbrSucc = scanNbrSucc;
        graphe->vecSucc[i]->poid = myRand(1, 3);
        
        for(j=0 ; j < graphe->vecSucc[i]->nbrSucc; j++){
            graphe->vecSucc[i]->vecSucc[j] = createGraphe(0);
            strcpy(graphe->vecSucc[i]->vecSucc[j]->station, scanSucc[j]);
            graphe->vecSucc[i]->vecSucc[j]->poid = myRand(1, 3);
            graphe->vecSucc[i]->vecSucc[j]->indice = j;
        }
        
        for(int k = 0; k < 6; k++){
            free(scanSucc[k]);
        }
        free(scanSucc);
    }
    fclose(fichier);
    return graphe;
}

tabremonte_t getPath(tabremonte_t * path, char * pathStationName, int sizeOfTab){
    int i;
    tabremonte_t tmp;

    tmp = NULL;
    for(i=0; i < sizeOfTab; i++){
        if(strcmp(path[i]->station, pathStationName )== 0){
            tmp = path[i];
        }
    }
    if(tmp == NULL){
        printf("No path found for %s\n", pathStationName);
        exit(0);
    }
    return tmp;
}

void affihceChemin(tabremonte_t * path, char * source, char * destination, int sizeOfTab){
    tabremonte_t currentNode;
    tabremonte_t * pathOrganised;
    int found, pathNewSize, tmpSize;

    found = 0;
    pathNewSize = 0;
    tmpSize = 1;

    currentNode = getPath(path, destination, sizeOfTab);
    
    while(found == 0){
        currentNode= getPath(path, currentNode->parent, sizeOfTab);
        if(strcmp(currentNode->station, source)==0){
            found = 1;
            tmpSize++;
            break;
        }
        tmpSize++;
    }

    currentNode = getPath(path, destination, sizeOfTab);
    pathNewSize = tmpSize;
    found = 0;

    pathOrganised = (tabremonte_t*)malloc(pathNewSize * sizeof(tabremonte_t));
    pathOrganised[tmpSize-1] = currentNode;
    tmpSize -= 1;
    while(found == 0){
        --tmpSize;
        currentNode= getPath(path, currentNode->parent, sizeOfTab);
        if(strcmp(currentNode->parent, "NONE")==0){
            pathOrganised[tmpSize] = currentNode;
            found = 1;
            break;
        }
        pathOrganised[tmpSize] = currentNode;
    }


    printf("Le plus court chemin est : ");
    for(int i = 0; i < pathNewSize; i++){
        if(strcmp(pathOrganised[i]->station, destination)==  0){
            printf("%s.\n\n", pathOrganised[i]->station);  
            printf("Le trajet durera %d minuttes.\n", pathOrganised[i]->total);  
              
        }
        else{
            printf("%s -> ", pathOrganised[i]->station);    
        }
    }
    printf("\n");

    for(int i = 0; i < pathNewSize; i++){
        free(pathOrganised[i]);    
    }
    free(pathOrganised);

}

char * normalizeStationName(char * stationName){
  char *result = NULL;

  if (stationName != NULL){
    int i;
    result = malloc (sizeof (*result) * (strlen (stationName) + 1));
    if (result != NULL){
      for (i = 0; stationName[i]; i++){
        if(stationName[i] == ' '){
            result[i] = '_';
        }
        else{
            result[i] = stationName[i];
        }
      }
      result[i] = '\0';
    }
  }
  return result;
} 

void dijkstra(graphe_t graphe, char *   src, char * dest){
    graphe_t currentNode, childNode, originalChildNode, arrival;
    tabremonte_t * path,* nodeVisited;
    tabremonte_t tmpNodeVisited;
    char * depart, * destination;

    int * distMin;
    int infinity = 99999;
    int i, distance, pathAdded, finished;
    int pathTabSize = 2 * graphe->nbrSucc;

    depart = normalizeStationName(src);
    destination = normalizeStationName(dest);

    arrival = getStationByName(destination, graphe);
    currentNode = getStationByName(depart, graphe);

    nodeVisited = (tabremonte_t *)malloc(graphe->nbrSucc * sizeof(remonte_t));
    for(i=0; i < graphe->nbrSucc; i++){
        nodeVisited[i] = malloc(sizeof(remonte_t));
        strcpy(nodeVisited[i]->station, "NONE");
        strcpy(nodeVisited[i]->parent, "NONE");
    }
    
    path = (tabremonte_t *)malloc(pathTabSize * sizeof(remonte_t));
    for(i=0; i < pathTabSize; i++){
        path[i] = malloc(sizeof(remonte_t));
        strcpy(path[i]->station, "NONE");
        strcpy(path[i]->parent, "NONE");
    }

    distMin = (int *)malloc(graphe->nbrSucc * sizeof(int));
    for(i=0; i < graphe->nbrSucc; i++){
        distMin[i] = infinity;
    }
    distMin[currentNode->indice] = 0;

    pathAdded = 0 ;
    finished = 0;

    while(finished == 0){
        tmpNodeVisited = malloc(sizeof(remonte_t));
    
        currentNode = getMinWeight(graphe, nodeVisited, distMin);
        
        if(pathAdded <=0){
            strcpy(path[pathAdded]->station,  depart);
            pathAdded++;
        }
        
        strcpy(tmpNodeVisited->station, currentNode->station);
        nodeVisited[currentNode->indice] = tmpNodeVisited;

        for(i=0; i < currentNode->nbrSucc; i++){
            childNode = currentNode->vecSucc[i];
            originalChildNode = getStationByName(childNode->station, graphe);
            distance = distMin[currentNode->indice] + childNode->poid;
            
            if (distance < distMin[originalChildNode->indice]){
                distMin[originalChildNode->indice] = distance;
                strcpy(path[pathAdded]->station,  childNode->station);
                strcpy(path[pathAdded]->parent,  currentNode->station);
                if(strcmp(path[pathAdded]->station, arrival->station) == 0){
                    finished = 1;
                    path[pathAdded]->total = distMin[originalChildNode->indice];
                    break;
                }
                pathAdded++;
            }
        }
    }
    
    affihceChemin(path, depart, destination, pathTabSize);
    
    free(tmpNodeVisited);

    for(i=0; i < graphe->nbrSucc; i++){
        free(nodeVisited[i]);
    }
    free(nodeVisited);
    free(distMin);
}
