#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SPECIES 100
#define MAX_INTERACTIONS 500

typedef struct {
    char id[10];                 // Identifiant unique
    char name[50];               // Nom
    char type[50];               // Type (Producteur, Herbivore, etc.)
    int trophicLevel;            // Niveau trophique
    int population;              // Population initiale
    double growthRate;           // Rythme de croissance
    int carryingCapacity;        // Capacité de portage
} Species;

typedef struct {
    int source;                  // Indice de l'espèce source
    int target;                  // Indice de l'espèce cible
    double weight;               // Poids de l'interaction
} Interaction;

typedef struct {
    Species species[MAX_SPECIES];
    int adjacencyMatrix[MAX_SPECIES][MAX_SPECIES];
    int numSpecies;
    Interaction interactions[MAX_INTERACTIONS];
    int numInteractions;
} TrophicNetwork;

int main() {
    TrophicNetwork network = {0};
    char speciesFile[100], interactionsFile[100];

    // Chargement des fichiers d'espèces et d'interactions
    printf("Entrez le nom du fichier du reseaux : ");
    scanf("%s", speciesFile);
    system("cls");
    if (loadSpecies(&network, speciesFile) == -1) {
        printf("Erreur lors du chargement du fichier des espèces.\n");
        return -1;
    }

    if (loadInteractions(&network, speciesFile) == -1) {
        printf("Erreur lors du chargement du fichier des interactions.\n");
        return -1;
    }

    // Affichage du menu principal
    menu(&network);

    return 0;
}
