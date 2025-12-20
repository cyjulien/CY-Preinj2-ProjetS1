[This README is also available in English !](TODO.html)

# Projet C-Wildwater

Wildwater est un projet de PréIng-2 consistant à réaliser un programme permettant de faire la synthèse de données d’un système de distribution d’eau.

[documentation](https://cours.cyu.fr/pluginfile.php/22793/mod_folder/content/0/Projet_C-WildWater_preIng2_2025_2026_v1.1.pdf?forcedownload=1)


## Ce que vous pouvez faire grossièrement avec ce programme

À l'aide de votre fichier contenant la data de votre réseau de distribution, vous pourrez :

- Obtenir des histogrammes claires sur les propriétés des usines de votre réseau : leur capacité maximale, le volume totale capté par les sources ainsi que le volume totale traité par les usines.

- Calculer avec précision les pertes d'eau dans le réseau aval de l'une de vos usines.

Avec ça, nous vous le disons, vous pourrez étudier votre réseau afin de le modifier avec sagesse ! 

## Guide d'instruction

### TODO : clone git, MAKEFILE, etc...

1. Cloner le repertoire
```
git clone https://github.com/cyjulien/CY-Preinj2-ProjetS1.git
```

2. Changer de directoire:
```
cd CY-Preinj2-ProjetS1
```

3. Profiiter en executant vos commandes !

# documentation des commandes possibles:

Une fois le chemin de votre fichier contenant la data est obtenue, saisissez ces commandes:

Obtenir les histogrammes des 10 premières et 50 dernières usines trié selon la commande:
```

make DATA=chemin_fichier.dat TYPE=histo ARG=max # Tri par capacité maximale de chaque usine

make DATA=chemin_fichier.dat TYPE=histo ARG=src # Tri par volume totale capté par les sources

make DATA=chemin_fichier.dat TYPE=histo ARG=src # Tri par volume total réellement traité

```

-> Sortie attendu : Deux fichiers .png enregistrées automatiquement dans le repertoire ./Histo

Obtenir l'ensemble des pertes d'eau dans le réseau aval de l'une de vos usines:

```

make DATA=chemin_fichier.dat TYPE=leaks ARG=identifiant_usine

```

-> Sortie attendu : la valeur réel du pourcentage de perte affiché dans le terminal

Nettoyer les fichiers:
```
make clean
```
