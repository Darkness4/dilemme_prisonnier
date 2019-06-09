# Programmation Système Projet

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

L'objectif est d'obtenir **un maximum de points**.

Chaque joueur commence avec un nombre de points par défaut.

Chaque partie se constitue de plusieurs matches 1v1.

Pendant ces matches, les deux joueurs doivent choisir entre **trahir** et
**coopérer**.

Une fois que les choix sont faits, un de ces cas va se présenter:

- Si A et B trahissent, A et B perdent des points ensembles
- Si A trahit B, A vole des points à B (et vice versa)
- Si A et B coopèrent, A et B gagnent des points ensembles

Notre version multijoueur est différente de l'original:

- Chaque joueur s'affronte les uns contre les autres. (Il y a 2 parmi n combinaisons différentes)
- Chaque match dure 5 rounds (par défaut)
- Le nombre de points est sauvegardé durant toute la partie. La victoire est uniquement décisive à la fin de la partie.
- Les joueurs peuvent se parler entre eux. On pousse ainsi la création d'alliance.
- Les joueurs partent avec un nombre de points au départ.

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
  - Passage immédiat en **phase de connexion**

## Build

```bash
cd dilemme-prisonnier
make -j4  # ou make executable -j4
          # Si debug: make debug -j4
          # Egalement, make clean (objs) ou make remove (objs + bin)
```

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

Choisissez le port dans lequel l'hôte doit ouvrir la connection.
Ce port entrant doit être ouvert en TCP.

```sh
./bin/dilemme_prisonnier_server <port>
```

Pour obtenir de l'aide:

```sh
./bin/dilemme_prisonnier_server -h
```

#### Client

Choisissez l'ip, le port et le pseudonyme dans lequel le joueur utilisera.

```sh
./bin/dilemme_prisonnier_client <ip> <port> <pseudo>
```

Pour obtenir de l'aide:

```sh
./bin/dilemme_prisonnier_client -h
```

## Documentation annexe

[Documentation Doxygen](http://marc.nguyen.gitlab.emse.fr/projets6-marcnguyen-thomaslardy/)
