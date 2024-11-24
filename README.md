# Trophic-Networks
Simulation dynamique et analyse de réseaux trophiques, intégrant des algorithmes de centralité, des modèles de croissance logistique, et des simulations d'impact de la disparition d'espèces sur l'écosystème.

Ce projet est une application interactive pour simuler et analyser des réseaux trophiques, permettant de comprendre les dynamiques écologiques et les interactions entre différentes espèces au sein d’un écosystème. Conçu en langage C, ce programme combine algorithmes de graphes et modèles de simulation biologique.

## Fonctionnalités principales

1. **Analyse de la structure du réseau**  
   - Calcul des degrés entrants et sortants pour évaluer la centralité radiale.  
   - Centralité d’intermédiarité pour identifier les espèces critiques dans le réseau trophique.  

2. **Simulation dynamique des populations**  
   - Modèle logistique de croissance intégrant les capacités de portage et les effets des prédations.  
   - Évolution des populations au fil du temps en fonction des interactions trophiques.  
   - Modélisation des rétroactions entre consommateurs et consommés.  

3. **Étude des impacts écologiques**  
   - Simulation de la disparition ou de la raréfaction d’une espèce pour évaluer l’impact global sur l’écosystème.  
   - Calcul des impacts avec ou sans pondération des interactions.  

4. **Personnalisation et sauvegarde**  
   - Ajustement des paramètres des espèces (population initiale, rythme de croissance, etc.).  
   - Sauvegarde et restauration des configurations pour une exploration approfondie.  

## Objectifs pédagogiques

Ce projet vise à sensibiliser à la complexité des écosystèmes et à l’importance des interactions entre espèces. Il permet d’expérimenter avec des modèles mathématiques simples pour observer des phénomènes tels que la régulation écologique et les oscillations des populations.

## Exemples d'application

- Étude de l’impact de la disparition d’un super-prédateur comme le lion dans un écosystème de savane.  
- Analyse des oscillations populationnelles dans un réseau marin, incluant phytoplancton et prédateurs.  
- Expérimentations pour découvrir des dynamiques écologiques stables ou chaotiques.

Ce projet est idéal pour les étudiants et chercheurs en écologie, les passionnés de modélisation, et les curieux désireux d’explorer les interactions dans la nature.

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











