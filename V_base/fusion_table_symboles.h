#ifndef _FUSION_TABLE_SYMB_H_

#define _FUSION_TABLE_SYMB_H_
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "table_string.h"
#include "type_liste_section.h"
#include "lecture.h"
#include "ecriture.h"



int max(int i1, int i2);
/*
return :
  the index if the symbol is define in table_symb
  -1 if the symbol doesn't exist
*/
int search_same_name(char * name, char * name_file);

int is_define_in_table(char * name_symb,Elf32_Half ndx, liste_elf32_sym table_symb, char * stringtable);

Elf32_Shdr new_string_sect(char* source, Elf32_Ehdr header, int offsetstring, int offset, table_string* table, table_string* table_source);

void concat_table_symb(liste_elf32_sym list_symb1,char* stringtable1, liste_elf32_sym list_symb2, char * stringtable2, liste_elf32_sym * final_list_symb, table_string* final_stringtable);

int cont_sect(FILE* res, char* s1, char* s2, int v1, int v2, int offset, Elf32_Shdr sectheader1[], Elf32_Shdr sectheader2[], Elf32_Ehdr header_s1, Elf32_Ehdr* header, table_string* table, liste_elf32_Shdr* liste_section);

int main(int argc, char *argv[]);
#endif
