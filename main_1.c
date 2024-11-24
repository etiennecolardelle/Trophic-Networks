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

// Fonction pour charger les espèces depuis un fichier nodes.csv
int loadSpecies(TrophicNetwork *network, const char *filename) {
    char filepath[100];
    strcpy(filepath, filename);
    strcat(filepath, "/nodes.csv");

    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier nodes.csv");
        return -1;
    }

    char line[256];
    int index = 0;

    // Lecture de l'entête
    fgets(line, sizeof(line), file);

    // Lecture des lignes
    while (fgets(line, sizeof(line), file)) {
        Species *s = &network->species[index];
        sscanf(line, "%9[^,],%49[^,],%49[^,],%d,%d,%lf,%d",
               s->id,
               s->name,
               s->type,
               &s->trophicLevel,
               &s->population,
               &s->growthRate,
               &s->carryingCapacity);
        index++;
    }

    network->numSpecies = index;
    fclose(file);
    return 0;
}

// Fonction pour charger les interactions depuis un fichier edges.csv
int loadInteractions(TrophicNetwork *network, const char *filename) {
    char filepath[100];
    strcpy(filepath, filename);
    strcat(filepath, "/edges.csv");

    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier edges.csv");
        return -1;
    }

    char line[256];
    int index = 0;

    // Lecture de l'entête
    fgets(line, sizeof(line), file);

    // Lecture des lignes
    while (fgets(line, sizeof(line), file)) {
        int sourceId, targetId;
        double weight;
        sscanf(line, "%d,%d,%lf", &sourceId, &targetId, &weight);

        // Enregistrer l'interaction
        Interaction *interaction = &network->interactions[index];
        interaction->source = sourceId - 1; // Convertir en indice (0-based)
        interaction->target = targetId - 1; // Convertir en indice (0-based)
        interaction->weight = weight;

        // Mettre à jour la matrice d'adjacence
        network->adjacencyMatrix[interaction->source][interaction->target] = 1;

        index++;
    }

    network->numInteractions = index;
    fclose(file);
    return 0;
}



// Recherche des producteurs primaires qui ne consomment pas d'autres espèces
void findPrimaryProducers(TrophicNetwork *network) {
    printf("Producteurs primaires qui ne consomment pas d'autres especes :\n");
    for (int i = 0; i < network->numSpecies; i++) {
        if (strcmp(network->species[i].type, "Producteur") == 0) {
            int isConsumed = 0;
            for (int j = 0; j < network->numSpecies; j++) {
                if (network->adjacencyMatrix[j][i]) {
                    isConsumed = 1;
                    break;
                }
            }
            if (!isConsumed) {
                printf("%s\n", network->species[i].name);
            }
        }
    }
}

// Recherche des derniers maillons de la chaîne alimentaire (espèces sans prédateurs)
void findTopPredators(TrophicNetwork *network) {
    printf("Especes sans predateurs (pas d'arcs sortants) :\n");
    for (int i = 0; i < network->numSpecies; i++) {
        int hasPredator = 0;
        for (int j = 0; j < network->numSpecies; j++) {
            if (network->adjacencyMatrix[i][j]) {
                hasPredator = 1;
                break;
            }
        }
        if (!hasPredator) {
            printf("%s\n", network->species[i].name);
        }
    }
}

// Recherche des espèces ayant une seule source d'alimentation
void findSpeciesWithSingleSource(TrophicNetwork *network) {
    printf("Especes avec une seule source d'alimentation :\n");
    for (int i = 0; i < network->numSpecies; i++) {
        int foodSources = 0;
        for (int j = 0; j < network->numSpecies; j++) {
            if (network->adjacencyMatrix[j][i]) {
                foodSources++;
            }
        }
        if (foodSources == 1) {
            printf("%s\n", network->species[i].name);
        }
    }
}

// Fonction récursive pour collecter les chaînes alimentaires d'une espèce
void collectFoodChains(TrophicNetwork *network, int speciesIndex, bool visited[], char chain[][50], int depth) {
    // Marquer l'espèce comme visitée
    visited[speciesIndex] = true;

    // Ajouter l'espèce actuelle à la chaîne en construction
    strcpy(chain[depth], network->species[speciesIndex].name);
    depth++;

    bool hasPredecessor = false;

    // Parcourir tous les prédécesseurs dans la matrice d'adjacence
    for (int i = 0; i < network->numSpecies; i++) {
        if (network->adjacencyMatrix[i][speciesIndex] && !visited[i]) {
            hasPredecessor = true;
            collectFoodChains(network, i, visited, chain, depth);
        }
    }

    // Si aucune espèce prédécesseur n'a été trouvée, afficher la chaîne complète
    if (!hasPredecessor) {
        for (int i = depth - 1; i >= 0; i--) {
            printf("%s", chain[i]);
            if (i > 0) printf(" <- ");
        }
        printf("\n");
    }

    // Annuler la visite pour permettre d'autres chaînes alimentaires
    visited[speciesIndex] = false;
}

// Fonction principale pour afficher les chaînes alimentaires complètes d'une espèce
void displayCompleteFoodChains(TrophicNetwork *network, int speciesIndex) {
    if (speciesIndex < 0 || speciesIndex >= network->numSpecies) {
        printf("Indice invalide. Veuillez entrer un indice entre 0 et %d.\n", network->numSpecies - 1);
        return;
    }

    printf("Chaines alimentaires completes pour %s :\n", network->species[speciesIndex].name);

    // Initialiser les structures pour suivre les visites et stocker la chaîne
    bool visited[MAX_SPECIES] = {false};
    char chain[MAX_SPECIES][50];

    collectFoodChains(network, speciesIndex, visited, chain, 0);
    printf("\n");
}


// Fonction pour afficher les niveaux trophiques
void displayTrophicLevels(TrophicNetwork *network) {
    printf("Niveaux trophiques des especes :\n");
    for (int i = 0; i < network->numSpecies; i++) {
        printf("%s:  %d\n", network->species[i].name, network->species[i].trophicLevel);
    }
}

// Calcul du niveau trophique maximal dans le réseau
int findMaxTrophicLevel(TrophicNetwork *network) {
    int maxLevel = 0;
    for (int i = 0; i < network->numSpecies; i++) {
        if (network->species[i].trophicLevel > maxLevel) {
            maxLevel = network->species[i].trophicLevel;
        }
    }
    return maxLevel;
}

// Sous-menu pour la recherche de sommets particuliers
void searchMenu(TrophicNetwork *network) {
    int choice;
    while (1) {
        printf("          --- Recherche de Sommets Particuliers ---\n");
        printf("1. Trouver les producteurs primaires qui ne consomment pas d'autres especes\n");
        printf("2. Trouver les derniers maillons des chaines alimentaires (sans predateurs)\n");
        printf("3. Trouver les especes ayant une seule source d'alimentation\n");
        printf("4. Retourner au menu principal\n");
        printf("Choisissez une option : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                system("cls");
                findPrimaryProducers(network);
                break;
            case 2:
                system("cls");
                findTopPredators(network);
                break;
            case 3:
                system("cls");
                findSpeciesWithSingleSource(network);
                break;
            case 4:
                system("cls");
                return; // Retourner au menu principal
            default:
                system("cls");
                printf("Option invalide. Veuillez reessayer.\n");
        }
    }
}

// Menu principal
void menu(TrophicNetwork *network) {
    int choice, speciesIndex;

    while (1) {
        printf("       --- Menu ---\n");
        printf("1. Recherche de Sommets Particuliers\n");
        printf("2. Afficher les chaines alimentaires d'une espece\n");
        printf("3. Afficher les niveaux trophiques des especes\n");
        printf("4. Trouver le niveau trophique maximal\n");
        printf("5. Quitter\n\n");
        printf("Choisissez une option : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                system("cls");
                searchMenu(network);  // Appeler le sous-menu pour la recherche de sommets
                break;
            case 2:
                system("cls");
                printf("Entrez l'indice de l'espece pour afficher ses chaines alimentaires : ");
                scanf("%d", &speciesIndex);
                displayCompleteFoodChains(network, speciesIndex);
                break;
            case 3:
                system("cls");
                displayTrophicLevels(network);
                break;
            case 4:
                system("cls");
                printf("Niveau trophique maximal : %d\n", findMaxTrophicLevel(network));
                break;
            case 5:
                system("cls");
                printf("Quitter le programme.\n");
                return;
            default:
                system("cls");
                printf("Option invalide. Veuillez reessayer.\n");
        }
    }
}

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
