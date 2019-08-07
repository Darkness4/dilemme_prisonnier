# Programmation Système Projet

https://gitlab.emse.fr/marc.nguyen/dilemme_prisonnier

## Dilemme du Prisonnier Multijoueur (avec chat)

Par Marc NGUYEN et Thomas LARDY.

- [Programmation Système Projet](#programmation-système-projet)
  - [Dilemme du Prisonnier Multijoueur (avec chat)](#dilemme-du-prisonnier-multijoueur-avec-chat)
  - [Règles du jeu](#règles-du-jeu)
  - [Déroulement](#déroulement)
  - [Build](#build)
  - [Usage](#usage)
    - [Linux](#linux)
      - [Server](#server)
      - [Client](#client)
  - [Documentation annexe](#documentation-annexe)

## Règles du jeu

L'objectif est d'obtenir le **plus de points possible**.

Chaque joueur commence avec un **nombre de points par défaut**.

Chaque jeu consiste en **plusieurs matchs 1v1**.

Pendant ces matchs, les deux joueurs doivent choisir entre **trahir** et **coopérer**.

Une fois les choix faits, l'un de ces cas se présentera :

- Si A et B trahissent, A et B **perdent des points ensemble**.
- Si A trahit B, A **vole des points** à B (et vice versa)
- Si A et B coopèrent, A et B **gagnent des points ensemble**.

Notre version multijoueur est différente de l'original :

- Chaque joueur est en compétition l'un contre l'autre. (Il y a 2 parmi n joueurs combinaisons)
- Chaque match dure 5 tours (par défaut)
- Le nombre de points est sauvegardé tout au long de la partie. La victoire n'est décisive qu'à la fin de la partie.
- Les joueurs peuvent se parler. Cela favorise la création d'alliances.
- Les joueurs commencent avec un certain nombre de points au départ.

## Déroulement

- **Phase de connexion**
  - Tous les joueurs doivent écrire /pret pour commencer et passer en **phase d'attente**
  - Le chat est public
- **Phase d'attente**
  - Les joueurs doivent écrire /pret pour chercher un match et passet en **phase d'acceptation**
  - Le chat est public
- **Phase d'acceptation**
  - Les deux joueurs doivent écrire /start pour commencer le match
  - Le chat est P2P si /start, sinon il est public
- **Phase de jeu**
  - Les deux joueurs doivent écrire /trahir ou /coop
  - Le chat est P2P
- **Phase finale**
  - Affichage du classement
  - Passage immédiat en **phase de connexion** et réinitialisation des scores.

## Build

Installez les dépendances nécessaires si vous ne l'avez pas déjà fait :

```sh
apt install build-essential cmake
```

Eventuellement, `ninja`, pour compiler rapidement :

```sh
apt install ninja-build
```

Ensuite, utilisez le script Shell (à n'importe quel emplacement) :

```sh
./scripts/build.sh
```

Les fichiers générés sont dans :

- bin/ (executable ici)
- build/ (fichier compilées, libraires, etc)

## Usage

### Linux

#### Server

Par défaut, les paramètres de jeu (situé dans [server.properties](./bin/server.properties)) sont les suivant :

```ini
max-players=24
waiting-time=5
score-default=1000000
score-trahir-coop=400000
score-double-trahir=-200000
score-double-coop=100000
max-rounds=5
```

Le fichier [server.properties](./bin/server.properties) doit être à côté de l'executable.

Choisissez le port dans lequel l'hôte doit ouvrir la connection.
Ce port entrant doit être ouvert en TCP.

```sh
cd bin
./dilemme_prisonnier_server <port>
```

Pour obtenir de l'aide:

```sh
./dilemme_prisonnier_server -h
```

#### Client

Choisissez l'ip du serveur, le port et le pseudonyme dans lequel le joueur utilisera.

```sh
cd bin
./dilemme_prisonnier_client <ip> <port> <pseudo>
```

Pour obtenir de l'aide:

```sh
./dilemme_prisonnier_client -h
```

## Documentation annexe

[Documentation Doxygen](http://marc.nguyen.gitlab.emse.fr/dilemme_prisonnier)
