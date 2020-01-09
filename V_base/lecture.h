#ifndef _LECTURE_H_

#define _LECTURE_H_

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "type_liste_symb.h"
#include "gestion_endian.h"
#include "verification.h"
#include "elf32_reloc_table.h"


FILE* ouverture_fich_lecture(char* name);

/*
                Partie 1.1
Recupere le header du fichier elf
Retour :      Elf32_Ehdr : header fichier elf en big endian
Arguments:    char* name : nom du fichier a ouvrir
*/
Elf32_Ehdr load_header(char* name);


/*
            Partie 1.2
Recupere une section du fichier elf
Retour :        Elf32_Shdr : header d une section en big endian
Arguments:      char* name : nom du fichier a ouvrir
                int offset : le numero de bit a partir duquel charger le fichier 
*/
Elf32_Shdr load_section(char* filename, int offset);


/*
            Partie 1.2
Recupere la table de section
Retour: les sections de sectionheaders seront en big endian
Arguments:      char* filename: nom du fichier
                Elf32_Ehdr header: Header du fichier elf  en little endian
                Elf32_Shdr sectionheaders[]:  tableau de section vide
*/
void load_tablesection(char* filename, Elf32_Ehdr header, Elf32_Shdr sectionheaders[]);


/*
Recupere une table de string
Retour :        char*: table de string
Arguments:      char* filename: nom du fichier
                int offset : le numero de bit a partir duquel se trouve la table de string
*/
char* load_stringtable(char* name_file,int offset);


/*
            Partie 1.4
Recupere un symbole
Retour :        Elf32_Sym : un Symbole en big endian
Arguments:      char* filename: nom du fichier elf
                int offset: le numero de bit a partir duquel se trouve le symbole
*/
Elf32_Sym load_symb(char* filename, int offset);


/*
            Partie 1.4
Recupere la table de symbole
Arguments:      char* file: nom du fichier elf 
                Elf32_Ehdr header: header du fichier elf en little endian
                Elf32_Shdr section[]: tableau avec toutes les sections avec les sections en little endians
                liste_elf32_sym* list_symb: liste qui contiendra tous les symboles (qui seront en big endian): represente la table de symbole
*/
void load_tablesymbole(char* file,Elf32_Ehdr header, Elf32_Shdr section[], liste_elf32_sym* list_symb);


/*
            Partie 1.4
Recupere l emplacement de la table de string des symboles
Retour:         le numero de bit a partir duquel se trouve la table de string
Arguments:      Elf32_Ehdr header: header du fichier elf en little endian
                Elf32_Shdr section[]: tableau avec toutes les sections avec chaque section en little endian
                char* stringtable: la table de string // venant du header \\
*/
int load_offset_symb(Elf32_Ehdr header, Elf32_Shdr section[], char* stringtable);

/*
Partie 1.5
Récupere toutes les sections contenant des reimplementations et leur reimplementations
Arguments:      char* filename le nom du fichier concerne
                elf32_section_reloc* relocationtable la table qui contiendra toutes les sections
*/
void load_relocation_table(char *filename, elf32_section_reloc* relocationtable);

#endif