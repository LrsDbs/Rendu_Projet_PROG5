#ifndef _TYPE_LISTE_SECTION_H_

#define _TYPE_LISTE_SECTION_H_
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int n;
  int taille_max;
  Elf32_Shdr* tab;
} liste_elf32_Shdr;

/* Constructeurs */

/* Creer une liste vide */
void creer_liste_S(liste_elf32_Shdr * p,int taille);

/* Operations d'acces */

/* Retourne vrai ssi p est vide */
int est_vide_S(liste_elf32_Shdr p);

/* Renvoie le nombre d'elements dans la liste */
int taille_S(liste_elf32_Shdr p);

/* lit l'element a l'indice ind*/
/*Return 0 si ca s est bien passe et le symbole sera dans le 3eme argument
  Return 1 si il y a eu une erreur*/
int get_symbol_S(liste_elf32_Shdr p, int ind, Elf32_Shdr* elem);

/* Operations de modification */

/* Vider la liste p */
void vider_S(liste_elf32_Shdr * p);

/* ajouter a la liste un Elf32_Sym x */
int ajouter_S(liste_elf32_Shdr * p, Elf32_Shdr x);

void re_alloc_S(liste_elf32_Shdr * p);

/* Supprimer l'Elf32_Sym en haut de la liste */
int supprimer_S(liste_elf32_Shdr * p);

/* Free la structure */
void free_liste_S(liste_elf32_Shdr * p);
#endif