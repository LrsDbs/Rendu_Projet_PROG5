#ifndef _AFFICHAGE_H_

#define _AFFICHAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "type_liste_symb.h"
#include "lecture.h"
#include "elf32_reloc_table.h"


/*
                Partie 1.1
Affiche le header du fichier elf
Arguments:    Elf32_Ehdr header: header du fichier en little endian
*/
void print_header( Elf32_Ehdr header);

/*
                Partie 1.2
Affiche la table de section
Arguments:      Elf32_Shdr sectionheaders[]: table de sections
                int size: nombre de section dans la table de section
                char* stringtable: table de string
*/
void print_section_table(Elf32_Shdr sectionheaders[], int size, char* stringtable);

/*
                Partie 1.3
Affiche la table de section
Retour:         
Arguments:      Elf32_Shdr section: la section a afficher
                FILE *f: fichier elf ouvert au prealable
                char* nameSec: table de string
*/
int print_section(Elf32_Shdr section, FILE *f, char * nameSec);

/*
                Partie 1.4
Affiche la table de section
Retour:         
Arguments:      liste_elf32_sym listesymb: la table de symbole a afficher
                char* stringtable: table de string des symboles
*/
void print_tablesymbol(liste_elf32_sym listesymb,char* stringtable);


/*
Partie 1.5
Association du type de la reimplementation en chaine de caracteres.
Retour: la chaine de caractere correspondante au type passe en entree
Arguments:      Elf32_Word type, le type de la reimplementation
                Elf32_Half le type machine utilise
*/
char * get_reloc_type(Elf32_Word type, Elf32_Half machine);

/*
            Partie 1.5
Affiche les tables de reimplémentation du fichier concerne
Arguments:      char* name_file le nom du fichier concerne
                elf32_section_reloc la table des sections contenant des reimplementations
*/
void print_relocation_table(char * name_file, elf32_section_reloc relocationtable);
#endif
