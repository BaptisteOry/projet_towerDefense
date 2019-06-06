# Projet IMAC Tower Defense - Flower Tower

Jeu de tower defense avec pour thème le flower power des années 60-70, dans le cadre de la formation ingénieur IMAC.

*https://github.com/BaptisteOry/projet_towerDefense*

## Prérequis

Le programme fonctionne sur Linux. Assurez-vous d'avoir SDL 1.2, OpenGL 2.0 et la librairie freeglut :

```
sudo apt-get update
sudo apt-get install libsdl1.2-dbg
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

## Démarrer

À la racine du programme (/), compilez avec la commande :

```
make
```

Puis, lancer le programme avec la commande :

```
bin/towerdef.itd
```

## Règles du jeu

### But du jeu

Survivez à toutes les vagues d'émotions négatives en plaçant des fleurs et en les améliorant grâce à des combis.

### Commandes

* H : aide et pause
* Echap : quitter
* A / Z / E / R + clic gauche : construire une fleur
	* A : fleur rouge - puissante
	* Z : fleur violette - équilibrée
	* E : fleur jaune - cadence rapide
	* R : fleur bleue - longue portée
* Q / S / D + clic gauche : construire un bâtiment
	* Q : combi radar - augmente la portée des tours
	* S : combi usine - augmente la puissance des tours
	* D : combi stock - augmente la cadence de tir des tours
* Clic gauche sur construction : consulter les informations
* Clic droit sur construction : détruire une construction

## Version

1.0 *- juin 2019*

## Crédits

* **Baptiste Ory** - (https://github.com/BaptisteOry)
* **Solène Mary-Vallée** - (https://github.com/SolHaine)

*Programme réalisé au cours du deuxième semestre de la formation ingénieur IMAC pour les cours de « Synthèse d'image » et « Programmation et algorithmique ».*

## Bon à savoir

* Nettoyer : ```make clean``` à la racine du projet (/)
* Documentation (dont rapport) : /doc
