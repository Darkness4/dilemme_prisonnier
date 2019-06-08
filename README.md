# Programmation Système Projet

## Dilemme du Prisonnier Multijoueur (avec chat)

Par Marc NGUYEN et Thomas LARDY.

- [Programmation Système Projet](#programmation-système-projet)
  - [Dilemme du Prisonnier Multijoueur (avec chat)](#dilemme-du-prisonnier-multijoueur-avec-chat)
  - [Fonctionnalités](#fonctionnalités)
  - [Build](#build)
  - [Usage](#usage)
    - [Linux](#linux)
      - [Server](#server)
      - [Client](#client)
  - [Documentation annexe](#documentation-annexe)

## Fonctionnalités

- Chat
- Dilemme du Prisonnier

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

Par défaut, les paramètres de jeu sont les suivant :

```c
TODO: Fill
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
