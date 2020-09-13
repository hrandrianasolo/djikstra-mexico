#define NBR_STATION 165
#define MAX_CHILD 6
#define MAX_STATION_CHAR 50

typedef struct noeud node_t;
typedef struct noeud * graphe_t;
typedef struct remonte remonte_t;
typedef struct remonte * tabremonte_t;

struct noeud{
    char station[MAX_STATION_CHAR];
    int poid;
    int nbrSucc;
    int indice;
    struct noeud ** vecSucc;
};

struct remonte{
    char parent[MAX_STATION_CHAR];
    char station[MAX_STATION_CHAR];
    int total;
};

int myRand(int,int);
void freeGraphe(graphe_t);
void afficheGraphe(graphe_t);
int checkIfExist(char *, tabremonte_t *, int );
graphe_t getMinWeight( graphe_t, tabremonte_t *, int *);
graphe_t getStationByName(char *, graphe_t);
graphe_t createGraphe(int);
graphe_t initGraphe(char *, int);
void affihceChemin(tabremonte_t *, char *, char *, int);
void dijkstra(graphe_t, char *, char *);
