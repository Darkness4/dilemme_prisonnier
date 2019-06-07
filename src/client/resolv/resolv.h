#ifndef RESOLV_INCLUDE_H
#define RESOLV_INCLUDE_H

/* module resolv: resolution DNS */

/*
  resolv convertit un nom d'hote (nom de domaine ou aaa.bbb.ccc.ddd) + un nom
  de service en une adresse IP exprimee sous la forme d'une structure
  struct sockaddr_in dont l'adresse est retournee
  en cas d'erreur, elle affiche un message et retourne NULL
*/
struct sockaddr_in *resolv(const char *host, const char *service);

/*
  stringIP convertit une adresse IP exprimee sous forme d'un entier dans
  l'orde de l'hote en une adresse IP exprimee sous forme d'une chaine
  aaa.bbb.ccc.ddd
  elle retourne NULL en cas d'erreur
  la chaine retournee est dans une variable statique, donc ecrasee a chaque
  appel
*/
char *stringIP(unsigned int entierIP);

#endif
