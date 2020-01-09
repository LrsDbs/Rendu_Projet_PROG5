#ifndef _FUSION_H_

#define _FUSION_H_
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "lecture.h"
#include "ecriture.h"
#include "gestion_endian.h"
#include "type_liste_section.h"
#include "table_string.h"

int max(int i1, int i2);

int is_define_in_table(char * name_symb,Elf32_Half ndx, liste_elf32_sym table_symb, char * stringtable);

void concat_table_symb(liste_elf32_sym list_symb1,char* stringtable1, liste_elf32_sym list_symb2, char * stringtable2, liste_elf32_sym * final_list_symb, table_string* final_stringtable);

/*                  Partie 6
Indice de la section contenant le nom donnee
Retour:         Retourne l'index de la section name ou -1 si elle n'existe pas
Arguments:      char * name: nom a retrouve
                char * name_file: fichier elf ouvert au prealable
*/
int search_same_name(char * name, char * name_file);

/*                  Partie 6
Fusionne 2 sections
Retour:         Retourne l'offset courant
Arguments:      FILE* res: Fichier ouvert resultat (en ecriture)
                char* s1: nom du fichier source 1
                char* s2: nom du fichier source 2
                int v1: L'indice de la section source 1 a fusionner
                int v2: L'indice de la section source 2 a fusionner
                int offset: indice courant
                Elf32_Shdr sectheader1[]: table des sections de source 1
                Elf32_Shdr sectheader2[]: table des sections de source 2
                Elf32_Ehdr header_s1 : header du premier fichier a fusioner
                Elf32_Ehdr* header: header du nouveau fichier
                table_string* table: table de string
                liste_elf32_Shdr* liste_section : tous les headers de section
*/
int cont_sect(FILE* res, char* s1, char* s2, int v1, int v2, int offset, Elf32_Shdr sectheader1[], Elf32_Shdr sectheader2[], Elf32_Ehdr header_s1, Elf32_Ehdr* header, table_string* table, liste_elf32_Shdr* liste_section);

/*
Creer la nouvelle table de string
Retour:         l entete de la nouvelle section de string
Arguments:      char* source: nom du fichier source
                Elf32_Ehdr header : le header du fichier elf
                int offsetstring : offset de la section
                int offset: indice courant
                table_string* table: tableau contenant tout les strings
                table_string* table_source : tableau contenant des strings
*/
Elf32_Shdr new_string_sect(char* source, Elf32_Ehdr header,int offsetstring, int offset, table_string* table, table_string* table_source);


#endif