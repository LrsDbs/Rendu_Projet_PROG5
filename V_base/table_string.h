#ifndef _TABLE_STRING_H_

#define _TABLE_STRING_H_

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture.h"

typedef struct {
  int val;
  char* string;
} table_string;


/* creation de la table
Arguments:      Elf32_Ehdr header1: header du premier fichier a fusioner
                Elf32_Ehdr header2: header du deuxieme fichier a fusioner
                char* source1: nom du premier fichier a fusioner
                char* source2: nom du deuxieme fichier a fusioner
*/
table_string creat_table_string(Elf32_Ehdr header1, Elf32_Ehdr header2, char* source1, char* source2);

/* initialisation de la table
Arguments:      table_string *table: table de string
                int size: taille de la table
*/
table_string creat_table_string_symb(Elf32_Word size1, Elf32_Word size2);
void init_table(table_string *table, int size);

/* ecriture dans la table
Arguments:      table_string *table: table de string
                Elf32_Shdr section: entete de la section en modification
                Elf32_Ehdr header: header du fichier source
                char* source: fichier source
*/
void write_table(table_string* table, Elf32_Shdr section, Elf32_Ehdr header, char* source);


void write_table_Symb(table_string* table, char* stringtable);
#endif
