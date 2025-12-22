[This README is also available in French!](https://github.com/cyjulien/CY-Preinj2-ProjetS1/blob/main/README_FR.md)

# C-Wildwater Project

Wildwater is a PréIng-2 project consisting of creating a program to synthesize data from a water distribution system.

[documentation](https://cours.cyu.fr/pluginfile.php/22793/mod_folder/content/0/Projet_C-WildWater_preIng2_2025_2026_v1.1.pdf?forcedownload=1)


## What you can do with this program

Using your file containing data from your distribution network, you will be able to:

- Obtain clear histograms on the properties of the plants in your network: their maximum capacity, the total volume collected by the sources, and the total volume treated by the plants.

- Accurately calculate water losses in the downstream network of one of your plants.

With this, we can tell you that you will be able to study your network in order to modify it wisely! 

## Instruction guide

1. Clone the directory
```
git clone https://github.com/cyjulien/CY-Preinj2-ProjetS1.git
```

2. Change directory:
```
cd CY-Preinj2-ProjetS1
```

3. Enjoy running your commands!

# Documentation of possible commands:

Once you have obtained the path to your data file, enter these commands:

Obtain histograms of the top 10 and bottom 50 plants sorted by command:
```

make DATA=file_path.dat TYPE=histo ARG=max # Sort by maximum capacity of each plant

make DATA=file_path.dat TYPE=histo ARG=src # Sort by total volume captured by sources

make DATA=file_path.dat TYPE=histo ARG=src # Sort by total volume actually treated

```

-> Expected output: Two .png files automatically saved in the ./Histo directory.

Obtain all water losses in the downstream network of one of your plants:

```

make DATA=file_path.dat TYPE=leaks ARG=plant_id

```

-> Expected output: a new line in DATA/leaks_log.dat

Clean up the files:
```
make clean
```
